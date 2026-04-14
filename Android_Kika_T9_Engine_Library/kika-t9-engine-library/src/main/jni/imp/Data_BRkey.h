// Data_BRkey.h: interface for the CData_BRkey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_BRKEY_H__11410CCF_76C8_49C0_B99E_8272D26F1EE9__INCLUDED_)
#define AFX_DATA_BRKEY_H__11410CCF_76C8_49C0_B99E_8272D26F1EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data_Mem.h"

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
struct BRKEY_UI
{
       CData_Mem             bBKey;
       CData_Mem             bRsaBKey;
};


#define MAX_BRKEY                      5

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CData_BRkey
{
private:
      USHORT            index;
      BRKEY_UI          br[MAX_BRKEY];

public:
      CData_BRkey();
      virtual ~CData_BRkey();

      void generateKey();

      USHORT getIndex()
      {
          return index;
      }

      LPBYTE getBKey()
      {
          return (LPBYTE)br[index].bBKey.getMem();
      }

      LPCSTR getRsaBKey()
      {
          return br[index].bRsaBKey.getMem();
      }
};


#endif // !defined(AFX_DATA_BRKEY_H__11410CCF_76C8_49C0_B99E_8272D26F1EE9__INCLUDED_)
