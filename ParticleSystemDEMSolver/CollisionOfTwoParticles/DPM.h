#include "Vec3D.h"
#include <vector>
#include <fstream>
#include <cmath>

/* Spherical particle derivation (definition/declarations of the the particle properties) */ 
struct Particle { //properties of a spherical particle
	double a; //radius
	double m; //mass
	Vec3D r;  //position
	Vec3D v;  //velocity
	Vec3D f;  //force
};

/* Flat wall derivation (definition/declarations of the wall properties) */
struct Wall {
	Vec3D n; //normal pointing into the wall
	Vec3D P0; //point on the wall
};

/* Derivation of the DPM/DEM solver (definition/declarations of the Discrete Particle Method ) */
struct DPM {
	/* global parameters */
	double dt; //time step
	double tMax; //final time
	double k; //stiffness 
	double gam; //dissipation coefficient
	unsigned int saveCount = 10; //number of timestep between saves
	std::vector<Particle> particles; //vector of particles
	std::vector<Wall> walls; //vector of walls
	Vec3D gravity;
	
	/* solve contains the time stepping algorithm and the output routines */ 
	void solve () {
		/* initialise time, and counter/ofstream for data output */
		double t=0;
		unsigned int counter = 0;
		std::ofstream file("data");
		while (t<tMax) {		
			/* Compute the gravitational body force for all particles */
			for (Particle& p : particles) {
				p.f = gravity * p.m;
			}
			
			/* Loop over all particle-wall contacts to calculate forces due to walls */
			for (Particle& p : particles)
			for (Wall& w : walls) {
				double distance = (w.P0-p.r)*w.n;
				double overlap = p.a-distance;
				if (overlap>=0) {
					double vreln = p.v*w.n;
					double normalForce = k*overlap+gam*vreln; 
					p.f += w.n*(-normalForce);
				}
			}
			
			/* Loop over all particle-particle contacts to calculate particle contact forces */ 
			for (auto p = particles.begin();  p!=particles.end(); ++p)
			for (auto q = p+1;  q!=particles.end(); ++q) {
				double distanceSquared = Vec3D::lengthSquared(p->r-q->r);
				if ((p->a+q->a)*(p->a+q->a)>distanceSquared) {
					double distance = sqrt(distanceSquared); 
					double overlap = p->a+q->a-distance;
					Vec3D normal = (p->r-q->r)/distance;
					double vreln = (p->v-q->v)*normal;
					double normalForce = k*overlap+gam*vreln; 
					p->f += normal*normalForce;
					q->f += normal*(-normalForce);
				}		
			}
			
			/* Integrate position and velocity with the Euler method */
			for (Particle& p : particles) {
				p.r += p.v*dt;
				p.v += p.f*(dt/p.m);
			}
			
			t += dt;
			/* Write output file in the .data format every 10th time step */
			if (++counter>=saveCount) {
				//reset the counter, write time to terminal
				counter = 0;
				std::cout << "t=" << t << std::endl; 
				//write global parameters (time, number of particles, domain size)
				file << particles.size() << " " << t << " 0 0 0 1 1 1\n"; 
				for (Particle& p : particles) {
					//write particle data (position, velocity, radius)
					file << p.r << " " << p.v << " " << p.a << " 0 0 0 0 0 0 0\n";
				}
			}
			
		} //end time loop
		file.close();
	} //end solve
};
