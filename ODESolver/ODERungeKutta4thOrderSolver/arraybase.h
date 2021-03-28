/*********************************************
Utility Library Function

Reference:

Object-Oriented Numerical Methods via C++ (2000-18)
by S. D. Rajan

*********************************************/
#ifndef __RAJAN_ARRAYBASE_H__
#define __RAJAN_ARRAYBASE_H__

#include <sstream>

class CArrayBase
{
    public:
        CArrayBase ();
        ~CArrayBase ();

        void static ShowStatistics (std::ostream& OF);

    protected:
        static double m_dAllocated;
        static double m_dDeAllocated;
};

#endif
