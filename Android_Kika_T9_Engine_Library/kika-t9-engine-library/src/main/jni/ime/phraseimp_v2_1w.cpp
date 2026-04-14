// phraseimp_v2_1w_1.cpp: implementation of the phraseimp_v2_1w_1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "string.h"
#include "phraseimp_v2_1w.h"

//#define __CREATE_LOG
//#define __CREATE_LOG_1
//#define __CREATE_LOG_2

#if defined(_DEBUG_LOG)
 #include "WfxFile.h"
#endif

#if defined(__CREATE_LOG)
 #include "WfxFile.h"
#endif

#include "debug_util.h"

#define __ENCODE_MSG
#define __APPLY_UNIQUE_RULE
// #define __DEBUG_MSG_MEM
//#define __FULL_MATCH_HEAD_KEY

//#define __HEAD_QUICK_SEARCH_DEBUG
//#define __DEBUG_MSG_RUN_MS

#if defined(__DEBUG_MSG_RUN_MS) || defined(__DEBUG_MSG_LOADDICT_MS) || defined(__DEBUG_MSG_MEM)
 #include <iostream>
 #if !defined(__TEST) && !defined(_WIN32)
  #if !defined(KIKA_LOGD)
   #include <android/log.h>
   #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)
  #endif
 #endif
