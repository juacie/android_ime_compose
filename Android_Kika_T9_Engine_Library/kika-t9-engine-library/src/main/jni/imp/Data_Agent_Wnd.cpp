// Data_Agent_Wnd.cpp: implementation of the CData_Agent_Wnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Data_Host.h"
#include "Data_Agent_Wnd.h"

#if defined(__WND)
 #include <atlbase.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
CData_Agent_Wnd::CData_Agent_Wnd()
{
     
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
CData_Agent_Wnd::~CData_Agent_Wnd()
{
     
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
void GetCPUID(LPCData_Mem dui)
{
     #if defined(__WND)

     CRegKey
     RegX;

     CString
     CstrKey =_T("SOFTWARE\\Microsoft\\Cryptography");

     HRESULT
     lResult =RegX.Open(HKEY_LOCAL_MACHINE, CstrKey);

     if ( lResult ==ERROR_SUCCESS )
     {
          DWORD
          dwLen =MAX_PATH;

          TCHAR
          LstrID[MAX_PATH];

          #if defined(__2005)
          RegX.QueryMultiStringValue(LstrID, _T("MachineGuid"), &dwLen);
          #else
          RegX.QueryValue(LstrID, _T("MachineGuid"), &dwLen);
          #endif

          RegX.Close();

          USES_CONVERSION;

          dui->set(T2A(LstrID));
     }
     else
     #endif
     {
          dui->set("TEST_001122334455667788");
     }
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
void CData_Agent_Wnd::init()
{
     GetCPUID(&dui.d_duid);

     dui.d_sdk_version.set("5.5.8");
     dui.d_language.set("");
     dui.d_appKey.set(APPKEY);

     dui.d_app_version.set(KERNEL_VER);
     dui.d_agt_version.set(KERNEL_VER);
     dui.d_os_version.set("4.1.2");

     dui.d_op.set("Windows");
     dui.d_pf.set("Windows");
     dui.d_ch.set("Window Metro");

     //char
     //sData[120];
}
