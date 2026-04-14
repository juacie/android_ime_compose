// HTTP_Imp_Wnd.cpp: implementation of the CHTTP_Imp_Wnd class.

#include "stdafx.h"
#include "HTTP_Imp_Wnd.h"

#if defined(__WND)

#include <atlbase.h>
#include <afxsock.h>
#include <afxinet.h>
#include <wininet.h>

#include "Data_Host.h"
#include "ClientMeta.h"

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static void LogMsg(LPCTSTR sMsg)
{
     TRACE(sMsg);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
CHTTP_Imp_Wnd::CHTTP_Imp_Wnd()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
CHTTP_Imp_Wnd::~CHTTP_Imp_Wnd()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void CHTTP_Imp_Wnd::saveData()
{
     FILE
     *fp =fopen("/tmp/klg.dat", "w");

     if ( fp ==NULL )
     {
          fp =fopen("/var/klg.dat", "w");
     }

     if ( fp !=NULL )
     {
          putw(param.iSucc, fp);
          fclose(fp);
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void CHTTP_Imp_Wnd::loadData()
{
     FILE
     *fp =fopen("/tmp/klg.dat", "r");

     if ( fp ==NULL )
     {
          fp =fopen("/var/klg.dat", "r");
     }

     if ( fp !=NULL )
     {
          param.iSucc =getw(fp);
          fclose(fp);
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void CHTTP_Imp_Wnd::removeData()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
INT  CHTTP_Imp_Wnd::onPost(LPCSTR duid, LPCBYTE sData, INT iLen)
{
     HINTERNET
     hInternet = InternetOpen(_T("Default_User_Agent"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
     
     if ( hInternet==NULL )
     {
          return 0;
     }
     
     USES_CONVERSION;

     LPTSTR
     LstrURL1 =NULL;

     if ( bDebug ==FALSE )
     {
          LstrURL1 =A2T(HOST_NAME_OEM);
     }
     else
     {
          #if defined(__DEBUG) || defined(_DEBUG)
          LstrURL1 =A2T(HOST_NAME_DEV);
          #elif defined(__TPOEM)
          LstrURL1 =A2T(HOST_NAME_OEM);
          #else
          LstrURL1 =A2T(HOST_NAME_DEV);
          #endif
     }
     
     TCHAR
     CstrP[256];
     wsprintf(CstrP, _T("/api.php?type=meta&duid=%s"), A2T(duid));
     
     LPTSTR
     LstrPath1 =CstrP;
     
     HINTERNET
     hConnect = InternetConnect(hInternet, LstrURL1, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
     
     CString
     CstrT;
     CstrT.Format(_T("%s%s"), LstrURL1, CstrP);
     
     if ( hConnect==NULL )
     {
          InternetCloseHandle(hInternet);
          return 0;
     }
     
     HINTERNET
     hRequest = HttpOpenRequest(hConnect, _T("POST"), LstrPath1, NULL, NULL, NULL, 0,  NULL);
     
     if ( hRequest==NULL )
     {
          InternetCloseHandle(hConnect);
          InternetCloseHandle(hInternet);
          return 0;
     }
     
     LPCTSTR
     RH0 =_T("Connection: keep-Alive\r\n");
     
     HttpAddRequestHeaders(hRequest, RH0, _tcslen(RH0), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
     
     LPCTSTR
     RH1 =_T("Content-Type: application/octet-stream; charset=UTF-8\r\n");
     
     HttpAddRequestHeaders(hRequest, RH1, _tcslen(RH1), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
     
     LPCTSTR
     RH3 =_T("Accept-Language: en-US,en;q=0.8\r\n");
     
     //HttpAddRequestHeaders(hRequest, RH3, _tcslen(RH3), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
     
     LPCTSTR
     RH4 =_T("Accept: */*\r\n");
     
     HttpAddRequestHeaders(hRequest, RH4, _tcslen(RH4), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
     
     LPCTSTR
     RH5 =_T("Accept-Encoding: gzip,deflate,sdch\r\n");
     
     //HttpAddRequestHeaders(hRequest, RH5, _tcslen(RH5), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
     
     //------------------------------------------------------------------------
     CString
     CstrT1;
     CstrT1.Format(_T("%s%s....Send Req1"), LstrURL1, CstrP);
     
     INT
     Succ =HttpSendRequest(hRequest, NULL, 0, (char*)sData, iLen);
     
     DWORD
     dwContentLen =0;
     
     DWORD
     dwBufLen =sizeof(dwContentLen);
     
     DWORD
     StatusCode = 0;
     
     DWORD
     StatusCodeLen = sizeof(StatusCode);
     
     HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &StatusCode, &StatusCodeLen, NULL);
     
     if ( StatusCode ==200 )
     {
          LogMsg(_T("Sucess\r\n"));
     }
     else
     {
          TCHAR
          ss[120] ={0};

          wsprintf(ss, _T("Fail =%d\r\n"), StatusCode);

          LogMsg(ss);
          AfxMessageBox(ss);
     }
     
     InternetCloseHandle(hRequest);
     InternetCloseHandle(hConnect);
     InternetCloseHandle(hInternet);

     return 1;
}

#endif
