/* 
   Numerical Computing:

   Main program for 4th Order Runge Kutta Method
   to solve first-order ODEs
*/

#include <iostream>
#include <iomanip>
#include "ODESolver.h"

int main ()
{
   const int PNUM = 1;    // select the problem to solve
   double h, xs, ys, xe;
   if (PNUM == 1)
   {
      // problem 1: y' = f(y(x), x) = -2y
      //            y(x=0) = 2
      //            0 < x < 2.5
      h  = 0.1;
      xs = 0.0;      ys = 2.0;
      xe = 2.5;
   }
   else if (PNUM == 2)
   {
      // problem 2: y' = f(y(x), x) = x^2 - 5x + 10
      //            y(x=0) = 1
      //            0 < x < 2
      h  = 0.1;
      xs = 0.0;      ys = 1.0;
      xe = 2.0;
   }

   const int POINTS = static_cast<int>((xe-xs)/h + 1);
   CVector<double> y(POINTS), dydx(POINTS);
   
   int nFE;
   CODESolver ODESolver(xs, ys, xe, h, POINTS, PNUM);
   ODESolver.GoCompute(y, dydx, nFE);

   const int FW = 15;
   std::cout << "Number of function evaluations: " << nFE << "\n";
   std::cout << std::setw(FW) << "x" 
             << std::setw(FW) << "y(x)"
             << std::setw(FW) << "dy/dx(x)\n";
   for (int i=1; i <= POINTS; i++)
   {
      double x = xs + (i-1)*h;
      std::cout << std::setw(FW) << x 
                << std::setw(FW) << y(i) 
                << std::setw(FW) << dydx(i) 
                << "\n";
   }

   return 0;
}

