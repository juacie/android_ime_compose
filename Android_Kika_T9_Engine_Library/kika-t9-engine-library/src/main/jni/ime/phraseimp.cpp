// phraseimp.cpp: implementation of the phraseimp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phraseimp.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include "debug_util.h"

// #define __DEBUG_MSG
// #define __DEBUG_MSG_EX
// #define __DEBUG_MSG_MEM
#define __ENCODE_MSG

#if defined(__DEBUG_MSG_RUN_MS) || defined(__DEBUG_MSG_LOADDICT_MS) || defined(__DEBUG_MSG_MEM)
 #include <iostream>
 #if !defined(__TEST) && !defined(_WIN32)
  #if !defined(KIKA_LOGD)
   #include <android/log.h>
   #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)
  #endif
 #endif
#endif

short                        phraseimp::wrdLen =0;
short                        phraseimp::keyLen =0;
short                        phraseimp::nwdLen =0;

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
phraseimp::phraseimp()
{
     wrdLen =1;
     keyLen =20;
     nwdLen =0;
     preLoad =false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
phraseimp::~phraseimp()
{
     clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp::clear()
{
     #if defined(__DEBUG_MSG_MEM)
     KIKA_LOGD("Kika-ENGINE, phraseimp::clear()");
     #endif

     vdict.clear();
     vnext.clear();
     memPhrs.clear();
     memKeys.clear();
     memNext.clear();
     
     #if defined(__LINUX_BASE)
     vdict.shrink_to_fit();
     vnext.shrink_to_fit();
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
MEM_INT32 phraseimp::getTimeStamp()
{
    time_t
    tt;

    time(&tt);

    return static_cast<MEM_INT32>(tt);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp::phrsSaveData(const char *fName)
{
     MEM_INT32
     total   =0,
     memSize =0;

     FILE*
     fp =fopen(fName, "w");

     if ( fp )
     {
          fwrite(HEAD_ID_PHRS_V2, sizeof(HEAD_ID_PHRS_V2) *sizeof(char), 1, fp);

          PHRS_ITR
          ii =vdict.begin();

          for ( ii = vdict.begin(); ii != vdict.end(); ii++ )
          {
                // sID +Keys +Phrs
                memSize +=sizeof(MEM_INT) +ii->wsKeys.size() +ii->wsPhrs.size();
          }

          MEM_INT*
          pMem =new MEM_INT[memSize +1];

          if ( pMem != nullptr )
          {
               memset(pMem, 0, memSize +1);

               PHRS_IDX*
               ux =new PHRS_IDX[(vdict.size()<<1) +1];

               if ( ux == nullptr )
               {
                    delete [] pMem;

                    fwrite(&total, sizeof(MEM_INT32), 1, fp);
                    fclose(fp);
                    return false;
               }

               MEM_INT32
               offs =0;

               // one times save 2 records
               for ( ii = vdict.begin(); ii != vdict.end(); ii++, total +=2 )
               {
                     ux[total].offs =offs;
                     ux[total].vLen =ii->wsKeys.size();

                     *(MEM_INT*)(pMem +offs) =ii->sID;

                     offs +=sizeof(MEM_INT);

                     const wchar_t *
                     pWMem =ii->wsKeys.c_str();

                     for ( MEM_INT xw =0; xw <ii->wsKeys.size(); xw++ )
                           pMem[offs +xw] =*(MEM_INT*)&pWMem[xw];

                     offs +=ux[total].vLen;

                     ux[total +1].offs =offs;
                     ux[total +1].vLen =ii->wsPhrs.size();

                     pWMem =ii->wsPhrs.c_str();

                     for ( MEM_INT xp =0; xp <ii->wsPhrs.size(); xp++ )
                           pMem[offs +xp] =*(MEM_INT*)&pWMem[xp];

                     offs +=ux[total +1].vLen;

               }
               fwrite(&total, sizeof(MEM_INT32),    1, fp);
               fwrite(&offs,  sizeof(MEM_INT32),    1, fp);
               fwrite(ux,     sizeof(PHRS_IDX), total, fp);
               fwrite(pMem,   sizeof(MEM_INT),   offs, fp);

               delete [] ux;
               delete [] pMem;

               fclose(fp);

               return true;
          }

          fwrite(&total, sizeof(unsigned int), 1, fp);
          fclose(fp);
     }
     return false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::phrsLoadData(const char *fName)
{
     setlocale( LC_ALL, "");

     clear();

     MEM_INT32
     total   =0,
     memSize =0;

     FILE*
     fp =fopen(fName, "r");

     if ( fp )
     {
          vdict.clear();

          char
          headSz[sizeof(HEAD_ID_PHRS_V2) +1] ={0};

          fread(headSz, sizeof(HEAD_ID_PHRS_V2) *sizeof(char), 1, fp);

          fread(&total,   sizeof(MEM_INT32), 1, fp);
          fread(&memSize, sizeof(MEM_INT32), 1, fp);

          if ( total ==0 )
          {
               return 0;
          }

          MEM_INT *
          pMem =new MEM_INT[memSize +1];

          if ( pMem !=0 )
          {
               PHRS_IDX*
               ux =new PHRS_IDX[total +1];

               if ( ux !=0 )
               {
                    memset(pMem, 0, memSize +1);

                    fread(ux,   sizeof(PHRS_IDX), total,   fp);
                    fread(pMem, sizeof(MEM_INT),  memSize, fp);

                    for ( MEM_INT32 i =0; i <total; i +=2 )
                    {
                          MEM_INT
                          offSz =sizeof(MEM_INT);

                          MEM_INT32
                          mOffset =ux[i].offs;

                          if ( ux[i].offs >memSize )
                          {
                               // Invalid offset value
                               delete [] ux;
                               delete [] pMem;

                               fclose(fp);
                               return -1;
                          }

                          //---------------------------------------------------
                          PHRS_UI
                          ui;

                          ui.sID =*(MEM_INT*)(pMem +ux[i].offs);

                          //---------------------------------------------------
                          wchar_t
                          wsData[1024] ={0};

                          for ( MEM_INT xw =0; xw <ux[i].vLen; xw++ )
                                wsData[xw] =pMem[ux[i].offs +offSz +xw];
                          ui.wsKeys =wsData;

                          //---------------------------------------------------
                          wchar_t
                          wsData2[1024] ={0};
                          for ( MEM_INT xp =0; xp <ux[i +1].vLen; xp++ )
                                wsData2[xp] =pMem[ux[i +1].offs +xp];
                          ui.wsPhrs =wsData2;

                          vdict.push_back(ui);
                    }
                    delete [] ux;
                    delete [] pMem;

                    fclose(fp);
                    return vdict.size();
               }
          }
          fclose(fp);
     }
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::wstring phraseimp::encryptDecrypt(const wchar_t* toEncrypt, const char* mKey)
{
     string
     key =mKey;

     wstring
     msg =toEncrypt;

     if ( !key.size() )
     {
          return msg;
     }

     const wchar_t *xx =msg.c_str();

     #if defined(__ENCODE_MSG)

     wstring
     msgOld =toEncrypt;

     for ( std::wstring::size_type i =0; i <msg.size(); ++i )
     {
           #if defined(__DEBUG_MSG)
           wchar_t
           aa[1024] ={0};

           #if defined(_WIN32)
           swprintf(aa, L"sz=%2d, msg[%2d] =%2x ^%2x = %2d\r\n", key.size(), i, toEncrypt[i], (i %key.size()), toEncrypt[i] ^key[i %key.size()]);
           #else
           swprintf(aa, 1024, L"sz=%2d, msg[%2d] =%2x ^%2x = %2d\r\n", key.size(), i, toEncrypt[i], (i %key.size()), toEncrypt[i] ^key[i %key.size()]);
           #endif

           dw_pintf_ex(aa);

           #endif

           msg[i] ^=key[i %key.size()];

           if ( msg[i] ==0 )
           {
                msg  =L"@x@";
                msg +=msgOld;
                return msg;
           }
     }
     #endif

     return msg;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::wstring phraseimp::encrypt(std::wstring msg, const std::string key)
{
     #if defined(__ENCODE_MSG2)

     // Make sure the key is at least as long as the message
     std::string
     tmp(key);

     while ( key.size() <msg.size() )
     {
             key +=tmp;
     }

     // And now for the encryption part
     for ( std::wstring::size_type i =0; i <msg.size( ); ++i )
     {
           msg[i] ^=key[i];
     }
     #endif

     return msg;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::wstring phraseimp::decrypt(std::wstring msg, const std::string key)
{
     return encrypt(msg, key); // lol
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct PHRS_UI_cmpKey
{
       PHRS_UI               In;

       PHRS_UI_cmpKey(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            return In.wsKeys ==chk.wsKeys;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
void phraseimp::fillData(vector<PHRS_UI> &vfrom, vector<wstring> &vto)
{
     PHRS_ITR
     bi =vfrom.begin();

     for ( ; bi <vfrom.end(); bi++ )
     {
           wstring
           wsContent =bi->wsPhrs;

           wstring
           wsContentEx =decrypt(wsContent, string(ENC_KEY_SZ));

           #if defined(__DEBUG_MSG)

           wchar_t aax[200];
           #if defined(_WIN32)
           swprintf(aax, L"sID =(%d), wsContent =%s\r\n", bi->sID, wsContentEx.c_str());
           swprintf(aax, L"sID =(%d)\r\n", bi->sID);
           #else
           swprintf(aax, 200, L"sID =(%d), wsContent =%s\r\n", bi->sID, wsContentEx.c_str());
           #endif

           dw_pintf_ex(aax);

           #else

           #if defined(_WIN32) && defined(__DEBUG_MSG_DATA)
           TRACE(_T("b4 wsContentEx =%s\r\n"), wsContentEx.c_str());
           #endif

           #endif

           vto.push_back(wsContentEx);
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
void phraseimp::fillDataVc(vector<PHRS_UI> &vfrom, vector<VCandidate> &vto, bool isFull)
{
     PHRS_ITR
     bi =vfrom.begin();

     if ( isFull ==true && vfrom.size() >0 )
     {
          int x =0;
          if ( isFull ==true )
          {
              x =12;
              if ( x ==0 )
              {
                   isFull =false;
              }
          }
     }
     for ( ; bi <vfrom.end(); bi++ )
     {
           wstring
           wsContent =bi->wsPhrs;

           wstring
           wsContentEx =decrypt(wsContent, string(ENC_KEY_SZ));

           #if defined(__DEBUG_MSG)

           wchar_t aax[200];
           #if defined(_WIN32)
           swprintf(aax, L"sID =(%d), wsContent =%s\r\n", bi->sID, wsContentEx.c_str());
           swprintf(aax, L"sID =(%d)\r\n", bi->sID);
           #else
           swprintf(aax, 200, L"sID =(%d), wsContent =%s\r\n", bi->sID, wsContentEx.c_str());
           #endif

           dw_pintf_ex(aax);

           #else

           #if defined(_WIN32) && defined(__DEBUG_MSG_DATA)
           TRACE(_T("b4 wsContentEx =%s\r\n"), wsContentEx.c_str());
           #endif

           #endif

           vto.push_back(VCandidate(wsContentEx, bi->wsKeys, bi->sID, bi->sID, 0, isFull));
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
void phraseimp::fillDataVc2(vector<VCandidate> &vfrom, vector<VCandidate> &vto, bool isFull)
{
     vector<VCandidate>::iterator
     bi =vfrom.begin();

     for ( ; bi <vfrom.end(); bi++ )
     {
           wstring
           wsContent =bi->ws;

           wstring
           wsContentEx =decrypt(wsContent, string(ENC_KEY_SZ));

           #if defined(__DEBUG_MSG)

           wchar_t aax[200];
           #if defined(_WIN32)
           swprintf(aax, L"sID =(%d), wsContent =%s\r\n", bi->sID, wsContentEx.c_str());
           swprintf(aax, L"sID =(%d)\r\n", bi->sID);
           #else
           swprintf(aax, 200, L"sID =(%d), wsContent =%s\r\n", bi->sID, wsContentEx.c_str());
           #endif

           dw_pintf_ex(aax);

           #else

           #if defined(_WIN32) && defined(__DEBUG_MSG_DATA)
           TRACE(_T("b4 wsContentEx =%s\r\n"), wsContentEx.c_str());
           #endif

           #endif

           int index =bi->sID;
           vto.push_back(VCandidate(wsContentEx, bi->wKey, bi->sID, index, 0, isFull, bi->timeID));
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
void phraseimp::fillGroupingData(const wchar_t *wsKeys, vector<PHRS_UI> &vfrom, vector<wstring> &vto, int cmpLen)
{
     PHRS_ITR
     bi =vfrom.begin();

     //------------------------------------------------------------------------
     // full length match first
     //------------------------------------------------------------------------
     for ( ; bi <vfrom.end(); bi++ )
     {
           wstring
           wsContent =bi->wsPhrs;

           #if defined(__DEBUG_MSG)

           wstring
           aa =decrypt(wsContent, string(ENC_KEY_SZ));

           wchar_t aax[200];
           #if defined(_WIN32)
           swprintf(aax, L"(%d)", bi->sID);
           #else
           swprintf(aax, 200, L"(%d)", bi->sID);
           #endif

           dw_pintf_ex(aax);

           aa +=aax;

           vto.push_back(aa);

           #else

           #if defined(__ORDER_BY_FULL_MATCH)
           if ( bi->wsKeys.size() ==cmpLen) //wcslen(wsKeys) )
           #endif
           {
                wstring
                wsKeyEx =decrypt(wsContent, string(ENC_KEY_SZ));

                int
                pos =wsKeyEx.find(L":");

                if ( pos >0 )
                {
                     wsKeyEx.erase(0, pos +1);
                     wsKeyEx.resize(cmpLen);
                }
                vto.push_back(wsKeyEx);
           }

           #endif
     }

     //------------------------------------------------------------------------
     // then match prefix-lenght
     //------------------------------------------------------------------------
     #if defined(__ORDER_BY_FULL_MATCH)

     bi =vfrom.begin();

     for ( ; bi <vfrom.end(); bi++ )
     {
           wstring
           wsContent =bi->wsPhrs;

           #if defined(__DEBUG_MSG)

           wstring
           aa =decrypt(wsContent, string(ENC_KEY_SZ));

           wchar_t aax[200];
           #if defined(_WIN32)
           swprintf(aax, L"(%d)", bi->sID);
           #else
           swprintf(aax, 200, L"(%d)", bi->sID);
           #endif

           dw_pintf_ex(aax);

           aa +=aax;

           v.push_back(aa);

           #else

           if ( bi->wsKeys.size() !=cmpLen) //wcslen(wsKeys) )
           {
                wstring
                wsKeyEx =decrypt(wsContent, string(ENC_KEY_SZ));

                int
                pos =wsKeyEx.find(L":");

                if ( pos >0 )
                {
                     wsKeyEx.erase(0, pos +1);
                     wsKeyEx.resize(cmpLen);
                }
                vto.push_back(wsKeyEx);
           }

           #endif
     }
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
void phraseimp::fillIdiomData(const wchar_t *wsKeys, vector<PHRS_UI> &vfrom, vector<wstring> &vto, int cmpLen)
{
     PHRS_ITR

     bi =vfrom.begin();

     //------------------------------------------------------------------------
     // full length match first
     //------------------------------------------------------------------------
     for ( ; bi <vfrom.end(); bi++ )
     {
           wstring
           wsNextWord;

           if ( getMemData(memNext.bmem(), bi->iNext, wsNextWord) ==0 )
           {
                continue;
           }

           //------------------------------------------------------------------

           #if defined(__DEBUG_MSG_DATA)
           {
                wchar_t aax[200];
                #if defined(_WIN32)
                swprintf(aax, L"all wsNextWord =%s\r\n", wsNextWord.c_str());
                #else
                swprintf(aax, 200, L"all wsNextWord =%s\r\n", wsNextWord.c_str());
                #endif
                dw_pintf_ex(aax);
           }
           #endif

           while ( 1 )
           {
                   int
                   pos =wsNextWord.find(L":");

                   if ( pos <0 )
                   {
                        #if defined(__DEBUG_MSG_DATA)
                        {
                            wchar_t aax[200];
                            #if defined(_WIN32)
                            swprintf(aax, L"add NextWord =%s\r\n", wsNextWord.c_str());
                            #else
                            swprintf(aax, 200, L"Add NextWord =%s\r\n", wsNextWord.c_str());
                            #endif
                            dw_pintf_ex(aax);
                        }
                        #endif

                        vto.push_back(wsNextWord);
                        break;
                   }
                   wstring
                   wsNext =wsNextWord.substr(0, pos);

                   wsNextWord.erase(0, pos +1);

                   #if defined(__DEBUG_MSG_DATA)
                   {
                        wchar_t aax[200];
                        #if defined(_WIN32)
                        swprintf(aax, L"add NextWord =%s\r\n", wsNext.c_str());
                        #else
                        swprintf(aax, 200, L"add NextWord =%s\r\n", wsNext.c_str());
                        #endif
                        dw_pintf_ex(aax);
                   }
                   #endif

                   vto.push_back(wsNext);
           }
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getID(const wchar_t *wsKeys)
{
     setlocale( LC_ALL, "");

     PHRS_UI
     ui(this, 0, wsKeys, (wchar_t *)L"");

     PHRS_ITR
     ui_found =find_if(vdict.begin(), vdict.end(), PHRS_UI_cmpKey(ui));

     if ( ui_found ==vdict.end() )
          return 0;

     return ui_found->sID;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct PHRS_UI_cmpID
{
       PHRS_UI               In;

       PHRS_UI_cmpID(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            return In.sID ==chk.sID;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getData(int sID, wstring &v1, wstring &v2, MEM_INT32 &i3)
{
     setlocale(LC_ALL, "");

     PHRS_UI
     ui(this, sID, (wchar_t *)L"", (wchar_t *)L"");

     PHRS_ITR
     ui_found =find_if(vdict.begin(), vdict.end(), PHRS_UI_cmpID(ui));

     if ( ui_found ==vdict.end() )
          return 0;

     v1 =ui_found->wsKeys;
     v2 =ui_found->wsPhrs;
     i3 =ui_found->iNext;

     return ui_found->sID;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Keys(MEM_INT32 iOffset, wstring &wsOut)
{
     return getMemData(memKeys.bmem(), iOffset, wsOut);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Phrs(MEM_INT32 iOffset, wstring &wsOut)
{
     return getMemData(memPhrs.bmem(), iOffset, wsOut);
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Phrs(V2_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wPhrs.empty() )
     {
          if ( getMemData(memPhrs.bmem(), ui.iPhrs, ui.wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =ui.wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_CPhrs(const V2_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wPhrs.empty() )
     {
          V2_DICT_UI
          ui2(ui.sID, ui.iKeys, ui.iPhrs, ui.iNext);

          if ( getMemData(memPhrs.bmem(), ui2.iPhrs, ui2.wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
          wsOut =ui2.wPhrs;
          return 1;
     }

     wsOut =ui.wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Phrs(V2_DICT_UI_ITR itr, wstring &wsOut)
{
     if ( itr->wPhrs.empty() )
     {
          if ( getMemData(memPhrs.bmem(), itr->iPhrs, itr->wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =itr->wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Keys(V2_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          if ( getMemData(memKeys.bmem(), ui.iKeys, ui.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_CKeys(const V2_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          V2_DICT_UI
          ui2(ui.sID, ui.iKeys, ui.iPhrs, ui.iNext);

          if ( getMemData(memKeys.bmem(), ui2.iKeys, ui2.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
          wsOut =ui2.wKeys;
          return 1;
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Keys(V2_DICT_UI_ITR itr, wstring &wsOut)
{
     if ( itr->wKeys.empty() )
     {
          if ( getMemData(memKeys.bmem(), itr->iKeys, itr->wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =itr->wKeys;
     return 1;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Phrs(V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wPhrs.empty() )
     {
          if ( getMemData(memPhrs.bmem(), ui.iPhrs, ui.wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =ui.wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_CPhrs(const V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wPhrs.empty() )
     {
          V2W_DICT_UI
          ui2(ui.sID, ui.iKeys, ui.iPhrs, ui.iQKey, ui.iHKey, ui.iHVoc);

          if ( getMemData(memPhrs.bmem(), ui2.iPhrs, ui2.wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
          wsOut =ui2.wPhrs;
          return 1;
     }

     wsOut =ui.wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Phrs(V2W_DICT_UI_ITR itr, wstring &wsOut)
{
     if ( itr->wPhrs.empty() )
     {
          if ( getMemData(memPhrs.bmem(), itr->iPhrs, itr->wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =itr->wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Keys(V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          if ( getMemData(memKeys.bmem(), ui.iKeys, ui.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_CKeys(const V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          V2W_DICT_UI
          ui2(ui.sID, ui.iKeys, ui.iPhrs, ui.iQKey, ui.iHKey, ui.iHVoc);

          if ( getMemData(memKeys.bmem(), ui2.iKeys, ui2.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
          wsOut =ui2.wKeys;
          return 1;
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Keys(V2W_DICT_UI_ITR itr, wstring &wsOut)
{
     if ( itr->wKeys.empty() )
     {
          if ( getMemData(memKeys.bmem(), itr->iKeys, itr->wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =itr->wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_QKey(V2W_DICT_UI_ITR itr, wstring &wsOut)
{
     if ( itr->wKeys.empty() )
     {
          if ( getMemData(memPhrs.bmem(), itr->iQKey, itr->wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =itr->wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_QKey(V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          if ( getMemData(memKeys.bmem(), ui.iQKey, ui.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_CQKey(const V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          V2W_DICT_UI
          ui2(ui.sID, ui.iKeys, ui.iPhrs, ui.iQKey, ui.iHKey, ui.iHVoc);

          if ( getMemData(memKeys.bmem(), ui2.iQKey, ui2.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
          wsOut =ui2.wKeys;
          return 1;
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_HVoc(V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wPhrs.empty() )
     {
          if ( getMemData(memPhrs.bmem(), ui.iHVoc, ui.wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =ui.wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_CHVoc(const V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wPhrs.empty() )
     {
          V2W_DICT_UI
          ui2(ui.sID, ui.iKeys, ui.iPhrs, ui.iQKey, ui.iHKey, ui.iHVoc);

          if ( getMemData(memPhrs.bmem(), ui2.iHVoc, ui2.wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
          wsOut =ui2.wPhrs;
          return 1;
     }

     wsOut =ui.wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_HVoc(V2W_DICT_UI_ITR itr, wstring &wsOut)
{
     if ( itr->wPhrs.empty() )
     {
          if ( getMemData(memPhrs.bmem(), itr->iHVoc, itr->wPhrs) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =itr->wPhrs;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_HKey(V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          if ( getMemData(memKeys.bmem(), ui.iHKey, ui.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_CHKey(const V2W_DICT_UI &ui, wstring &wsOut)
{
     if ( ui.wKeys.empty() )
     {
          V2W_DICT_UI
          ui2(ui.sID, ui.iKeys, ui.iPhrs, ui.iQKey, ui.iHKey, ui.iHVoc);

          if ( getMemData(memKeys.bmem(), ui2.iHKey, ui2.wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
          wsOut =ui2.wKeys;
          return 1;
     }

     wsOut =ui.wKeys;
     return 1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_HKey(V2W_DICT_UI_ITR itr, wstring &wsOut)
{
     if ( itr->wKeys.empty() )
     {
          if ( getMemData(memKeys.bmem(), itr->iHKey, itr->wKeys) ==0 )
          {
               wsOut =L"";
               return 0;
          }
     }

     wsOut =itr->wKeys;
     return 1;
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_Next(MEM_INT32 iOffset, wstring &wsOut)
{
     return getMemData(memNext.bmem(), iOffset, wsOut);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp::getMemData_HVoc(MEM_INT32 iOffset, wstring &wsOut)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_nextWord
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            wstring
            wsKey1 =a.wsKeys,
            wsKey2 =b.wsKeys;

            int
            cmp =wcsncmp(wsKey1.c_str(), wsKey2.c_str(), 1);

            #if defined(__FOR_ANDROID)
            if ( cmp <0 )
            {
                 return 1;
            }
            else if ( cmp >0 )
            {
                 return 1;
            }
            return 0;

            #else
            return cmp ==0;

            #endif
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp::qsortAndUnique()
{
     std::sort( vnext.begin(), vnext.end(), target_nextWord());

     qsortPos();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp::qsort()
{
     std::sort(vdict.begin(), vdict.end());

     //qsortPos();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp::appendDirectly(int sID, const wstring wsKeys, const wstring wsPhrs, const MEM_INT32 iNext)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_UI
     ui(this, sID, wsKeys.c_str(), wsContent.c_str(), iNext);

     #if defined(__DEBUG_MSG2)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"add (sID =%ld, %ls, %ls)\r\n\r\n",  sID, ui.wsKeys.c_str(), ui.wsPhrs.c_str());
          #else
          swprintf(aa, 1024, L"add (sID =%ld, %ls, %ls)\r\n\r\n",  sID, ui.wsKeys.c_str(), ui.wsPhrs.c_str());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     if ( iNext !=INVALID_MEM_OFFSET )
     {
          wstring
          wsPhraseEx =wsPhrs;

          int
          pos =wsPhraseEx.find(L":");

          if ( pos >0 )
          {
               wsPhraseEx =wsPhraseEx.substr(0, pos);
          }

          IDIOM_UI
          ui2(wsPhraseEx.c_str(), iNext);

          vnext.push_back(ui2);
     }
     vdict.push_back(ui);

     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp::append(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 iNext)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_UI
     ui2(this, sID, wsKeys, wsContent.c_str(), iNext);

     #if defined(__DEBUG_MSG2)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"add (sID =%d, %ls, %ls\r\n", sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #else
          swprintf(aa, 1024, L"add (sID =%d, %ls, %ls\r\n", sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     if ( std::find(vdict.begin(), vdict.end(), ui2) ==vdict.end() )
     {
          vdict.push_back(ui2);
          qsort();

          return true;
     }
     return false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp::remove(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_UI
     ui2(this, sID, wsKeys, wsContent.c_str());

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"remove (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #else
          swprintf(aa, 1024, L"remove (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #endif

          dw_pintf_ex(aa);
          KIKA_LOGD("Kika-ENGINE, phraseimp::remove, %ls\r\n", aa);
     }
     #endif

     #if defined(__DEBUG_MSG_EX)
     PHRS_ITR
     bi =vdict.begin();

     while ( bi !=vdict.end() )
     {
             KIKA_LOGD("Kika-ENGINE, phraseimp::get k=%ls, c=%ls", bi->wsKeys.c_str(), bi->wsPhrs.c_str());
             bi++;
     }
     #endif

     PHRS_ITR
     ui2_found =std::find(vdict.begin(), vdict.end(), ui2);

     if ( ui2_found != vdict.end() )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("Kika-ENGINE, phraseimp::remove, OK!");
          #endif

          vdict.erase(ui2_found);
          qsort();

          return true;
     }
     #if defined(__DEBUG_MSG)
     KIKA_LOGD("Kika-ENGINE, phraseimp::remove, FAIL!");
     #endif

     return false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp::isExist(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_UI
     ui2(this, sID, wsKeys, wsContent.c_str());

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"isExist (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #else
          swprintf(aa, 1024, L"isExist (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_ITR
     ui2_found =std::find(vdict.begin(), vdict.end(), ui2);

     if ( ui2_found != vdict.end() )
     {
          return true;
     }
     return false;
}
