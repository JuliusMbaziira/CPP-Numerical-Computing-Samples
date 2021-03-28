/* 
	Source file: ODE Solver Implementation
*/

#include <iostream>
#include "ODESolver.h"
#include "RK4OM.h"

int CODESolver::m_nCount = 0;
int CODESolver::m_Problem = 0;

CODESolver::CODESolver (double xs, double ys, 
                        double xe, double h, int POINTS,
                        int PNUM)
{
   m_nSize = POINTS;
   m_xs = xs;
   m_ys = ys;
   m_xe = xe;
   m_h = h;
   m_Problem = PNUM;
}

CODESolver::~CODESolver ()
{
}

void CODESolver::GoCompute (CVector<double>& y,
                            CVector<double>& dydx,
                            int& nFE)
{
   CRK4OM RK4OM;
   RK4OM.Solve (y, dydx, m_nSize, m_xs, m_ys, m_h, 
                UserFunctionEvaluation);
   nFE = m_nCount;
}

void CODESolver::UserFunctionEvaluation (double x,
                                         double& y)
//
// the function evaluates the function f(x) value
// at the current point given in x
//
// problem 1: y' = f(y(x), x) = -2y
//            y(x=0) = 2
//            0 < x < 2.5
//
// problem 2: y' = f(y(x), x) = x^2 - 5x +10
//            y(x=0) = 1
//            0 < x < 2
{
   // counter to see how many function evaluations take place
   ++m_nCount;

   if (m_Problem == 1)
   {
      y = -2.0*y;
   }
   else if (m_Problem == 2)
   {
      y = x*x - 5.0*x + 10.0;
   }
}
