// autotest_v1.h: interface for the autotest_v1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOTEST_V1_H__E7B2FB42_370D_4329_A532_1118DB27421D__INCLUDED_)
#define AFX_AUTOTEST_V1_H__E7B2FB42_370D_4329_A532_1118DB27421D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string_util.h"

struct KEY_ANS
{
     wstring                 key; 
     vector<wstring>         ans;
};

struct KEY_LANG
{
     string                  name; 
     int                     id; 
     int                     count; 
     vector<KEY_ANS>         keys;
};


//-----------------------------------------------------------------------------
class autotest_v1
{
public:
      KEY_LANG               lang;
      vector<KEY_LANG>       mKeys;

public:

      autotest_v1();
      virtual ~autotest_v1();

      void actionDone();
      void action(const std::string& text, const std::string& delims);

      std::vector<std::string> splitCH(const std::string& text, const char seperator);
      std::vector<std::wstring> split(const std::string& text, const std::string& delims);
      std::vector<std::string> splitGet(const std::string& text, const std::string& delims);
};

#endif // !defined(AFX_AUTOTEST_V1_H__E7B2FB42_370D_4329_A532_1118DB27421D__INCLUDED_)