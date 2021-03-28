/* 
	RK4OM Implementation/Source File
*/
#include "RK4OM.h"

CRK4OM::CRK4OM ()
{
}

CRK4OM::~CRK4OM ()
{
}

bool CRK4OM::Solve (CVector<double>& dVy,
                    CVector<double>& dVdydx,
                    int n,
                    double xs, double ys,
                    double h,
                    void (*uf)(double, double&))
{
	// function value at the initial point
   double y = ys;
   double x = xs;
   dVy(1) = y;
   double yt = y;
   (*uf)(x, yt);
   dVdydx(1) = yt;

	// loop through the other points
   for (int i=2; i <= n; i++)
   {
      // first term
      double yt = y;
      (*uf)(x, yt);
      double k1 = yt;

      // second term
      double xt = x + 0.5*h;
      yt = y + k1*0.5*h;
      (*uf)(xt, yt);
      double k2 = yt;

      // third term
      xt = x + 0.5*h;
      yt = y + k2*0.5*h;
      (*uf)(xt, yt);
      double k3 = yt;

      // fourth term
      xt = x + h;
      yt = y + k3*h;
      (*uf)(xt, yt);
      double k4 = yt;

      // apply recurrence forumla
      double yip1 = y + (k1+2.0*k2+2.0*k3+k4)*h/6.0;

      // store the estimated solution
      dVy(i) = yip1;
      
      // update values for the next point
      y = yip1;
      x = xs + (i-1)*h;

      // compute derivative at the current point
      (*uf)(xt, yip1);
      dVdydx(i) = yip1;
   }

   return true;
}

