#include <iostream>
#include <sstream>
#include <omp.h>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>

/**
 * This code tests different implementations of force computation and makes a small speed test.
 *
 * We compute the forces f_i =\sum_{j=1}^N f_{i,j} using four different implementations:
 *  1) Summation in serial
 *  2) Summation using openMP, defining the += operation as atomic
 *  3) Summation using openMP, using shared variables (which should fail if two threads write at the same time,
 *     but is calculated as an optimal-speed case)
 *  4) Summation using openMP, using a self-written (manual) reduction.
 *     Guided/dynamic scheduling is used as it seems faster than static.
 *
 * The results on a quad-core are:
 * serial,    sum: 0, fsum: 3.38184e+08, clock: 5954120
 * atomic,    sum: 0, fsum: 3.38184e+08, clock: 2040039
 * shared,    sum: 244763, fsum: 3.38184e+08, clock: 1556665
 * reduction, sum: 0, fsum: 3.38184e+08, clock: 1118625
 *
 * The parallel code is about 5 times quicker than the serial code.
 */

struct Particle {
    double position;
    double force;
};

struct DPM {
    std::vector<Particle> particles;

    DPM(int nParticles) {
        std::random_device rd;
        std::uniform_real_distribution<> dist(0, 1);
        particles.resize(nParticles);
        for (auto& p : particles) {
            p.position = dist(rd);
        }
    }

    void computeForce() {
        for (int i=0; i<particles.size(); ++i) {
            particles[i].force = 0;
        }
        for (int i=0; i<particles.size(); ++i) {
            for (int j=i+1; j<particles.size(); ++j) {
                double distance = fabs(particles[i].position - particles[j].position);
                if (distance<.5) {
                    particles[i].force += 1;
                    particles[j].force -= 1;
                }
            }
        }
    }

    void computeForceAtomic() {
        for (int i=0; i<particles.size(); ++i) {
            particles[i].force = 0;
        }
        #pragma omp parallel for
        for (int i=0; i<particles.size(); ++i) {
            for (int j=i+1; j<particles.size(); ++j) {
                double distance = fabs(particles[i].position - particles[j].position);
                if (distance<.5) {
                    #pragma omp atomic
                    particles[i].force += 1;
                    #pragma omp atomic
                    particles[j].force -= 1;
                }
            }
        }
    }

    void computeForceShared() {
        for (int i=0; i<particles.size(); ++i) {
            particles[i].force = 0;
        }
        #pragma omp parallel for
        for (int i=0; i<particles.size(); ++i) {
            for (int j=i+1; j<particles.size(); ++j) {
                double distance = fabs(particles[i].position - particles[j].position);
                if (distance<.5) {
                    particles[i].force += 1;
                    particles[j].force -= 1;
                }
            }
        }
    }

    // see http://forum.openmp.org/forum/viewtopic.php?f=3&t=1647
    void computeForceReduction() {
        for (int i=0; i<particles.size(); ++i) {
            particles[i].force = 0;
        }

        #pragma omp parallel
        {
            std::vector<double> forces(particles.size(),0);

            #pragma omp for schedule (guided)
            for(int i = 0; i < particles.size(); ++i) {
                for (int j = i + 1; j < particles.size(); ++j) {
                    double distance = fabs(particles[i].position - particles[j].position);
                    if (distance < .5) {
                        forces[i] += 1;
                        forces[j] += -1;
                    }
                }
            }

            #pragma omp critical
            {
                for (int i = 0; i < particles.size(); ++i) {
                    particles[i].force += forces[i];
                }
            }

        }
    }

    double check() {
        double sumForces = 0;
        for (int i=0; i<particles.size(); ++i) {
           sumForces += particles[i].force;
        }
        return sumForces;
    }

    double checkAbs() {
        double sumForces = 0;
        for (int i=0; i<particles.size(); ++i) {
            sumForces += fabs(particles[i].force);
        }
        return sumForces;
    }

    void getNumberOfThreads() {
        std::cout << "Number of threads:";
        #pragma omp parallel
        {
            #pragma omp critical
            std::cout << ' ' << omp_get_num_threads();
        }
        std::cout << std::endl;
    }
};

int main ()
{
    int N=30000;
    DPM dpm(N);
    //dpm.getNumberOfThreads();

    auto start = std::chrono::system_clock::now();
    dpm.computeForce();
    auto end = std::chrono::system_clock::now();
    std::cout << "serial,    sum: " << dpm.check() << ", fsum: "  << dpm.checkAbs() << ", clock: " << (end-start).count() << "\n";

    start = std::chrono::system_clock::now();
    dpm.computeForceAtomic();
    end = std::chrono::system_clock::now();
    std::cout << "atomic,    sum: " << dpm.check() << ", fsum: "  << dpm.checkAbs() << ", clock: " << (end-start).count() << "\n";

    start = std::chrono::system_clock::now();
    dpm.computeForceShared();
    end = std::chrono::system_clock::now();
    std::cout << "shared,    sum: " << dpm.check() << ", fsum: "  << dpm.checkAbs() << ", clock: " << (end-start).count() << "\n";

    start = std::chrono::system_clock::now();
    dpm.computeForceReduction();
    end = std::chrono::system_clock::now();
    std::cout << "reduction, sum: " << dpm.check() << ", fsum: "  << dpm.checkAbs() << ", clock: " << (end-start).count() << "\n";

    return 0;
}
