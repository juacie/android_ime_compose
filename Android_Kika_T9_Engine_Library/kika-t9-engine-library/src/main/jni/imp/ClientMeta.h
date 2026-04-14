// ClientMeta.h: interface for the CClientMeta class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTMETA_H__65980D55_237A_4314_A0EC_284607BD2EC3__INCLUDED_)
#define AFX_CLIENTMETA_H__65980D55_237A_4314_A0EC_284607BD2EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base64_Imp.h"
#include "Data_Type.h"
#include "Data_Agent.h"
#include "Data_Decode.h"
#include "HTTP_Imp_Wnd.h"
#include "HTTP_Imp_Linux.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
const CHAR A_WORD_TRACE_SDK[]     = "1.0.0.0";
const CHAR A_DUID_IN_HEADER_SDK[] = "1.1.0.0";
const CHAR A_EID_IN_SDK[]         = "2.2.7.0";
const CHAR A_WORD_MULTITRACES[]   = "2.2.0.0";

const bool WRITE_SENDPROCESS_PER_BATCH = true;

const CHAR A_AD[]    = "ad";
const CHAR A_WORD[]  = "word";
const CHAR A_META[]  = "meta";
const CHAR A_EVENT[] = "event";
const CHAR A_ERROR[] = "error";

const CHAR A_type[]    = "meta";
const CHAR A_dayHour[] = "2017010101";


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CMeta_Parser
{
private:
      CData_MemSz            fmt;
      Agent_Data_Header      dui;

public:
      CMeta_Parser();
      virtual ~CMeta_Parser();
      
      INT  ParseData(LPBYTE pByte, UINT MaxLen);

      INT  ParseHeadData(LPBYTE &pByte, UINT MaxLen);

      INT  ReadEncryptContent(LPBYTE &pByte, UINT MaxLen);

      LPCSTR ToString();
      LPCSTR ToStringEx();
      LPCSTR ToStringEx2();
};


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CClientMeta
{
public:
      CPack_Byte             mta;
      CData_MemSz            fmt;

      #if defined(__WND)
      CHTTP_Imp_Wnd          http;
      #elif defined(__LINUX)
      CHTTP_Imp_Linux        http;
      #endif

public:
      CClientMeta();
      virtual ~CClientMeta();
      
      INT  data_pack(LPAgent_Data_Init dui);

      #if defined(__GPP)
      INT  data_unpack(std::string s);
      #endif
};


#endif // !defined(AFX_CLIENTMETA_H__65980D55_237A_4314_A0EC_284607BD2EC3__INCLUDED_)