#endif

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
phraseimp_v2_1w::phraseimp_v2_1w()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
phraseimp_v2_1w::~phraseimp_v2_1w()
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
void phraseimp_v2_1w::clear()
{
     #if defined(__DEBUG_MSG_MEM)
     KIKA_LOGD("Kika-ENGINE, phraseimp_v2_1w::clear()");
     #endif

     v2dict.clear();
     v2dictW.clear();
     v2dictH.clear();
     v2dgrp_dynload.clear();
     v2dgrp_preload.clear();

     #if defined(__LINUX_BASE)
     v2dict.shrink_to_fit();
     v2dictH.shrink_to_fit();
     v2dictW.shrink_to_fit();
     v2dgrp_dynload.shrink_to_fit();
     v2dgrp_preload.shrink_to_fit();
     #endif

     phraseimp::clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1_target_equal
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            int
            yes =a.compare(b);

            if ( yes ==0 )
            {
                 return true;
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_keyMatchFuzzy
{
       PHRS_UI               In;
       int                   cmpMatchLen;
       phraseimp*            pIme;
       CMP_FUNC_FLAG         flag;

       v2_1w_PHRS_UI_keyMatchFuzzy(phraseimp* _pIme, CMP_FUNC_FLAG _flag, PHRS_UI _input, int _cmpLen) :
       pIme(_pIme), flag(_flag), In(_input), cmpMatchLen(_cmpLen)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            switch ( flag )
            {
                     case cmp_for_T9:
                          if ( pIme->getMemData_CKeys(chk, wsKeys) ==0 )
                          {
                               return false;
                          }
                          break;
                     case cmp_for_Qwerty:
                     case cmp_for_FullMatchQwerty:
                          if ( pIme->getMemData_CQKey(chk, wsKeys) ==0 )
                          {
                               return false;
                          }
                          break;
                     case cmp_for_HeadMapping:
                          if ( pIme->getMemData_CHKey(chk, wsKeys) ==0 )
                          {
                               return false;
                          }
                          break;
            }

            wstring
            wsPhrs;
            if ( pIme->getMemData_CPhrs(chk, wsPhrs) ==0 )
            {
                 return false;
            }

            if ( In.wsKeys ==wsKeys )
            {
                 switch ( flag )
                 {
                     case cmp_for_Qwerty:
                     case cmp_for_FullMatchQwerty:
                     case cmp_for_HeadMapping:
                      {
                          //只要Key對了就ok
                          return true;
                      }
                     case cmp_for_T9:
                     default:
                          if ( wsPhrs.size() >1 )
                          {
                               if ( wsPhrs.compare(2, cmpMatchLen, In.wsPhrs) ==0 )
                               {
                                    return true;
                               }
                          }
                          else
                          {
                               if ( wsPhrs.compare(0, cmpMatchLen, In.wsPhrs) ==0 )
                               {
                                    return true;
                               }
                          }
                 }
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_keyMatch
{
       PHRS_UI               In;

       v2_1w_PHRS_UI_keyMatch(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_CKeys(chk, wsKeys) ==0 )
            {
                 return false;
            }
            return In.wsKeys ==wsKeys;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp_v2_1w::loadDict(const char* dictName)
{
     clear();

     FILE
     *fp =fopen(dictName, "rb");

     if ( fp )
     {
          char
          headSz[sizeof(HEAD_ID_DICT_V2_1W) +1] ={0};

          fread(headSz, sizeof(HEAD_ID_DICT_V2_1W) *sizeof(char), 1, fp);

          MEM_INT32
          vSize =0;

          fread(&vSize, sizeof(MEM_INT32), 1, fp);

          #if defined(__DEBUG_MSG)
          {
               wchar_t
               aa[1024] ={0};

               #if defined(_WIN32)
               swprintf(aa, L"vSize =%d\r\n", vSize);
               #else
               swprintf(aa, 1024, L"vSize =%d\r\n", vSize);
               #endif

               dw_pintf_ex(aa);
          }
          #endif

          MEM_INT32
          sizePhrs =0,
          sizeKeys =0,
          sizeNext =0;

          fread(&sizePhrs, sizeof(MEM_INT32), 1, fp);
          fread(&sizeKeys, sizeof(MEM_INT32), 1, fp);
          fread(&sizeNext, sizeof(MEM_INT32), 1, fp);

          bool
          mem1 =memPhrs.alloc(sizePhrs),
          mem2 =memKeys.alloc(sizeKeys),
          mem3 =memNext.alloc(sizeNext);

          if ( mem1 ==false || mem2 ==false || mem3 ==false )
          {
               return false;
          }

          //-------------------------------------------------------------------
          // 2018/04/20
          //-------------------------------------------------------------------
          fread(memPhrs.bmem(), sizePhrs, 1, fp);
          fread(memKeys.bmem(), sizeKeys, 1, fp);
          fread(memNext.bmem(), sizeNext, 1, fp);

          if ( sizePhrs >MAX_ALLOC_MEM || sizeKeys >MAX_ALLOC_MEM || sizeNext >MAX_ALLOC_MEM )
          {
               // alloc too big, impossible
               return false;
          }

          v2dictH.clear();
          v2dictW.clear();

          #if defined(__LINUX_BASE)
          v2dictH.shrink_to_fit();
          v2dictW.shrink_to_fit();
          #endif

          unsigned int idx;
          for ( idx =0; idx <vSize; idx++ )
          {
                V2W_DICT_UI_LOAD
                u2Load;

                fread(&u2Load, sizeof(V2W_DICT_UI_LOAD), 1, fp);

                V2W_DICT_UI
                u2(u2Load);

                v2dict.push_back(u2);

                //-------------------------------------------------------------
                // Read (Char) Qwerty Keys
                //-------------------------------------------------------------
                if ( u2.iQKey !=INVALID_MEM_OFFSET )
                {
                     if ( getMemData_QKey(u2, u2.wKeys) ==0 )
                     {
                          continue;
                     }
                     v2dictW.push_back(u2);
                }

                //-------------------------------------------------------------
                // Read (Voca) HeadMapping Keys
                //-------------------------------------------------------------
                if ( u2.iHKey !=INVALID_MEM_OFFSET )
                {
                     if ( getMemData_HKey(u2, u2.wKeys) ==0 )
                     {
                          continue;
                     }
                     wstring
                     wsPhrs;

                     if ( getMemData_HVoc(u2, wsPhrs) ==0 )
                     {
                          continue;
                     }
                     v2dictH.push_back(u2);
                }

                //-------------------------------------------------------------
                // Read Next Word
                //-------------------------------------------------------------
                if ( u2.iNext !=INVALID_MEM_OFFSET )
                {
                     wstring
                     wsPhrs;

                     if ( getMemData_Phrs(u2.iPhrs, wsPhrs) ==0 )
                     {
                          //最差狀況是沒有聯想詞!
                          continue;
                     }

                     wsPhrs.resize(1);

                     IDIOM_UI
                     ui2(wsPhrs.c_str(), u2.iNext);

                     vnext.push_back(ui2);
                }
          }

          //-------------------------------------------------------------------
          // 2018/04/12, group data
          //-------------------------------------------------------------------
          vSize =0;
          fread(&vSize, sizeof(MEM_INT32), 1, fp);

          for ( idx =0; idx <vSize; idx++ )
          {
                MEM_INT32
                pos;

                fread(&pos, sizeof(MEM_INT32), 1, fp);

                V2W_DICT_UI_ITR
                ii =v2dict.begin() +pos;

                PHRS_V2W_POS
                ex;
                ex.itr =ii;

                #if defined(__FOR_NEW_SET)
                v2dgrp_dynload.insert(ex);
                #else
                v2dgrp_dynload.push_back(ex);
                #endif

                //-------------------------------------------------------------
                wstring
                wsKeys;
                getMemData_Keys(ii, wsKeys);

                wstring
                wsPhrs;
                getMemData_Phrs(ii, wsPhrs);

                PHRS_UI
                uig;
                uig.sID    =idx;
                uig.wsKeys =wsKeys;
                uig.wsPhrs =wsPhrs;
                uig.pIme   =this;

                #if defined(__FOR_NEW_SET)
                v2dgrp_preload.insert(uig);
                #else
                v2dgrp_preload.push_back(uig);
                #endif
          }

          #if defined(__CREATE_LOG)
          {
           CString
          fName =WfxCurrPath(_T("\\Tab\\Grouping.txt"));

          CWfxFileW
          fileX;
          fileX.SaveHeader(fName);

          set<PHRS_V2W_POS>::iterator
          ii =v2dgrp_dynload.begin();

          for ( ; ii !=v2dgrp_dynload.end(); ii++ )
          {
                wstring
                wsKeys;
                if ( getMemData_Keys(ii->itr, wsKeys) ==0 )
                {
                     break;
                }
                // preLoad
                fileX.AppendLn(wsKeys.c_str());
          }
          fileX.CloseFile();
          }
          #endif

          //-------------------------------------------------------------------
          // 2018/04/12
          //-------------------------------------------------------------------
          fclose(fp);

          //-------------------------------------------------------------------
          //-------------------------------------------------------------------
          qsortAndUnique();

          return true;
     }
     else
     {
          #if defined(__DEBUG_MSG)
          {
               wchar_t
               aa[1024] ={0};

               #if defined(_WIN32)
               swprintf(aa, L"dictName =%ls, load fail\r\n", dictName);
               #else
               swprintf(aa, 1024, L"dictName =%ls, load fail\r\n", dictName);
               #endif

               dw_pintf(aa);
          }
          #endif
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
struct v2_1w_EN_target_Same_Key_ID_Strokes
{
       phraseimp*            pIme;

       v2_1w_EN_target_Same_Key_ID_Strokes(phraseimp_v2_1w* pime) : pIme(pime)
       {
       }

       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            wstring
            wsKeys1;
            if ( pIme->getMemData_Keys(a.itr, wsKeys1) ==0 )
            {
                 return false;
            }
            wstring
            wsKeys2;
            if ( pIme->getMemData_Keys(b.itr, wsKeys2) ==0 )
            {
                 return false;
            }

            #if defined(_WIN32) && defined(__DEBUG_MSG)
            //TRACE(_T("%ls,%ls\r\n"), wsPhrase1.c_str(),wsPhrase2.c_str());
            #endif

            int
            cmp =wcscmp(wsKeys1.c_str(), wsKeys2.c_str());

            if ( cmp <0 )
            {
                 return true;
            }
            else if ( cmp ==0 )
            {
                 if ( a.itr->sID <b.itr->sID )
                 {
                      return true;
                 }
                 else if ( a.itr->sID >b.itr->sID )
                 {
                      return false;
                 }

                 wstring
                 wsStrokes1;
                 if ( pIme->getMemData_Keys(a.itr, wsStrokes1) ==0 )
                 {
                      return false;
                 }
                 wstring
                 wsStrokes2;
                 if ( pIme->getMemData_Keys(b.itr, wsStrokes2) ==0 )
                 {
                      return false;
                 }

                 wsStrokes1.erase(0, 2);
                 wsStrokes2.erase(0, 2);

                 cmp =wcscmp(wsStrokes1.c_str(), wsStrokes2.c_str());

                 if ( cmp <0 )
                 {
                      return true;
                 }
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct v2_1w_CH_target_Same_Key_Strokes_ID
{
       phraseimp*            pIme;

       v2_1w_CH_target_Same_Key_Strokes_ID(phraseimp* pime) : pIme(pime)
       {
       }

       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            #if defined(_WIN32) && defined(__DEBUG_MSG)
            //TRACE(_T("%ls,%ls\r\n"), wsPhrase1.c_str(),wsPhrase2.c_str());
            #endif

            wstring
            wsKeys1;
            if ( pIme->getMemData_Keys(a.itr, wsKeys1) ==0 )
            {
                 return false;
            }
            wstring
            wsKeys2;
            if ( pIme->getMemData_Keys(b.itr, wsKeys2) ==0 )
            {
                 return false;
            }

            int
            cmp =wcscmp(wsKeys1.c_str(), wsKeys2.c_str());

            if ( cmp <0 )
            {
                 return true;
            }
            else if ( cmp ==0 )
            {
                 wstring
                 wsStrokes1;
                 if ( pIme->getMemData_Phrs(a.itr, wsStrokes1) ==0 )
                 {
                      return false;
                 }
                 wstring
                 wsStrokes2;
                 if ( pIme->getMemData_Phrs(b.itr, wsStrokes2) ==0 )
                 {
                      return false;
                 }

                 wsStrokes1.erase(0, 2);
                 wsStrokes2.erase(0, 2);

                 cmp =wcscmp(wsStrokes1.c_str(), wsStrokes2.c_str());

                 if ( cmp <0 )
                 {
                      return true;
                 }
                 else if ( cmp ==0 )
                 {
                      if ( a.itr->sID <b.itr->sID )
                      {
                           return true;
                      }
                 }
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct v2_1w_target_Same_Key_ID_Strokes
{
       phraseimp*            pIme;

       v2_1w_target_Same_Key_ID_Strokes(phraseimp *_pIme) : pIme(_pIme)
       {
       }

       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            const wchar_t *s1 =a.wKeys.c_str();
            const wchar_t *s2 =b.wKeys.c_str();

            if ( a.sID <b.sID )
            {
                 return true;
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct v2_1w_target_Same_Key_ID_By_MaxLen
{
       phraseimp*            pIme;

       v2_1w_target_Same_Key_ID_By_MaxLen(phraseimp *_pIme) : pIme(_pIme)
       {
       }

       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            if ( a.sID <b.sID )
            {
                 return true;
            }
            //if ( a.wPhrs.size() <b.wPhrs.size() )
            //{
            //     return true;
            //}
            ///else if ( a.wPhrs.size() ==b.wPhrs.size() )
            ///{
            ///     if ( a.sID <b.sID )
            ///     {
            ///          return true;
            ///     }
            ///}
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp_v2_1w::qsortPos()
{
     v2dict_max =v2dict.size();

     if ( preLoad !=0 )
     {
          V2W_DICT_UI_ITR
          ii =v2dict.begin();

          v2dict_max =0;

          for ( ; ii !=v2dict.end(); ii++ )
          {
                // preLoad
                if ( ii->iKeys ==INVALID_MEM_OFFSET )
                {
                     // 後面都是詞
                     break;
                }

                v2dict_max++;

                wstring
                wsKeys;
                if ( getMemData_Keys(ii, wsKeys) ==0 )
                {
                     break;
                }
          }

          //重新用sID來排
          //std::sort( v2dictW.begin(), v2dictW.end(), v2_1w_target_Same_Key_ID_Strokes(this));
          //std::sort( v2dictH.begin(), v2dictH.end(), v2_1w_target_Same_Key_ID_By_MaxLen(this));

          #if defined(__CREATE_LOG_2)
          {
              CString
              fName =WfxCurrPath(_T("\\Tab\\Qwerty.txt"));

              CWfxFileW
              fileX;
              fileX.SaveHeader(fName);

              vector<V2W_DICT_UI>::iterator
              ii =v2dictW.begin();

              for ( ; ii !=v2dictW.end(); ii++ )
              {
                    // preLoad
                    fileX.AppendLn(ii->wKeys.c_str());
              }
              fileX.CloseFile();
          }
          #endif
     }
}

#if defined(__TRY_NEW_2_1)
//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp_v2_1w::clear_u()
{
     udict.clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp_v2_1w::append_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 orderID)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_V2_W_POS
     ui2(wsKeys, wsContent.c_str());

     #if defined(__DEBUG_MSG2)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"append_u (sID =%d, %ls, %ls\r\n", sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #else
          swprintf(aa, 1024, L"append_u (sID =%d, %ls, %ls\r\n", sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_V2_W_POS_ITR_USER
     ui2_found =std::find(udict.begin(), udict.end(), ui2);

     if ( ui2_found ==udict.end() )
     {
          //V2W_DICT_UI
          //u2(sID, wsKeys, wsContent.c_str());

          //v2dict.push_back(u2);

          //cannot use append(sID, wsKeys, wsContent.c_str());

          PHRS_V2_W_POS
          ex;
          ex.itr       =v2dict.end() -1;
          ex.wsKeys    =ex.itr->wKeys;
          ex.wsStrokes =ex.itr->wPhrs;

          udict.insert(ex);
          //qsort();

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
bool phraseimp_v2_1w::remove_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_V2_W_POS
     ui2(wsKeys, wsContent.c_str());

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"remove_u (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #else
          swprintf(aa, 1024, L"remove_u (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_V2_W_POS_ITR_USER
     ui2_found =std::find(udict.begin(), udict.end(), ui2);

     if ( ui2_found !=udict.end() )
     {
          v2dict.erase(ui2_found->itr);
          udict.erase(ui2_found);
          //qsort();

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
bool phraseimp_v2_1w::isExist_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_V2_W_POS
     ui2(wsKeys, wsContent.c_str());

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"isExist_u (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #else
          swprintf(aa, 1024, L"isExist_u (sID =%d, %ls, %ls)\r\n",  sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_V2_W_POS_ITR_USER
     ui2_found =std::find(udict.begin(), udict.end(), ui2);

     if ( ui2_found !=udict.end() )
     {
          return true;
     }
     return false;
}
#endif

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut)
{
     if ( iOffset ==INVALID_MEM_OFFSET )
     {
          return -1;
     }

     LPCMEM_CHR
     pMem =pMemData +(iOffset -1);

     wstring
     wsGet =UTF8_to_wchar(pMem);

     if ( strlen(pMem) >3 && pMem[0] =='@' && pMem[1] =='x' && pMem[2] =='@' )
     {
          wsOut =wsGet;
          wsOut.erase(0, 3);
          return wsOut.size();
     }
     else
     {
          wsOut =encryptDecrypt(wsGet.c_str(), ENC_KEY_SZ);
          return wsOut.size();
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"v2dict.size() =%d\r\n", v2dict.size());
          #else
          swprintf(aa, 1024, L"v2dict.size() =%d\r\n", v2dict.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_UI
     ui(this, sID, wsKeys, (wchar_t *)L"");

     V2W_DICT_UI_ITR
     bi =v2dict.begin();

     #if defined(_WIN32) && defined(__DEBUG_MSG)
     //TRACE(_T("vdict2 size =%d\r\n"), v2dict.size());
     #endif

     while ( v.size() <(unsigned long)total )
     {
             V2W_DICT_UI_ITR
             ui_found =find_if(bi, v2dict.end(), v2_1w_PHRS_UI_keyMatch(ui));

             if ( ui_found ==v2dict.end() )
             {
                  break;
             }
             wstring
             wsPhrs;

             if ( getMemData(memNext.bmem(), ui_found->iPhrs, wsPhrs) ==0 )
             {
                  continue;
             }

             #if defined(_WIN32) && defined(__DEBUG_MSG)
             //TRACE(_T("wsContent =%s\r\n"), wsPhrs.c_str());
             #endif

             v.push_back(decrypt(wsPhrs, string(ENC_KEY_SZ)));

             bi =ui_found +1;
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_idiomKeyMatch
{
       PHRS_UI               In;

       v2_1w_PHRS_UI_idiomKeyMatch(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_CKeys(chk, wsKeys) ==0 )
            {
                 return false;
            }

            int
            cmp =In.wsKeys ==wsKeys;

            if ( cmp <0 )
            {
                 return false;
            }

            wstring
            wsPhrs;
            if ( In.pIme->getMemData_CPhrs(chk, wsPhrs) ==0 )
            {
                 return false;
            }

            return In.wsPhrs ==wsPhrs;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_IDIOM_UI_idiomKeyMatch
{
       IDIOM_UI              In;

       v2_1w_IDIOM_UI_idiomKeyMatch(IDIOM_UI input) : In(input)
       {
       }

       bool operator()(const IDIOM_UI& chk) const
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
int  phraseimp_v2_1w::getIdiom(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     setlocale( LC_ALL, "");

     if ( vnext.size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     IDIOM_UI
     ui(wsKeyIn, INVALID_MEM_OFFSET);

     IDIOM_ITR
     bi =vnext.begin();

     vector<PHRS_UI>
     vdictGet;

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"vnext total =%d\r\n", vnext.size());
          #else
          swprintf(aa, 1024, L"vnext total =%d\r\n", vnext.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     bool
     found =false;

     //------------------------------------------------------------------------
     //------------------------------------------------------------------------
     unsigned long
     getRangeItem =10;

     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     while ( vdictGet.size() <getRangeItem )
     {
             IDIOM_ITR
             ui_found =find_if(bi, vnext.end(), v2_1w_IDIOM_UI_idiomKeyMatch(ui));

             if ( ui_found ==vnext.end() )
             {
                  break;
             }
             PHRS_UI
             ui2(this, sID, wsKeyIn, L"", ui_found->iNext);

             bool
             match =true;

             if ( match ==true )
             {
                  vdictGet.push_back(ui2);

                  #if defined(__DEBUG_MSG)
                  {
                       wchar_t
                       aa[1024] ={0};

                       #if defined(_WIN32)
                       swprintf(aa, L"1, %ls\r\n", ui2.wsKeys.c_str());
                       #else
                       swprintf(aa, 1024, L"1, %ls\r\n", ui2.wsKeys.c_str());
                       #endif

                       dw_pintf_ex(aa);
                  }
                  #endif

                  found =true;

                  break;
             }

             bi =ui_found +1;
     }
     if ( found ==false )
          return 0;

     fillIdiomData(wsKeyIn, vdictGet, v, wcslen(wsKeyIn));

     //------------------------------------------------------------------------
     if ( v.size() >(unsigned long)total )
     {
          v.resize(total);
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_keyMatchGrouping_preload
{
       int                   cmpLen;
       PHRS_UI               In;

       v2_1w_PHRS_UI_keyMatchGrouping_preload(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            #if defined(_WIN32) && defined(__DEBUG_MSG_DATA)
            //TRACE(_T("In =%s, Chk =%s\r\n"), In.wsKeys.c_str(), chk.itr->wsKeys.c_str());
            #endif

            int
            cmp =wcsncmp(In.wsKeys.c_str(), chk.wsKeys.c_str(), cmpLen);

            return cmp ==0;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getGrouping_preload(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     if ( v2dgrp_preload.size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     PHRS_GRP_ITR
     bi =v2dgrp_preload.begin();

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"v2dict total =%d\r\n", v2dgrp_preload.size());
          #else
          swprintf(aa, 1024, L"v2dict total =%d\r\n", v2dgrp_preload.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);

     PHRS_UI
     ui(this, sID, wsKeyIn, L"");

     PHRS_GRP_ITR
     ui_found =find_if(v2dgrp_preload.begin(), v2dgrp_preload.end(), v2_1w_PHRS_UI_keyMatchGrouping_preload(ui, cmpLen));

     vector<PHRS_UI>
     vdictGet;

     while ( ui_found !=v2dgrp_preload.end() )
     {
             PHRS_GRP_ITR
             bi1 =ui_found;

             #if defined(_WIN32) && defined(__DEBUG_MSG)
             //TRACE(_T("%ls, %ls, %ls\r\n"), ui.wsKeys.c_str(), wsKeys.c_str(), wsPhrs.c_str());
             #endif

             if ( ui.wsKeys.compare(0, cmpLen, bi1->wsKeys) )
             {
                  break;
             }

             PHRS_UI
             ui2(this, bi1->sID, bi1->wsKeys.c_str(), bi1->wsPhrs.c_str(), INVALID_MEM_OFFSET);

             vdictGet.push_back(ui2);

             ui_found++;
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"Get Match Total =%d\r\n", vdictGet.size());
          #else
          swprintf(aa, 1024, L"Get Match Total =%d\r\n", vdictGet.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     fillGroupingData(wsKeyIn, vdictGet, v, cmpLen);

     //------------------------------------------------------------------------
     if ( v.size() >(unsigned long)total )
     {
          v.resize(total);
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_keyMatchGrouping_dynamic
{
       int                   cmpLen;
       PHRS_UI               In;

       v2_1w_PHRS_UI_keyMatchGrouping_dynamic(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }

       bool operator()(const PHRS_V2W_POS& chk) const
       {
            #if defined(_WIN32) && defined(__DEBUG_MSG_DATA)
            //TRACE(_T("In =%s, Chk =%s\r\n"), In.wsKeys.c_str(), chk.itr->wsKeys.c_str());
            #endif

            wstring
            wsKeys;
            if ( In.pIme->getMemData_Keys(chk.itr, wsKeys) ==0 )
            {
                 return false;
            }

            int
            #if !defined(__PARTIAL_GET)
            cmp =wcscmp(In.wsKeys.c_str(), wsKeys.c_str());
            #else
            cmp =wcsncmp(In.wsKeys.c_str(), wsKeys.c_str(), cmpLen);
            #endif

            return cmp ==0;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getGrouping_dynamic(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     if ( v2dict.size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     V2W_DICT_UI_ITR
     bi =v2dict.begin();

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"v2dict total =%d\r\n", v2dict.size());
          #else
          swprintf(aa, 1024, L"v2dict total =%d\r\n", v2dict.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);

     PHRS_UI
     ui(this, sID, wsKeyIn, L"");

     PHRS_V2W_POS_ITR
     ui_found =find_if(v2dgrp_dynload.begin(), v2dgrp_dynload.end(), v2_1w_PHRS_UI_keyMatchGrouping_dynamic(ui, cmpLen));

     vector<PHRS_UI>
     vdictGet;

     PHRS_V2W_POS_ITR
     bi1 =ui_found;

     while ( bi1 !=v2dgrp_dynload.end() )
     {
             wstring
             wsKeys;
             if ( getMemData_Keys(bi1->itr, wsKeys) ==0 )
             {
                  break;
             }
             wstring
             wsPhrs;
             if ( getMemData_Phrs(bi1->itr, wsPhrs) ==0 )
             {
                  break;
             }

             PHRS_UI
             ui2(this, bi1->itr->sID, wsKeys.c_str(), wsPhrs.c_str(), bi1->itr->iNext);

             #if defined(_WIN32) && defined(__DEBUG_MSG)
             //TRACE(_T("ui wsK =%ls, keyin wsK =%ls\r\n"), ui2.wsKeys.c_str(), wsKeys.c_str());
             #endif

             if ( ui.wsKeys.compare(0, cmpLen, wsKeys) )
             {
                  break;
             }

             vdictGet.push_back(ui2);

             bi1++;
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"Get Match Total =%d\r\n", vdictGet.size());
          #else
          swprintf(aa, 1024, L"Get Match Total =%d\r\n", vdictGet.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     fillGroupingData(wsKeyIn, vdictGet, v, cmpLen);

     //------------------------------------------------------------------------
     if ( v.size() >(unsigned long)total )
     {
          v.resize(total);
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getGrouping(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     return getGrouping_dynamic(sID, wsKeyIn, v, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::addGrouping(int sID, const wchar_t *wsKeys)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_CmpKeyLen
{
       PHRS_UI               In;
       phraseimp*            pIme;

       v2_1w_PHRS_UI_CmpKeyLen(phraseimp* _pIme, PHRS_UI input) :
       pIme(_pIme), In(input)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( pIme->getMemData_CKeys(chk, wsKeys) ==0 )
            {
                 return false;
            }

            int
            pos =wsKeys.find(In.wsKeys);

            bool
            match =(pos ==0) && (pos != std::string::npos);

            return match;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_CmpKeyLenFuzzy
{
       PHRS_UI               In;
       phraseimp*            pIme;
       CMP_FUNC_FLAG         flag;

       v2_1w_PHRS_UI_CmpKeyLenFuzzy(phraseimp* _pIme, CMP_FUNC_FLAG _flag, PHRS_UI input) :
       pIme(_pIme), flag(_flag), In(input)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            switch ( flag )
            {
                     case cmp_for_T9:
                          if ( pIme->getMemData_CKeys(chk, wsKeys) ==0 )
                          {
                               return false;
                          }
                          break;
                     case cmp_for_Qwerty:
                     case cmp_for_FullMatchQwerty:
                          if ( pIme->getMemData_CQKey(chk, wsKeys) ==0 )
                          {
                               return false;
                          }
                          break;
                     case cmp_for_HeadMapping:
                          if ( pIme->getMemData_CHKey(chk, wsKeys) ==0 )
                          {
                               return false;
                          }
                          break;
            }

            int
            pos =wsKeys.find(In.wsKeys);

            bool
            match =(pos ==0) && (pos != std::string::npos);

            return match;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_target_cmpID
{
       bool operator()( PHRS_UI const &a, PHRS_UI const &b) const
       {
            return a.sID <b.sID;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     return getKeyFuzzyEx(&v2dict, cmp_for_T9, sID, wsKeys, v, wsKeyMatch, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyFuzzyW(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     return getKeyFuzzyEx(&v2dictW, cmp_for_Qwerty, sID, wsKeys, v, wsKeyMatch, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyFullMatchW(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     return getKeyFuzzyEx(&v2dictW, cmp_for_FullMatchQwerty, sID, wsKeys, v, wsKeyMatch, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyFuzzyH(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     return getKeyFuzzyEx(&v2dictH, cmp_for_HeadMapping, sID, wsKeys, v, wsKeyMatch, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy ( not used )
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyFullMatchH(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     return getKeyFuzzyEx(&v2dictH, cmp_for_FullMatchHeadMapping, sID, wsKeys, v, wsKeyMatch, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyFuzzyEx(vector<V2W_DICT_UI> *pV2dict, CMP_FUNC_FLAG flag, int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     setlocale( LC_ALL, "");

     if ( pV2dict->size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"v2dict total =%d\r\n", pV2dict->size());
          #else
          swprintf(aa, 1024, L"v2dict total =%d\r\n", pV2dict->size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     int
     cmpLen      =wcslen(wsKeyIn),
     cmpLenMatch =wcslen(wsKeyMatch);

     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :pV2dict->size());

     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     set<wstring>
     vdictSet;

     vector<PHRS_UI>
     vdictGet;

     PHRS_UI
     ui(this, sID, wsKeyIn, wsKeyMatch);

     V2W_DICT_UI_ITR
     ui_found =pV2dict->end();

     if ( flag !=cmp_for_T9 )
     {
          // 頭碼不需要先找到所有Key相同的第一個Index
     }
     else
     {
          //ui_found =
          //find_if(pV2dict->begin(), pV2dict->end(), v2_1w_PHRS_UI_keyMatchFuzzy(this, flag, ui, cmpLenMatch));

          long Up  =pV2dict->size() -1;
          long Low =0;
          long Hit;
          long Cmp;

          V2W_DICT_UI_ITR
          ui_found =pV2dict->end();

          while ( Low <=Up )
          {
                  Hit = long(( Up +Low ) /2);

                  wstring
                  wsKeys, wsPhrs;
                  if ( pV2dict ==&v2dictW)
                  {
                       if ( getMemData_CKeys(v2dictW[Hit], wsKeys) ==0 )
                       {
                            return 0;
                       }
                       if ( getMemData_CPhrs(v2dictW[Hit], wsPhrs) ==0 )
                       {
                            return 0;
                       }
                  }
                  else if ( pV2dict ==&v2dictH)
                  {
                       if ( getMemData_CHKey(v2dictH[Hit], wsKeys) ==0 )
                       {
                            return 0;
                       }
                       if ( getMemData_CHVoc(v2dictH[Hit], wsPhrs) ==0 )
                       {
                            return 0;
                       }
                  }
                  else
                  {
                       if ( getMemData_CKeys(v2dict[Hit], wsKeys) ==0 )
                       {
                            return 0;
                       }
                       if ( getMemData_CPhrs(v2dict[Hit], wsPhrs) ==0 )
                       {
                            return 0;
                       }
                  }

                  Cmp =ui.wsKeys.compare(wsKeys);

                  if ( Cmp ==0 )
                  {
                       switch ( flag )
                       {
                                case cmp_for_T9:
                                case cmp_for_Qwerty:
                                case cmp_for_FullMatchQwerty:
                                     Cmp =ui.wsKeys.compare(wsKeys);
                                     if ( ui.wsPhrs.size() >0 )
                                     {
                                          if ( wsPhrs.size() >1 )
                                          {
                                               wsPhrs =wsPhrs.erase(0, 2);
                                          }
                                          Cmp =ui.wsPhrs.compare(wsPhrs);
                                     }
                                     break;

                                case cmp_for_FullMatchHeadMapping:
                                case cmp_for_HeadMapping:
                                     break;
                       }
                  }

                  if ( Cmp ==0 )
                  {
                       ui_found =pV2dict->begin() +Hit;
                       break;
                  }
                  else if ( Cmp >0 )
                  {
                       Low = Hit +1;
                  }
                  else
                  {
                       Up = Hit -1;
                  }
          }
     }

     int
     getTotal =0;

     if ( ui_found !=pV2dict->end() )
     {
          bool
          match =true;

          V2W_DICT_UI_ITR
          bi1 =ui_found -1;

          // 先找到首個 keys & phrase 都一樣的Index
          while ( ui_found !=pV2dict->begin() )
          {
                  wstring
                  wsKeys;
                  switch ( flag )
                  {
                           case cmp_for_T9:
                                if ( getMemData_Keys(bi1, wsKeys) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_Qwerty:
                           case cmp_for_FullMatchQwerty:
                                if ( getMemData_QKey(bi1, wsKeys) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_HeadMapping:
                                if ( getMemData_HKey(bi1, wsKeys) ==0 )
                                {
                                     break;
                                }
                  }
                  wstring
                  wsPhrs;
                  switch ( flag )
                  {
                           case cmp_for_T9:
                           case cmp_for_Qwerty:
                           case cmp_for_FullMatchQwerty:
                                if ( getMemData_Phrs(bi1, wsPhrs) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_HeadMapping:
                                if ( getMemData_HVoc(bi1, wsPhrs) ==0 )
                                {
                                     break;
                                }
                  }

                  int
                  cmp =wsKeys.compare(wsKeyIn);

                  if ( cmp !=0 )
                       break;

                  //2018/07/03
                  //cmp =wsPhrs.compare(2, cmpLenMatch, wsKeyMatch);

                  //if ( cmp !=0 )
                  //{
                       //break;
                  //}
                  ui_found =bi1--;
          }

          // 再往下找
          while ( getTotal <total && ui_found !=pV2dict->end() )
          {
                  wstring
                  wsKeys;
                  switch ( flag )
                  {
                           case cmp_for_T9:
                                if ( getMemData_Keys(ui_found, wsKeys) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_Qwerty:
                           case cmp_for_FullMatchQwerty:
                                if ( getMemData_QKey(ui_found, wsKeys) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_HeadMapping:
                                if ( getMemData_HKey(ui_found, wsKeys) ==0 )
                                {
                                     break;
                                }
                  }
                  wstring
                  wsPhrs;
                  switch ( flag )
                  {
                           case cmp_for_T9:
                           case cmp_for_Qwerty:
                           case cmp_for_FullMatchQwerty:
                                if ( getMemData_Phrs(ui_found, wsPhrs) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_HeadMapping:
                                if ( getMemData_HVoc(ui_found, wsPhrs) ==0 )
                                {
                                     break;
                                }
                  }

                  #if defined(_WIN32) && defined(__DEBUG_MSG_LOG)
                  //TRACE(_T("%ls, %ls\r\n"), wsKeys.c_str(), wsPhrs.c_str());
                  #endif

                  int
                  cmp =wsKeys.compare(wsKeyIn);

                  if ( cmp !=0 )
                  {
                       break;
                  }
                  //2018/07/03
                  if ( wsPhrs.size() <2 )
                  {
                       ui_found++;
                       continue;
                  }
                  if ( flag ==cmp_for_T9 )
                  {
                       cmp =wsPhrs.compare(2, cmpLenMatch, wsKeyMatch);

                       if ( cmp !=0 )
                       {
                            ui_found++;
                            continue;
                       }
                  }

                  PHRS_UI
                  ui2(this, ui_found->sID, wsKeys.c_str(), wsPhrs.c_str(), ui_found->iNext);

                  bool
                  ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                  if ( ok ==true )
                  {
                       vdictGet.push_back(ui2);
                       vdictSet.insert(ui2.wsPhrs);

                       getTotal++;

                       #if !defined(__DEBUG_MSG)
                       {
                           wchar_t
                           aa[1024] ={0};

                           #if defined(_WIN32)
                           swprintf(aa, L"1, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                           #else
                           swprintf(aa, 1024, L"1, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                           #endif

                           //dw_pintf_ex(aa);
                           printf("aa");
                       }
                       #endif
                  }
                  ui_found++;
          }
     }

     //------------------------------------------------------------------------
     // get match key all of rest
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;

     if ( getTotal <total )
     {
          #if defined(__DEBUG_MSG_RUN_MS)
          time_t
          now = time(0);
          clock_t
          t1 =clock();
          #endif

          V2W_DICT_UI_ITR
          ui_found =find_if(pV2dict->begin(), pV2dict->end(), v2_1w_PHRS_UI_CmpKeyLenFuzzy(this, flag, ui));

          #if defined(__DEBUG_MSG_RUN_MS)
          now = time(0);
          clock_t
          t2 =clock();
          float
          runningTime =((float)t2-(float)t1);

          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32) || defined(__TEST)
          swprintf(aa, L"phraseimp_v2_1w::%ls, run ms21 =%f\r\n", wsKeyIn, runningTime /1000);
          dw_pintf(aa);
          #else
           #if defined(__HEAD_QUICK_SEARCH_DEBUG)
          KIKA_LOGD("phraseimp_v2_1w::%ls, run ms21 check count =%d\r\n", wsKeyIn, (int)(pV2dict->end() -pV2dict->begin()));
           #endif
          KIKA_LOGD("phraseimp_v2_1w::%ls, run ms21 =%f\r\n", wsKeyIn, runningTime /1000);
          #endif

          #endif // defined

          while ( getTotal <total && vdictGet.size() <getRangeItem )
          {
                  if ( ui_found ==pV2dict->end() )
                  {
                       break;
                  }

                  wstring
                  wsKeys;
                  switch ( flag )
                  {
                           case cmp_for_T9:
                                if ( getMemData_Keys(ui_found, wsKeys) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_Qwerty:
                           case cmp_for_FullMatchQwerty:
                                if ( getMemData_QKey(ui_found, wsKeys) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_HeadMapping:
                                if ( getMemData_HKey(ui_found, wsKeys) ==0 )
                                {
                                     break;
                                }
                  }

                  //-----------------------------------------------------------
                  // rule same as v2_1w_PHRS_UI_CmpKeyLenFuzzy
                  //-----------------------------------------------------------
                  int
                  pos =ui_found->wKeys.find(wsKeyIn);

                  bool
                  match =(pos ==0) && (pos != std::string::npos);

                  if ( match ==false )
                  {
                       break;
                  }
                  //-----------------------------------------------------------

                  wstring
                  wsPhrs;
                  switch ( flag )
                  {
                           case cmp_for_T9:
                           case cmp_for_Qwerty:
                           case cmp_for_FullMatchQwerty:
                                if ( getMemData_Phrs(ui_found, wsPhrs) ==0 )
                                {
                                     break;
                                }
                                break;
                           case cmp_for_HeadMapping:
                                if ( getMemData_HVoc(ui_found, wsPhrs) ==0 )
                                {
                                     break;
                                }
                  }

                  #if defined(_WIN32) && defined(__DEBUG_MSG_KOG)
                  //TRACE(_T("Phrs =%ls, Keys =%ls\r\n"), wsPhrs.c_str(), wsKeys.c_str());
                  #endif

                  PHRS_UI
                  ui2(this, ui_found->sID, wsKeys.c_str(), wsPhrs.c_str());

                  bool
                  ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                  if ( ok ==true )
                  {
                       match =true;

                       switch ( flag )
                       {
                                case cmp_for_T9:
                                     match =(cmpLen <=(int)wsKeys.size());
                                     if ( wsKeyMatch !=0 && wcslen(wsKeyMatch) >0 )
                                     {
                                          int
                                          end =ui2.wsPhrs.find(wsKeyMatch, 2);

                                          match =(end !=std::string::npos) && end ==2;
                                     }
                                     break;

                                case cmp_for_Qwerty:
                                     match =(cmpLen <=(int)wsKeys.size());
                                     break;

                                case cmp_for_FullMatchQwerty:
                                     match =(cmpLen ==(int)wsKeys.size());
                                     break;

                                case cmp_for_HeadMapping:
                                     //字長>輸入字根, 視為不Match
                                     match =(cmpLen <=(int)wsKeys.size());
                                     if ( match ==true )
                                     {
                                          for ( int i =0; i <cmpLen; i++ )
                                          {
                                                if ( wsKeys[i] !=wsKeyIn[i] )
                                                {
                                                    match =false;
                                                    break;
                                                }
                                          }
                                     }
                                     break;

                                case cmp_for_FullMatchHeadMapping:
                                     //Head Match 要完全Match
                                     match =(cmpLen ==(int)wsKeys.size());
                                     break;
                       }

                       if ( match ==true )
                       {
                            vdictGet2.push_back(ui2);
                            vdictSet.insert(ui2.wsPhrs);

                            getTotal++;

                            #if defined(__DEBUG_MSG)
                            {
                                 wchar_t
                                 aa[1024] ={0};

                                 #if defined(_WIN32)
                                 swprintf(aa, L"2, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                                 #else
                                 swprintf(aa, 1024, L"2, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                                 #endif

                                 dw_pintf_ex(aa);
                            }
                            #endif
                       }
                  }

                  // find next
                  //bi_next =ui_found +1;
                  ui_found++;
          }
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"Get Match Total =%d\r\n", vdictGet.size());
          #else
          swprintf(aa, 1024, L"Get Match Total =%d\r\n", vdictGet.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     // fill back data
     //------------------------------------------------------------------------
     fillData(vdictGet, v);

     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_1w_target_cmpID());

     fillData(vdictGet2, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v2_1_target_equal()), v.end());

     #endif

     //------------------------------------------------------------------------
     if ( v.size() >(unsigned long)total )
     {
          v.resize(total);
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_keyMatch_Vpos
{
       PHRS_UI               In;

       v2_1w_PHRS_UI_keyMatch_Vpos(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_CKeys(chk, wsKeys) ==0 )
            {
                 return false;
            }

            return wsKeys ==In.wsKeys;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_keyMatch_Key1
{
       PHRS_UI               In;
       int                   cmpLen;

       v2_1w_PHRS_UI_keyMatch_Key1(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_CKeys(chk, wsKeys) ==0 )
            {
                 return false;
            }

            return wcsncmp(In.wsKeys.c_str(), wsKeys.c_str(), cmpLen) ==0;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     if ( v2dict.size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
         wchar_t
         aa[1024] ={0};

         #if defined(_WIN32)
         swprintf(aa,       L"v2dict total =%d\r\n", v2dict.size());
         #else
         swprintf(aa, 1024, L"v2dict total =%d\r\n", v2dict.size());
         #endif

         dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);

     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :v2dict.size());

     set<wstring>
     vdictSet;

     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet;

     PHRS_UI
     ui(this, sID, wsKeyIn, L"");

     V2W_DICT_UI_ITR
     ui_found =find_if(v2dict.begin(), v2dict.end(), v2_1w_PHRS_UI_keyMatch_Vpos(ui));

     int
     getTotal =0;

     // if found any
     if ( ui_found !=v2dict.end() )
     {
          while ( getTotal <total && ui_found !=v2dict.end() )
          {
                  V2W_DICT_UI_ITR
                  bi1 =ui_found;

                  wstring
                  wsKeys;
                  if ( getMemData_Keys(bi1, wsKeys) ==0 )
                  {
                       break;
                  }
                  wstring
                  wsPhrs;
                  if ( getMemData_Phrs(bi1, wsPhrs) ==0 )
                  {
                       break;
                  }

                  PHRS_UI
                  ui2(this, ui_found->sID, ui_found->wKeys, ui_found->wPhrs);

                  #if defined(_WIN32) && defined(__DEBUG_MSG)
                  {
                      const int     sID =ui2.sID;
                      const wchar_t *p1 =ui2.wsKeys.c_str();
                      const wchar_t *p2 =ui2.wsPhrs.c_str();
                      const wchar_t *p3 =ui_found->wsStrokes.c_str();

                      wchar_t
                      wsTmp[450] ={0};
                      wsprintf(wsTmp, _T("sID =%d, k=%ls, w=%ls, stk =%ls\r\n"), sID, p1, p2, p3);

                      dw_pintf_ex(wsTmp);
                  }
                  #endif

                  //-----------------------------------------------------------
                  // Must be equal to Input Key Length
                  //-----------------------------------------------------------
                  if ( (int)wsKeys.size() !=cmpLen )
                  {
                       break;
                  }

                  int
                  cmp =wcsncmp(ui.wsKeys.c_str(), wsKeys.c_str(), cmpLen);

                  if ( cmp !=0 )
                  {
                       break;
                  }

                  bool
                  ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                  if ( ok ==true )
                  {
                       vdictGet.push_back(ui2);
                       vdictSet.insert(ui2.wsPhrs);

                       getTotal++;

                       #if defined(__DEBUG_MSG)
                       {
                           wchar_t
                           aa[1024] ={0};

                           #if defined(_WIN32)
                           swprintf(aa,       L"1, %ls, %ls\r\n", ui2.wsKey.c_str(), ui2.wsPhrase.c_str());
                           #else
                           swprintf(aa, 1024, L"1, %ls, %ls\r\n", ui2.wsKey.c_str(), ui2.wsPhrase.c_str());
                           #endif

                           dw_pintf_ex(aa);
                       }
                       #endif
                  }
                  ui_found++;
          }
     }

     //------------------------------------------------------------------------
     // get match key all of rest
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;

     if ( getTotal <total )
     {
          //assume that, some canidate will be eliminate by some condition
          //so we take more 20 canidates
          int
          total2 =cmpLen ==1 ? total :total +20;

          // not found any same length same keys, the search from begin
          if ( vdictGet.size() ==0 )
          {
               ui_found =
               find_if(v2dict.begin(), v2dict.end(), v2_1w_PHRS_UI_keyMatch_Key1(ui, cmpLen));
          }

          // if found any
          if ( ui_found !=v2dict.end() )
          {
               while ( ui_found !=v2dict.end() )
               {
                       wstring
                       wsKeys;
                       if ( getMemData_Keys(ui_found, wsKeys) ==0 )
                       {
                            break;
                       }

                       wstring
                       wsPhrs;
                       if ( getMemData_Phrs(ui_found, wsPhrs) ==0 )
                       {
                            break;
                       }

                       PHRS_UI
                       ui2(this, ui_found->sID, ui_found->wKeys, ui_found->wPhrs);

                       #if defined(_WIN32) && defined(__DEBUG_MSG)
                       {
                           const int     sID =ui2.sID;
                           const wchar_t *p1 =ui2.wsKeys.c_str();
                           const wchar_t *p2 =ui2.wsPhrs.c_str();
                           const wchar_t *p3 =ui_found->wsStrokes.c_str();

                           wchar_t
                           wsTmp[450] ={0};
                           wsprintf(wsTmp, _T("sID =%d, k=%ls, w=%ls, stk =%ls\r\n"), sID, p1, p2, p3);

                           dw_pintf_ex(wsTmp);
                       }
                       #endif

                       //------------------------------------------------------
                       // Must be equal to Input Key Length
                       //------------------------------------------------------
                       int
                       cmp =wcsncmp(ui.wsKeys.c_str(), wsKeys.c_str(), cmpLen);

                       if ( cmp !=0 )
                       {
                            break;
                       }

                       bool
                       ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                       if ( ok ==true )
                       {
                            vdictGet2.push_back(ui2);
                            vdictSet.insert(ui2.wsPhrs);

                            getTotal++;

                            #if defined(__DEBUG_MSG)
                            {
                                wchar_t
                                aa[1024] ={0};

                                #if defined(_WIN32)
                                swprintf(aa,       L"1, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                                #else
                                swprintf(aa, 1024, L"1, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                                #endif

                                dw_pintf_ex(aa);
                            }
                            #endif
                       }
                       ui_found++;
               }
          }
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa,       L"Get Match Total =%d\r\n", vdictGet.size());
          #else
          swprintf(aa, 1024, L"Get Match Total =%d\r\n", vdictGet.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     // fill data
     //------------------------------------------------------------------------
     std::sort(vdictGet.begin(), vdictGet.end(), v2_1w_target_cmpID());
     fillData(vdictGet, v);

     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_1w_target_cmpID());
     fillData(vdictGet2, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v2_1_target_equal()), v.end());

     #endif

     //------------------------------------------------------------------------
     if ( v.size() >(unsigned long)total )
     {
          v.resize(total);
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_fuzzyMatch
{
       PHRS_UI               In;

       v2_1w_PHRS_UI_fuzzyMatch(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const V2W_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_CKeys(chk, wsKeys) ==0 )
            {
                 return false;
            }

            //if ( chk.wsKeys.length() >In.wsKeys.length() )
            {
                 int
                 pos =wsKeys.find(In.wsKeys);

                 bool
                 match =(pos ==0) && (pos != std::string::npos);

                 return match;
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : just fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     if ( v2dict.size() ==0 )
     {
          return 0;
     }

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"v2dict.size() =%d\r\n", v2dict.size());
          #else
          swprintf(aa, 1024, L"v2dict.size() =%d\r\n", v2dict.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_UI
     ui(this, sID, wsKeyIn, (wchar_t *)L"");

     V2W_DICT_UI_ITR
     bi =v2dict.begin();

     while ( v.size() <(unsigned long)total )
     {
             V2W_DICT_UI_ITR
             ui_found =find_if(bi, v2dict.end(), v2_1w_PHRS_UI_fuzzyMatch(ui));

             if ( ui_found ==v2dict.end() )
                  break;

             wstring
             wsPhrs;
             if ( getMemData_Phrs(ui_found, wsPhrs) ==0 )
             {
                  continue; //error
             }

             v.push_back(decrypt(wsPhrs, string(ENC_KEY_SZ)));

             bi =ui_found +1;
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_1w_PHRS_UI_keyMatchFuzzyEx
{
       PHRS_UI               In;

       v2_1w_PHRS_UI_keyMatchFuzzyEx(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            int
            pos =chk.wsKeys.find(In.wsKeys);

            if ( pos !=std::string::npos && pos ==0 )
            {
                 return true;
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::getKeyWildc(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsWildcard, int total)
{
     setlocale( LC_ALL, "");

     if ( v2dict.size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"v2dict total =%d\r\n", v2dict.size());
          #else
          swprintf(aa, 1024, L"v2dict total =%d\r\n", v2dict.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);

     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :v2dict.size());

     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     set<wstring>
     vdictSet;

     PHRS_UI
     ui(this, sID, wsKeyIn, L"");

     vector<PHRS_UI>
     vdictGet2;

     //if ( getTotal <total )
     {
          V2W_DICT_UI_ITR
          bi_next =v2dict.begin();

          while ( vdictGet2.size() <getRangeItem )
          {
                  V2W_DICT_UI_ITR
                  ui_found =find_if(bi_next, v2dict.end(), v2_1w_PHRS_UI_CmpKeyLen(this, ui));

                  if ( ui_found ==v2dict.end() )
                  {
                       break;
                  }

                  wstring
                  wsKeys;
                  if ( getMemData_Keys(ui_found, wsKeys) ==0 )
                  {
                       break;
                  }
                  wstring
                  wsPhrs;
                  if ( getMemData_Phrs(ui_found, wsPhrs) ==0 )
                  {
                       break;
                  }

                  #if defined(_WIN32) && defined(__DEBUG_MSG_KOG)
                  //TRACE(_T("Phrs =%ls, Keys =%ls\r\n"), wsPhrs.c_str(), wsKeys.c_str());
                  #endif

                  PHRS_UI
                  ui2(this, ui_found->sID, wsKeys.c_str(), wsPhrs.c_str());

                  bool
                  ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                  if ( ok ==true )
                  {
                       int
                       cmp =0;

                       {
                            for ( int i =0; i <cmpLen; i++ )
                            {
                                  wchar_t
                                  wCH =wsKeyIn[i];

                                  if ( wCH ==wsWildcard[0] )
                                       continue;

                                  if ( wCH !=wsKeys[i] )
                                  {
                                       cmp =-1;
                                       break;
                                  }
                            }
                       }
                       if ( cmp !=0 )
                       {
                            //if ( wcLen ==0 )
                            //     break;
                            bi_next =ui_found +1;
                            continue;
                       }
                       vdictGet2.push_back(ui2);
                       vdictSet.insert(ui2.wsPhrs);
                  }

                  // find next
                  bi_next =ui_found +1;
          }
     }

     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"Get Match Total =%d\r\n", vdictGet.size());
          #else
          swprintf(aa, 1024, L"Get Match Total =%d\r\n", vdictGet.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     //------------------------------------------------------------------------
     // fill data
     //------------------------------------------------------------------------
     // vdictGet no need to re-order
     //fillData(vdictGet, v);

     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_1w_target_cmpID());
     fillData(vdictGet2, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v2_1_target_equal()), v.end());

     #endif

     //------------------------------------------------------------------------
     if ( v.size() >(unsigned long)total )
     {
          v.resize(total);
     }
     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_1w::get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total, bool fuzzy)
{
     if ( v2dict.size() ==0 )
     {
          return 0;
     }
     if ( fuzzy )
     {
          getFuzzy(sID, wsKeys, v, total);
     }
     else
     {
          getKey(sID, wsKeys, v, total);
     }

     #if defined(__APPLY_UNIQUE_RULE)

     std::sort(v.begin(), v.end());
     v.erase(std::unique(v.begin(), v.end(), v2_1_target_equal()), v.end());

     #endif

     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp_v2_1w::isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     return false;
}
