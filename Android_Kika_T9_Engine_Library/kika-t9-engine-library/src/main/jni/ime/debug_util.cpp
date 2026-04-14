// debug_util.cpp: implementation of the debug_util class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "debug_util.h"

#include <iostream>
using namespace std;


#if defined(_WIN32)

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
void d_pintf(const char * msg)
{
     TRACE(_T("%s"), msg);
}

void d_pintf_ex(const char * msg)
{
     TRACE(_T("%s"), msg);
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
void dw_pintf(const wchar_t * msg)
{
     TRACE(_T("%s"), msg);
}

void dw_pintf_ex(const wchar_t * msg)
{
     TRACE(_T("%s"), msg);
}
#endif
