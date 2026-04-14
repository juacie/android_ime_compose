// Data_Type.h: interface for the CData_Type class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_TYPE_H__94323C02_22D7_475B_8B9F_CC1C1F39D1BC__INCLUDED_)
#define AFX_DATA_TYPE_H__94323C02_22D7_475B_8B9F_CC1C1F39D1BC__INCLUDED_

#pragma once
#pragma pack (1)

#if defined(__GPP)
 
 #include <vector>
 #include <iostream>
 using namespace std;

#endif

#include <stdio.h>
#include <string.h>


#if !defined(BYTE) && !defined(BASETYPES)

 typedef short                         INT;
 typedef unsigned short                UINT;
 typedef bool                          BOOL;
 typedef char                          CHAR;
 typedef char*                         LPSTR;
 typedef unsigned char                 BYTE;
 typedef unsigned char                 UCHAR;
 typedef unsigned char*                LPBYTE;
 typedef short                         SHORT;
 typedef unsigned short                USHORT;
 typedef const char*                   LPCSTR;

 #define TRUE                          true
 #define FALSE                         false

#endif

typedef const unsigned char*           LPCBYTE;

#endif // !defined(AFX_DATA_TYPE_H__94323C02_22D7_475B_8B9F_CC1C1F39D1BC__INCLUDED_)
