// autotest_kr.h: interface for the autotest_kr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOTEST_KR_H__FD667D0A_026C_4E60_90A7_0B0F9C392FE7__INCLUDED_)
#define AFX_AUTOTEST_KR_H__FD667D0A_026C_4E60_90A7_0B0F9C392FE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "string_util.h"
#include "key_korean_data_h.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class autotest_kr
{
public:
      map <wstring, wstring>
      mKeyin;

      key_korean_data_ID
      mKeyID;

public:
      autotest_kr();
      virtual ~autotest_kr();

      void Initial_KeyID();

      void calcTest(const char* sFile);
      void runTest(const char* sFile, bool chk_seperate, bool chk_ui_action);

      bool runTestLine(string sLine, long index, bool chk_seperate, bool chk_ui_action);
      bool runTestKeyin(wstring wKeyin, wstring wAns, long index, bool chk_seperate, bool chk_ui_action);

      bool numberGetResult(wstring wKeyin, wstring &wKrKey, wstring &wGetAns, bool chk_seperate, bool chk_ui_action);

      virtual void evt_onPause() { }
      virtual void evt_progress(long index) { }
      virtual void evt_setRange(long min, long max) { }
      virtual void evt_errReport(long line, wstring msg) { }
      virtual void evt_errReportEx(long line, wstring wKeyinNumber, wstring wKeyin, wstring wAns, wstring wGetAns, int cmp) { }
};

#endif // !defined(AFX_AUTOTEST_KR_H__FD667D0A_026C_4E60_90A7_0B0F9C392FE7__INCLUDED_)
