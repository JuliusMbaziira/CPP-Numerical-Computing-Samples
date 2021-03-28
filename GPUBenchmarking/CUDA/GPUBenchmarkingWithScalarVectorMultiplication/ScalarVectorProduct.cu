/* 
 * 
 * GPU Computing: Benchmarking CUDA with the Scalar-Vector Product 
 * Computes only one element in a single thread and uses N threads with 1
 * Single Thread-Per-Block.
 * 
 */

#include <stdio.h>
#include <time.h>
#include <cuda.h>

#define SCALAR 3

float printRunTime(const char *string, struct timespec *ti1, struct timespec *ti2)
{
   float runtime;
   
   runtime = (ti2->tv_sec - ti1->tv_sec ) + 1e-9*(ti2->tv_nsec - ti1->tv_nsec);
   fprintf(stderr,"Run time %s : %f microsecs.\n", string, runtime * 1e6);
   return runtime;
}

void show(float *a, long n)
{
   int i,j;
   int max = 10;
   
   j = (n > max) ? n-max : 0;
   
   printf("Results (max last %d) : \n",max);
   
   for(i=j; i<n; i++)
     printf("%3d : %6.2f\n",i,a[i]);
}

// This runs on the CPU :
float multiplyOnHost(float *a, long n, float s)
{
   long i;
   struct timespec ti1,ti2;
   
   fprintf(stderr,"Now computing on CPU : \n");
   clock_gettime(CLOCK_REALTIME,&ti1);        // read starttime into t1
   
   for(i=0;i<n;i++)
     a[i] *= s;
   
   clock_gettime(CLOCK_REALTIME,&ti2);        // read endtime into t2
   return printRunTime("CPU",&ti1,&ti2);
}

// This is the kernel that runs on the GPU :
__global__ void multiplyKernel(float *b, float s)
{
   int i = blockIdx.x * blockDim.x + threadIdx.x;
   b[i] *= s;
}

// This part also runs on the CPU (and not on the GPU !) :
float multiplyOnDevice(float *b_h, long n, float s)
{
   float *a_d;   // Pointer to device array
   struct timespec ti1,ti2,ti3,ti4;
   int nBlocks;
   
   // Do the actual calculation on N threads inside the GPU device:
   nBlocks = n;
   if(nBlocks > 65535)
   {
      fprintf(stderr,"Maximum number of blocks exceeded : %d > 65535 !\n", nBlocks);
      exit(1);
   }
   fprintf(stderr,"Now computing on GPU (%d blocks, %d threads per block): \n", nBlocks, 1);
   
   cudaMalloc((void **) &a_d, n * sizeof(float)); // Allocate array on device

   clock_gettime(CLOCK_REALTIME,&ti1);            // read starttime into t1
   cudaMemcpy(a_d, b_h, n * sizeof(float), cudaMemcpyHostToDevice);
   
   clock_gettime(CLOCK_REALTIME,&ti2);            // read starttime into t2
   multiplyKernel <<< nBlocks, 1 >>> (a_d, s);
   cudaDeviceSynchronize();                       // We make sure all threads finished before stopping clock
   
   clock_gettime(CLOCK_REALTIME,&ti3);            // read endtime into t3
   
   // Retrieve result from device and store it in host array
   cudaMemcpy(b_h, a_d, sizeof(float)*n, cudaMemcpyDeviceToHost);

   clock_gettime(CLOCK_REALTIME,&ti4);            // read endtime into t4
   cudaFree(a_d);
   
   printRunTime("GPU (no data transfer)",&ti2,&ti3);

   return printRunTime("GPU (including data transfer)",&ti1,&ti4);
}

// main routine that executes on the host
int main(int argc, char **argv)
{
   float *a_h,*b_h;      // Pointers to host arrays
   long n = 4096L;       // Number of elements in arrays (problemsize)
   long i;
   float ts,tp;
   float s = SCALAR;
   
   if(argc >=2 )
     sscanf(argv[1],"%ld",&n);

   fprintf(stderr,"Starting with n=%ld ....\n",n);
   
   // Allocate arrays on host
   a_h = (float *)malloc(n * sizeof(float));
   b_h = (float *)malloc(n * sizeof(float));

   // Initialize host arrays 
   for (i=0; i<n; i++)
     b_h[i] = a_h[i] = (float)i;
   
   // Run sequential algorithm on CPU
   ts = multiplyOnHost(a_h, n, s);
   
   // Run parallel algorithm on GPU
   tp = multiplyOnDevice(b_h, n, s);
   
   // check results
#if (DEBUG > 0)
   show(a_h,n);

   printf("Comparing results ....\n");
   int diffresults=0;
   for (i=0; i<n; i++)
   {
      float f = fabs(a_h[i] - b_h[i]);
      if(f > 1e-6)
      {	
	 printf("CPU: %f , GPU: %f , diff: %f\n",a_h[i],b_h[i],f);
	 diffresults=1;
      }
   }
   if(diffresults == 0)
      printf("Both results are equal up to 1e-6\n");
#endif
   
   printf("Ratio execution time GPU/CPU: %f\n",tp/ts);

   free(a_h);
   free(b_h);
   exit(0);
}

