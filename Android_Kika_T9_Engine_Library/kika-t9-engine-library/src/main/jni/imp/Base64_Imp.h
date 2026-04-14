// Base64_Imp.h: interface for the CBase64_Imp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE64_IMP_H__4107889C_C115_48B8_AE2A_DDBB89A6B116__INCLUDED_)
#define AFX_BASE64_IMP_H__4107889C_C115_48B8_AE2A_DDBB89A6B116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Data_Type.h"

#if defined(__GPP)


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);


class CBase64_Imp
{
public:
      CBase64_Imp();
      virtual ~CBase64_Imp();
      
};

#endif

#endif // !defined(AFX_BASE64_IMP_H__4107889C_C115_48B8_AE2A_DDBB89A6B116__INCLUDED_)
