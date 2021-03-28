/* 
	ODESolver declarations
*/

#pragma once
#include "vectortemplate.h"

class CODESolver
{
    public:
        CODESolver (double xs, double ys, double xe, double h, int POINTS,
                    int PNUM);
        ~CODESolver ();

        void GoCompute (CVector<double>& y,
                        CVector<double>& dydx, int& nFE);
        static void UserFunctionEvaluation (double x,
                                            double& y);

    private:
       static int m_Problem;
       int    m_nSize;
       double m_xs;
       double m_ys;
       double m_xe;
       double m_h;
       static int m_nCount;
};
