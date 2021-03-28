/* 
	RK4OM declarations
*/
#pragma once
#include "vectortemplate.h"

class CRK4OM
{

   public:
      CRK4OM ();
      ~CRK4OM ();

      bool Solve (CVector<double>& y,
                  CVector<double>& dydx,
                  int n,
                  double xs, double ys,
                  double h,
                  void (*uf)(double, double&)); 

   private:

};
