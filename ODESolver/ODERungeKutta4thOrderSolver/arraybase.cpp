/*********************************************
Utility Library Function

Reference:

Object-Oriented Numerical Methods via C++ (2000-18)
by S. D. Rajan
*********************************************/
#include <iostream>
#include "arraybase.h"

double CArrayBase::m_dAllocated = 0.0;
double CArrayBase::m_dDeAllocated = 0.0;

CArrayBase::CArrayBase ()
// ----------------------------------------------------------------------------
// Function: constructor
// Input:    None 
// Output:   None
// ----------------------------------------------------------------------------
{
}

CArrayBase::~CArrayBase ()
// ----------------------------------------------------------------------------
// Function: destructor
// Input:    None 
// Output:   None
// ----------------------------------------------------------------------------
{
}

void CArrayBase::ShowStatistics (std::ostream& OF)
// ----------------------------------------------------------------------------
// Function: destructor
// Input:    None 
// Output:   None
// ----------------------------------------------------------------------------
{
    OF << "\n\n";
    OF << "  Allocated : " << m_dAllocated   << " bytes\n"
       << "DeAllocated : " << m_dDeAllocated << " bytes\n";
}
