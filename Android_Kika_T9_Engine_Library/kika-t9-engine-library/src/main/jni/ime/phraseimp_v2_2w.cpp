// phraseimp_v2_2w.cpp: implementation of the phraseimp_v2_2w class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phraseimp_v2_2w.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define __HEAD_SEARCH
#define __HEAD_QUICK_SEARCH

// #define __DEBUG_MSG_MEM


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
phraseimp_v2_2w::phraseimp_v2_2w()
{

}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
phraseimp_v2_2w::~phraseimp_v2_2w()
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
void phraseimp_v2_2w::clear()
{
     #if defined(__DEBUG_MSG_MEM)
     KIKA_LOGD("Kika-ENGINE, phraseimp_v2_2w::clear()");
     #endif

     vdictW_Pos.clear();
     #if defined(__LINUX_BASE)
     vdictW_Pos.shrink_to_fit();
     #endif

     chr_HCharIndex.clear();
     voc_HCharIndex.clear();

     phraseimp_v2_1w::clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp_v2_2w::loadDict(const char* dictName)
{
     clear();

     #if defined(__DEBUG_MSG_LOADDICT_MS)
     time_t
     now = time(0);
     clock_t
     t1_1 =clock();
     #endif

     FILE
     *fp =fopen(dictName, "rb");

     if ( fp )
     {
          char
          headSz[sizeof(HEAD_ID_DICT_V2_2W) +1] ={0};

          fread(headSz, sizeof(HEAD_ID_DICT_V2_2W) *sizeof(char), 1, fp);

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
          sizePhrs        =0,
          sizeKeys        =0,
          sizeNext        =0,
          sizeChrHChar    =0,
          sizeVocHW_HChar =0,
          sizePosVoc_Max  =0;

          fread(&sizePhrs,        sizeof(MEM_INT32), 1, fp);
          fread(&sizeKeys,        sizeof(MEM_INT32), 1, fp);
          fread(&sizeNext,        sizeof(MEM_INT32), 1, fp);
          fread(&sizeChrHChar,    sizeof(MEM_INT32), 1, fp);
          fread(&sizeVocHW_HChar, sizeof(MEM_INT32), 1, fp);
          fread(&sizePosVoc_Max,  sizeof(MEM_INT32), 1, fp);

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

          //-------------------------------------------------------------------
          // Read HeadChar Index of Qwerty Keys for Char
          //-------------------------------------------------------------------
          for ( MEM_INT32 chr_idx =0; chr_idx <sizeChrHChar; chr_idx++ )
          {
                V2_2W_HWCHAR_IDX
                u2w;

                fread(&u2w, sizeof(V2_2W_HWCHAR_IDX), 1, fp);

                chr_HCharIndex.insert(u2w);

                #if defined(_WIN32) && defined(__DEBUG_MSG_LOG)
                //TRACE(_T("%c, pos =%ld, Total =%ld\r\n"), u2w.wCH, u2w.Pos, u2w.Total);
                #endif
          }

          //-------------------------------------------------------------------
          // Read HeadChar Index of Qwerty Keys for Voca
          //-------------------------------------------------------------------
          for ( MEM_INT32 voc_idx =0; voc_idx <sizeVocHW_HChar; voc_idx++ )
          {
                V2_2W_HWCHAR_IDX
                u2w;

                fread(&u2w, sizeof(V2_2W_HWCHAR_IDX), 1, fp);

                voc_HCharIndex.insert(u2w);

                #if defined(_WIN32) && defined(__DEBUG_MSG_LOG)
                //TRACE(_T("%c, pos =%ld, Total =%ld\r\n"), u2w.wCH, u2w.Pos, u2w.Total);
                #endif
          }

          //-------------------------------------------------------------------
          // Read Qwerty sorting Index position
          //-------------------------------------------------------------------
          for ( MEM_INT32 q_idx =0; q_idx <sizePosVoc_Max; q_idx++ )
          {
                MEM_INT32
                Pos;

                fread(&Pos, sizeof(MEM_INT32), 1, fp);

                vdictW_Pos.push_back(Pos);
          }

          //-------------------------------------------------------------------
          //
          //-------------------------------------------------------------------
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
                /*
                if ( u2.iQKey !=INVALID_MEM_OFFSET )
                {
                     if ( getMemData_QKey(u2, u2.wKeys) ==0 )
                     {
                          continue;
                     }
                     v2dictW.push_back(u2);
                }
                */

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
          // Read (Char) Qwerty Keys
          //-------------------------------------------------------------------
          for ( idx =0; idx <vSize && idx <vdictW_Pos.size(); idx++ )
          {
                MEM_INT32
                pos =vdictW_Pos[idx];

                V2W_DICT_UI
                u2 =v2dict[pos];

                if ( u2.iQKey ==INVALID_MEM_OFFSET )
                     continue;

                // PreLoad
                if ( getMemData_QKey(u2, u2.wKeys) ==0 )
                {
                     continue;
                }
                v2dictW.push_back(u2);
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

          #if defined(__DEBUG_MSG_LOADDICT_MS)
          now = time(0);
          clock_t
          t1_2 =clock();
          float runningTime =((float)t1_2-(float)t1_1);

          wchar_t
          aa[1024] ={0};

          swprintf(aa, 1024, L"phraseimp_v2_2w::loadDict spend time =%f\r\n", runningTime /1000);
          wcout<<aa;
          #endif // defined

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
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_2w::getKeyFuzzyW(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     return getKeyFuzzyEx(&v2dictW, cmp_for_Qwerty, sID, wsKeys, v, wsKeyMatch, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_2w_target_cmpID
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
//
//-----------------------------------------------------------------------------
struct v2_2w_PHRS_UI_CmpKeyLenFuzzy
{
       PHRS_UI               In;
       phraseimp*            pIme;
       CMP_FUNC_FLAG         flag;

       v2_2w_PHRS_UI_CmpKeyLenFuzzy(phraseimp* _pIme, CMP_FUNC_FLAG _flag, PHRS_UI input) :
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
struct v2_2w_PHRS_UI_keyMatchFuzzy
{
       PHRS_UI               In;
       int                   cmpMatchLen;
       phraseimp*            pIme;
       CMP_FUNC_FLAG         flag;

       v2_2w_PHRS_UI_keyMatchFuzzy(phraseimp* _pIme, CMP_FUNC_FLAG _flag, PHRS_UI _input, int _cmpLen) :
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
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v2_2w::getKeyFuzzyEx(vector<V2W_DICT_UI> *pV2dict, CMP_FUNC_FLAG flag, int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
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
     p_end   =pV2dict->end(),
     p_begin =pV2dict->begin();

     switch ( flag )
     {
              #if defined(__HEAD_QUICK_SEARCH)
              case cmp_for_T9:
              case cmp_for_Qwerty:
              case cmp_for_FullMatchQwerty:
                   {
                        V2_2W_HWCHAR_IDX
                        ui;
                        ui.wCH =wsKeyIn[0];

                        set<V2_2W_HWCHAR_IDX>::iterator
                        pFoundHChar =chr_HCharIndex.find(ui);

                        if ( pFoundHChar !=chr_HCharIndex.end() )
                        {
                             int
                             pos   =pFoundHChar->Pos,
                             Total =pFoundHChar->Total;

                             p_begin =pV2dict->begin() +pos;
                             p_end   =(p_begin +Total);
                        }
                        break;
                   }
                   break;
              #else
                   break;
              #endif

              case cmp_for_FullMatchHeadMapping:
              case cmp_for_HeadMapping:
                   {
                        V2_2W_HWCHAR_IDX
                        ui;
                        ui.wCH =wsKeyIn[0];

                        set<V2_2W_HWCHAR_IDX>::iterator
                        pFoundHChar =voc_HCharIndex.find(ui);

                        if ( pFoundHChar !=voc_HCharIndex.end() )
                        {
                             int
                             pos   =pFoundHChar->Pos,
                             Total =pFoundHChar->Total;

                             p_begin =pV2dict->begin() +pos;
                             p_end   =(p_begin +Total);
                        }
                        break;
                   }
     }

     V2W_DICT_UI_ITR
     ui_found =p_end;

     if ( flag !=cmp_for_T9 )
     {
          // 頭碼不需要先找到所有Key相同的第一個Index
     }
     else
     {
          ui_found =
          find_if(p_begin, p_end, v2_2w_PHRS_UI_keyMatchFuzzy(this, flag, ui, cmpLenMatch));
     }

     int
     getTotal =0;

     if ( ui_found !=p_end )
     {
          bool
          match =true;

          V2W_DICT_UI_ITR
          bi1 =ui_found -1;

          // 先找到首個 keys & phrase 都一樣的Index
          while ( ui_found !=p_begin )
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
          while ( getTotal <total && ui_found !=p_end )
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

                       #if defined(__DEBUG_MSG)
                       {
                           wchar_t
                           aa[1024] ={0};

                           #if defined(_WIN32)
                           swprintf(aa, L"1, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
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

     //------------------------------------------------------------------------
     // get match key all of rest
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;

     if ( getTotal <total )
     {
          //V2W_DICT_UI_ITR
          //bi_next =p_begin;

          V2W_DICT_UI_ITR
          ui_found =find_if(p_begin, p_end, v2_2w_PHRS_UI_CmpKeyLenFuzzy(this, flag, ui));

          while ( getTotal <total && vdictGet.size() <getRangeItem )
          {
                  if ( ui_found ==p_end )
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
                       bool
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

     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_2w_target_cmpID());

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
struct v2_2w_target_cmpIDVc
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
int  phraseimp_v2_2w::getKeyAllFuzzyW(int sID, const wchar_t *wsKeyIn, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total)
{
     setlocale( LC_ALL, "");

     vector<V2W_DICT_UI> *
     pV2dict =&v2dict;

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
     p_end   =pV2dict->end(),
     p_begin =pV2dict->begin();

     CMP_FUNC_FLAG
     flag =cmp_for_T9;

     switch ( flag )
     {
              #if defined(__HEAD_QUICK_SEARCH)
              case cmp_for_T9:
              case cmp_for_Qwerty:
              case cmp_for_FullMatchQwerty:
                   {
                        V2_2W_HWCHAR_IDX
                        ui;
                        ui.wCH =wsKeyIn[0];

                        set<V2_2W_HWCHAR_IDX>::iterator
                        pFoundHChar =chr_HCharIndex.find(ui);

                        if ( pFoundHChar !=chr_HCharIndex.end() )
                        {
                             int
                             pos   =pFoundHChar->Pos,
                             Total =pFoundHChar->Total;

                             p_begin =pV2dict->begin() +pos;
                             p_end   =(p_begin +Total);
                        }
                        break;
                   }
                   break;
              #else
                   break;
              #endif

              case cmp_for_FullMatchHeadMapping:
              case cmp_for_HeadMapping:
                   {
                        V2_2W_HWCHAR_IDX
                        ui;
                        ui.wCH =wsKeyIn[0];

                        set<V2_2W_HWCHAR_IDX>::iterator
                        pFoundHChar =voc_HCharIndex.find(ui);

                        if ( pFoundHChar !=voc_HCharIndex.end() )
                        {
                             int
                             pos   =pFoundHChar->Pos,
                             Total =pFoundHChar->Total;

                             p_begin =pV2dict->begin() +pos;
                             p_end   =(p_begin +Total);
                        }
                        break;
                   }
     }

     V2W_DICT_UI_ITR
     ui_found =p_end;

     if ( flag !=cmp_for_T9 )
     {
          // 頭碼不需要先找到所有Key相同的第一個Index
     }
     else
     {
          ui_found =
          find_if(p_begin, p_end, v2_2w_PHRS_UI_keyMatchFuzzy(this, flag, ui, cmpLenMatch));
     }

     int
     getTotal =0;

     if ( ui_found !=p_end )
     {
          bool
          match =true;

          V2W_DICT_UI_ITR
          bi1 =ui_found -1;

          // 先找到首個 keys & phrase 都一樣的Index
          while ( ui_found !=p_begin )
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
          while ( getTotal <total && ui_found !=p_end )
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

                       #if defined(__DEBUG_MSG)
                       {
                           wchar_t
                           aa[1024] ={0};

                           #if defined(_WIN32)
                           swprintf(aa, L"1, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
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

     //------------------------------------------------------------------------
     // get match key all of rest
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;

     if ( getTotal <total )
     {
          //V2W_DICT_UI_ITR
          //bi_next =p_begin;

          V2W_DICT_UI_ITR
          ui_found =find_if(p_begin, p_end, v2_2w_PHRS_UI_CmpKeyLenFuzzy(this, flag, ui));

          while ( getTotal <total && vdictGet.size() <getRangeItem )
          {
                  if ( ui_found ==p_end )
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
                       bool
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
     fillDataVc(vdictGet, v);

     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_2w_target_cmpIDVc());

     fillDataVc(vdictGet2, v);

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
