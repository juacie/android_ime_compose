// autotest_kr.cpp: implementation of the autotest_kr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autotest_kr.h"
#include "trans_key_korean.h"
#include "string_util.h"

#include <fstream>
#include <sstream>
#include <iostream>


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
autotest_kr::autotest_kr()
{
     Initial_KeyID();

     //runTest();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
autotest_kr::~autotest_kr()
{
     mKeyin.clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void autotest_kr::Initial_KeyID()
{
     long
     total =MAX_key_korean_data_ID;
     
     for ( long i =0; i <total; i++ )
     {
           wchar_t*
           pWD =mKeyID.GetWDataSz(i);
           
           pWD[1] =0;

           wchar_t*
           pKey =pWD +2;

           mKeyin[pKey] =pWD;
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void autotest_kr::calcTest(const char* sFile)
{
     std::ifstream infile(sFile);
     std::string   line;
     
     long
     total =0;

     while ( std::getline(infile, line) )
     {
             line.erase(remove(line.begin(), line.end(), '\r'), line.end());
             line.erase(remove(line.begin(), line.end(), '\n'), line.end());
             
             if ( line.size() >0 )
             {
                  total++;
             }
     }
     infile.close();

     evt_setRange(0, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void autotest_kr::runTest(const char* sFile, bool chk_seperate, bool chk_ui_action)
{
     std::ifstream infile(sFile);
     std::string   line;

     long
     index =0;
     while ( std::getline(infile, line) )
     {
             line.erase(remove(line.begin(), line.end(), '\r'), line.end());
             line.erase(remove(line.begin(), line.end(), '\n'), line.end());
             
             evt_progress(++index);

             if ( line.size() >0 )
             {
                  if ( runTestLine(line, index, chk_seperate, chk_ui_action) ==false )
                  {
                       break;
                  }
             }
     }
     infile.close();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool autotest_kr::runTestLine(string sLine, long index, bool chk_seperate, bool m_Chk_UI_Action)
{
     if ( sLine.size() >0 )
     {
          std::size_t
          found =sLine.find(",");
          
          if ( found !=std::string::npos )
          {
               string
               sAns =sLine.substr(0, found);
               
               wstring
               wAns =utf8_to_utf16(sAns);

               const wchar_t*
               p =wAns.c_str();

               if ( p[0] ==0x0feff )
                    wAns.erase(0, 1);

               #if defined(_WIN32)
               TRACE(_T("wAns sz =%d"), wAns.size());
               #endif
               
               sLine.erase(0, found +1);
               
               found =sLine.find(",");
               
               if ( found !=std::string::npos )
               {
                    string
                    sKeyin =sLine.substr(0, found);

                    wstring
                    wKeyin =utf8_to_utf16(sKeyin);

                    if ( runTestKeyin(wKeyin, wAns, index, chk_seperate, m_Chk_UI_Action) ==false )
                    {
                         return false;
                    }
               }
          }
     }
     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool autotest_kr::numberGetResult(wstring wKeyin, wstring &wKrKey, wstring &wGetAns, bool chk_seperate, bool m_Chk_UI_Action)
{
     wstring
     wsKeyTmp =wKeyin,
     wsKeyOrg =wKeyin;

     while ( wKeyin.size() )
     {
             map<wstring, wstring>::iterator
             p =mKeyin.find(wKeyin);
     
             if ( p !=mKeyin.end() )
             {
                  wstring
                  wKey =mKeyin[wKeyin];

                  wKrKey +=wKey;

                  #if defined(_DEBUG_KEY) && defined(_WIN32)
                  {
                      wchar_t
                      aa[1024] ={0};

                      swprintf(aa, L"Get Keyin =%ls, KrKey =%ls", 
                               wKeyin.c_str(), wKrKey.c_str());
                      
                      AfxMessageBox(aa);
                  }
                  #endif

                  wsKeyTmp.erase(0, wKeyin.size());

                  wKeyin =wsKeyTmp;

                  continue;
             }
             wKeyin.erase(wKeyin.size() -1, 1);
     }
     
     trans_key_korean
     korean;

     wGetAns =korean.getComposingText(wKrKey, chk_seperate, m_Chk_UI_Action);

     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool autotest_kr::runTestKeyin(wstring wKeyin, wstring wAns, long index, bool chk_seperate, bool m_Chk_UI_Action)
{
     wstring
     wKrKey;

     wstring
     wsKeyTmp =wKeyin,
     wsKeyOrg =wKeyin;

     while ( wKeyin.size() )
     {
             map<wstring, wstring>::iterator
             p =mKeyin.find(wKeyin);
     
             if ( p !=mKeyin.end() )
             {
                  wstring
                  wKey =mKeyin[wKeyin];

                  wKrKey +=wKey;

                  #if defined(_DEBUG_KEY) && defined(_WIN32)
                  {
                      wchar_t
                      aa[1024] ={0};

                      swprintf(aa, L"Get Keyin =%ls, KrKey =%ls", 
                               wKeyin.c_str(), wKrKey.c_str());
                      
                      AfxMessageBox(aa);
                  }
                  #endif

                  wsKeyTmp.erase(0, wKeyin.size());

                  wKeyin =wsKeyTmp;

                  continue;
             }
             wKeyin.erase(wKeyin.size() -1, 1);
     }
     
     trans_key_korean
     korean;

     wstring
     wGetAns =korean.getComposingText(wKrKey, chk_seperate, m_Chk_UI_Action);

     int
     cmp =wGetAns.compare(wAns);

     evt_errReportEx(index, wsKeyOrg, wKrKey, wAns, wGetAns, cmp);

     if ( cmp !=0 )
     {
          wchar_t 
          aa[1024] ={0};

          #if defined(_WIN32)

          swprintf(aa, L"line =%d, KeyNumb =%ls, Keyin =%ls, Expect =%ls(%d), Get =%ls(%d)\r\n", 
                   index, wsKeyOrg.c_str(), wKrKey.c_str(), wAns.c_str(), wAns.size(), wGetAns.c_str(), wGetAns.size());

          #else

          swprintf(aa, 1024, L"line =%d, KeyNumb =%ls, Keyin =%ls(%d), Expect =%ls(%d), Get =%ls\r\n", 
                   index, wsKeyOrg.c_str(), wKrKey.c_str(), wAns.c_str(), wAns.size(), wGetAns.c_str(), wGetAns.size());

          #endif

          evt_errReport(index, aa);

          return false;
     }
     return true;
}
