// phraseimp_v2.cpp: implementation of the phraseimp_v2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "string.h"
#include "phraseimp_v2.h"
#include "debug_util.h"

#define __ENCODE_MSG
#define __APPLY_UNIQUE_RULE

// #define __DEBUG_MSG_MEM
// #define __DEBUG_ORG_LOG_DATA_ORG
// #define __DEBUG_ORG_LOG_DATA
// #define __DEBUG_ORG_LOG_KLEN

#if defined(_DEBUG_LOG) || defined(__DEBUG_ORG_LOG_DATA) || defined(__DEBUG_ORG_LOG_KLEN) || defined(__DEBUG_ORG_LOG_DATA_ORG)
 #include "WfxFile.h"
#endif

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
phraseimp_v2::phraseimp_v2()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
phraseimp_v2::~phraseimp_v2()
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
void phraseimp_v2::clear()
{
     #if defined(__DEBUG_MSG_MEM)
     KIKA_LOGD("Kika-ENGINE, phraseimp_v2::clear()");
     #endif

     v1dpos.clear();
     v2klen.clear();
     v2dict.clear();
     v2dgrp_dynload.clear();
     v2dgrp_preload.clear();

     #if defined(__LINUX_BASE)
     v1dpos.shrink_to_fit();
     v2klen.shrink_to_fit();
     v2dict.shrink_to_fit();
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
struct v2_target_equal
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
struct v2_PHRS_UI_keyMatchFuzzy
{
       PHRS_UI               In;
       int                   cmpLen;
       phraseimp*            pIme;
       
       v2_PHRS_UI_keyMatchFuzzy(phraseimp* pime, PHRS_UI input, int cmpLenIn) : pIme(pime), In(input), cmpLen(cmpLenIn)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( pIme->getMemData_CKeys(chk, wsKeys) ==0 )
            {
                 return false;
            }
            
            wstring
            wsPhrs;
            if ( pIme->getMemData_CPhrs(chk, wsPhrs) ==0 )
            {
                 return false;
            }
            
            if ( In.wsKeys ==wsKeys )
            {
                 if ( wsPhrs.size() >1 )
                 {
                      if ( wsPhrs.compare(2, cmpLen, In.wsPhrs) ==0 )
                      {
                           return true;
                      }
                 }
                 else
                 {
                      if ( wsPhrs.compare(0, cmpLen, In.wsPhrs) ==0 )
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
//
//-----------------------------------------------------------------------------
struct v2_PHRS_UI_keyMatch
{
       PHRS_UI               In;
       
       v2_PHRS_UI_keyMatch(PHRS_UI input) : In(input)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
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
bool phraseimp_v2::loadDict(const char* dictName)
{
     clear();
     
     FILE
     *fp =fopen(dictName, "rb");
     
     if ( fp )
     {
          char
          headSz[sizeof(HEAD_ID_DICT_V2) +1] ={0};
          
          fread(headSz, sizeof(HEAD_ID_DICT_V2) *sizeof(char), 1, fp);
          
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
          
          if ( sizePhrs >MAX_ALLOC_MEM || sizeKeys >MAX_ALLOC_MEM || sizeNext >MAX_ALLOC_MEM )
          {
               // alloc too big, impossible
               return false;
          }
          
          unsigned int idx;
          for ( idx =0; idx <vSize; idx++ )
          {
                V1_DICT_UI
                ux(maxPhrLen(), maxKeyLen(), maxNwdLen());
                
                fread(ux.bMem(), ux.bMemSz(), 1, fp);
                
                MEM_INT32
                iOffs_Phrs =*(MEM_INT32*)ux.wsWLen,
                iOffs_Keys =*(MEM_INT32*)ux.wsKLen;
                
                //-------------------------------------------------------------
                std::wstring
                wsPhrs,
                wsKeys;
                
                if ( getMemData(memPhrs.bmem(), iOffs_Phrs, wsPhrs) ==0 )
                {
                     return false;
                }
                
                if ( getMemData(memKeys.bmem(), iOffs_Keys, wsKeys) ==0 )
                {
                     return false;
                }
                
                MEM_INT32
                iOffs_NextWord =*(MEM_INT32*)ux.wsNLen;
                
                appendDirectly(idx, wsKeys, wsPhrs, iOffs_NextWord);
          }
          
          fread(memNext.bmem(), sizeNext, 1, fp);
          
          //-------------------------------------------------------------------
          // 2018/04/12
          //-------------------------------------------------------------------
          fclose(fp);
          
          qsortAndUnique();
          
          #if defined(__DEBUG_ORG_LOG_DATA_ORG)
          {
               CString
               fname =WfxCurrPath(_T("\\44-Thai-Data-Org.log"));
               
               CWfxFileW
               fx;
               fx.SaveHeader(fname);
               
               for ( PHRS_ITR ii =vdict.begin(); ii !=vdict.end(); ii++ )
               {
                     CString
                     CstrK =ii->wsKeys.c_str();
                     
                     for ( int cc =CstrK.GetLength(); cc <10; cc++ )
                     {
                           CstrK +=' ';
                     }
                     fx.Append(CstrK);
                     fx.Append(L",");
                     fx.AppendLn(ii->wsPhrs.c_str());
               }
          }
          #endif
          
          //-------------------------------------------------------------------
          //-------------------------------------------------------------------
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
struct v2_EN_target_Same_Key_ID_Strokes
{
       phraseimp_v2*         pIme;
       
       v2_EN_target_Same_Key_ID_Strokes(phraseimp_v2* pime) : pIme(pime)
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
struct v2_CH_target_Same_Key_Strokes_ID
{
       phraseimp_v2*         pIme;
       
       v2_CH_target_Same_Key_Strokes_ID(phraseimp_v2* pime) : pIme(pime)
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
struct v2_target_Same_Key_ID_Strokes
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            wstring
            wsStrokes1 =a.wsStrokes,
            wsStrokes2 =b.wsStrokes;
            
            #if defined(_WIN32) && defined(__DEBUG_MSG)
            //TRACE(_T("%ls,%ls\r\n"), wsPhrase1.c_str(),wsPhrase2.c_str());
            #endif
            
            int
            cmp =wcscmp(a.itr->wsKeys.c_str(), b.itr->wsKeys.c_str());
            
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
struct v2_target_Same_Key_Strokes_ID
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            wstring
            wsStrokes1 =a.wsStrokes,
            wsStrokes2 =b.wsStrokes;
            
            #if defined(_WIN32) && defined(__DEBUG_MSG)
            //TRACE(_T("%ls,%ls\r\n"), wsPhrase1.c_str(),wsPhrase2.c_str());
            #endif
            
            int
            cmp =wcscmp(a.itr->wsKeys.c_str(), b.itr->wsKeys.c_str());
            
            if ( cmp <0 )
            {
                 return true;
            }
            else if ( cmp ==0 )
            {
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
struct v2_target_Same_Key_Len
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            int
            sz01 =a.itr->wsKeys[0],
            sz02 =b.itr->wsKeys[0];
            
            // 1. 首碼優先
            if ( sz01 <sz02 )
                 return true;
            
            if ( sz01 ==sz02 )
            {
                 // 2. 再比 Key 長度
                 int
                 cmp =wcscmp(a.itr->wsKeys.c_str(), b.itr->wsKeys.c_str());
                 
                 if ( cmp <0 )
                      return true;
                 
                 else if ( cmp ==0 )
                 {
                      // 3. 再比頻率
                      if ( a.itr->sID <b.itr->sID )
                      {
                           return true;
                      }
                      else if ( a.itr->sID ==b.itr->sID )
                      {
                           // 4. 再比 Key 內容
                           int
                           cmp =wcscmp(a.wsStrokes.c_str(), b.wsKeys.c_str());
                           
                           if ( cmp <0 )
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
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp_v2::qsortPos()
{
     v1dpos.clear();
     v2klen.clear();
     
     for ( PHRS_ITR ii =vdict.begin(); ii !=vdict.end(); ii++ )
     {
           PHRS_V1_POS
           ex(ii);
           
           ex.wsStrokes.erase(0, 2);
           
           v1dpos.push_back(ex);
           
           #if defined(__USE_V2_KLEN)
           v2klen.push_back(ex);
           #endif
     }
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_ORG_LOG_DATA_ORG)
     {
          CString
          fname =WfxCurrPath(_T("\\44-Thai-Data-Org.log"));
          
          CWfxFileW
          fx;
          fx.SaveHeader(fname);
          
          for ( PHRS_ITR ii =vdict.begin(); ii !=vdict.end(); ii++ )
          {
                CString
                CstrK =ii->wsKeys.c_str();
                
                for ( int cc =CstrK.GetLength(); cc <10; cc++ )
                {
                      CstrK +=' ';
                }
                fx.Append(CstrK);
                fx.Append(L",");
                fx.AppendLn(ii->wsPhrs.c_str());
          }
     }
     #endif
     //------------------------------------------------------------------------
     
     if ( sortID ==SORT_BY_EN )
     {
          std::sort( v1dpos.begin(), v1dpos.end(), v2_target_Same_Key_ID_Strokes());
     }
     else
     {
          std::sort( v1dpos.begin(), v1dpos.end(), v2_target_Same_Key_Strokes_ID());
     }
     
     #if defined(__USE_V2_KLEN)
     std::sort( v2klen.begin(), v2klen.end(), v2_target_Same_Key_Len());
     #endif
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_ORG_LOG_DATA)
     {
          CString
          fname =WfxCurrPath(_T("\\44-Thai-Data-Order.log"));
          
          CWfxFileW
          fx;
          fx.SaveHeader(fname);
          
          for ( PHRS_V1_POS_ITR ii =v1dpos.begin(); ii !=v1dpos.end(); ii++ )
          {
                CString
                CstrK =ii->wsKeys.c_str();
                
                for ( int cc =CstrK.GetLength(); cc <10; cc++ )
                {
                      CstrK +=' ';
                }
                fx.Append(CstrK);
                fx.Append(L",");
                fx.AppendLn(ii->itr->wsPhrs.c_str());
          }
     }
     #endif
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_ORG_LOG_KLEN)
     {
          CString
          fname =WfxCurrPath(_T("\\44-Thai-KLen.log"));
          
          CWfxFileW
          fx;
          fx.SaveHeader(fname);
          
          for ( PHRS_V1_POS_ITR ii =v2klen.begin(); ii !=v2klen.end(); ii++ )
          {
                CString
                CstrK =ii->wsKeys.c_str();
                
                for ( int cc =CstrK.GetLength(); cc <10; cc++ )
                {
                      CstrK +=' ';
                }
                fx.Append(CstrK);
                fx.Append(L",");
                fx.AppendLn(ii->itr->wsPhrs.c_str());
          }
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
int  phraseimp_v2::getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut)
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
int  phraseimp_v2::getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
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
     
     V2_DICT_UI_ITR
     bi =v2dict.begin();
     
     #if defined(_WIN32) && defined(__DEBUG_MSG)
     //TRACE(_T("vdict2 size =%d\r\n"), v2dict.size());
     #endif
     
     while ( v.size() <(unsigned long)total )
     {
             V2_DICT_UI_ITR
             ui_found =find_if(bi, v2dict.end(), v2_PHRS_UI_keyMatch(ui));
             
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
struct v2_PHRS_UI_idiomKeyMatch
{
       PHRS_UI               In;
       
       v2_PHRS_UI_idiomKeyMatch(PHRS_UI input) : In(input)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
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
struct v2_IDIOM_UI_idiomKeyMatch
{
       IDIOM_UI              In;
       
       v2_IDIOM_UI_idiomKeyMatch(IDIOM_UI input) : In(input)
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
// Remarks      : first full match, then fuzzy (current not used)
//
//-----------------------------------------------------------------------------
int  phraseimp_v2::getIdiom(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
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
     getRangeItem =10;//(wcslen(wsKeys) ==1 ? 500 :vdict.size());
     
     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     while ( vdictGet.size() <getRangeItem )
     {
             IDIOM_ITR
             ui_found =find_if(bi, vnext.end(), v2_IDIOM_UI_idiomKeyMatch(ui));
             
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
struct v2_PHRS_UI_keyMatchGrouping_preload
{
       int                   cmpLen;
       PHRS_UI               In;
       
       v2_PHRS_UI_keyMatchGrouping_preload(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
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
// Remarks      : first full match, then fuzzy (current not used)
//
//-----------------------------------------------------------------------------
int  phraseimp_v2::getGrouping_preload(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
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
     ui_found =find_if(v2dgrp_preload.begin(), v2dgrp_preload.end(), v2_PHRS_UI_keyMatchGrouping_preload(ui, cmpLen));
     
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
struct v2_PHRS_UI_keyMatchGrouping_dynamic
{
       int                   cmpLen;
       PHRS_UI               In;
       
       v2_PHRS_UI_keyMatchGrouping_dynamic(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }
       
       bool operator()(const PHRS_V2_POS& chk) const
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
            cmp =wcsncmp(In.wsKeys.c_str(), wsKeys.c_str(), cmpLen);
            
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
int  phraseimp_v2::getGrouping_dynamic(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");
     
     if ( v2dict.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     V2_DICT_UI_ITR
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
     
     PHRS_V2_POS_ITR
     ui_found =find_if(v2dgrp_dynload.begin(), v2dgrp_dynload.end(), v2_PHRS_UI_keyMatchGrouping_dynamic(ui, cmpLen));
     
     vector<PHRS_UI>
     vdictGet;
     
     PHRS_V2_POS_ITR
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
int  phraseimp_v2::getGrouping(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     return getGrouping_dynamic(sID, wsKeyIn, v, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v2_PHRS_UI_CmpKeyLen
{
       PHRS_UI               In;
       phraseimp_v2*         pIme;
       
       v2_PHRS_UI_CmpKeyLen(phraseimp_v2* pime, PHRS_UI input) : pIme(pime), In(input)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
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
struct v2_target_cmpID
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
int  phraseimp_v2::getKeyFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
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
     cmpLen      =wcslen(wsKeyIn),
     cmpLenMatch =wcslen(wsKeyMatch);
     
     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :v2dict.size());
     
     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     set<wstring>
     vdictSet;
     
     vector<PHRS_UI>
     vdictGet;
     
     PHRS_UI
     ui(this, sID, wsKeyIn, wsKeyMatch);
     
     V2_DICT_UI_ITR
     ui_found =find_if(v2dict.begin(), v2dict.end(), v2_PHRS_UI_keyMatchFuzzy(this, ui, cmpLenMatch));
     
     int
     getTotal =0;
     
     if ( ui_found !=v2dict.end() )
     {
          V2_DICT_UI_ITR
          bi1 =ui_found -1;
          
          // 先找到首個 keys & phrase 都一樣的Index
          while ( ui_found !=v2dict.begin() )
          {
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
          while ( getTotal <total && ui_found !=v2dict.end() )
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
                  cmp =wsPhrs.compare(2, cmpLenMatch, wsKeyMatch);
                  
                  if ( cmp !=0 )
                  {
                       ui_found++;
                       continue;
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
     
     //------------------------------------------------------------------------
     // get match key all of rest
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;
     
     if ( getTotal <total )
     {
          V2_DICT_UI_ITR
          bi_next =v2dict.begin();
          
          while ( vdictGet.size() <getRangeItem )
          {
                  V2_DICT_UI_ITR
                  ui_found =find_if(bi_next, v2dict.end(), v2_PHRS_UI_CmpKeyLen(this, ui));
                  
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
                       bool
                       match =(cmpLen <=(int)wsKeys.size());
                       
                       if ( wsKeyMatch !=0 && wcslen(wsKeyMatch) >0 )
                       {
                            int
                            end =ui2.wsPhrs.find(wsKeyMatch, 2);
                            
                            match =(end !=std::string::npos) && end ==2;
                       }
                       
                       if ( match ==true )
                       {
                            vdictGet2.push_back(ui2);
                            vdictSet.insert(ui2.wsPhrs);
                            
                            #if defined(__DEBUG_MSG)
                            {
                                 wchar_t
                                 aa[1024] ={0};
                                 
                                 #if defined(_WIN32)
                                 swprintf(aa,       L"2, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                                 #else
                                 swprintf(aa, 1024, L"2, %ls, %ls\r\n", ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                                 #endif
                                 
                                 dw_pintf_ex(aa);
                            }
                            #endif
                       }
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
     // fill back data
     //------------------------------------------------------------------------
     fillData(vdictGet, v);
     
     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_target_cmpID());
     fillData(vdictGet2, v);
     
     #if defined(__APPLY_UNIQUE_RULE)
     
     v.erase(std::unique(v.begin(), v.end(), v2_target_equal()), v.end());
     
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
struct v2_PHRS_UI_keyMatch_Vpos
{
       PHRS_UI               In;
       
       v2_PHRS_UI_keyMatch_Vpos(PHRS_UI input) : In(input)
       {
       }
       
       bool operator()(const PHRS_V1_POS& chk) const
       {
            if ( chk.itr->wsKeys ==In.wsKeys )
            {
                 #if defined(_WIN32) && defined(__DEBUG_MSG)
                 
                 wchar_t
                 wsTmp[450] ={0};
                 wsprintf(wsTmp, _T("sID =%d, k=%ls, w=%ls\r\n"), chk.itr->sID, chk.itr->wsKeys.c_str(), chk.itr->wsPhrs.c_str());
                 
                 dw_pintf_ex(wsTmp);
                 
                 #endif
                 
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
struct v2_PHRS_UI_keyMatch_Key1
{
       PHRS_UI               In;
       int                   cmpLen;
       
       v2_PHRS_UI_keyMatch_Key1(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
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
// Remarks      : 
//
//-----------------------------------------------------------------------------
struct v2_PHRS_UI_keyMatchFuzzyLen
{
       PHRS_UI               In;
       int                   cmpLen;
       
       v2_PHRS_UI_keyMatchFuzzyLen(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }
       
       bool operator()(const PHRS_UI& chk) const
       {
            //return wcsncmp(chk.wsKeys.c_str(), In.wsKeys.c_str(), cmpLen) ==0;
            
            int
            pos =chk.wsKeys.find(In.wsKeys);
            
            if ( pos !=std::string::npos && pos ==0 )
            {
                 #if defined(_WIN32) && defined(__DEBUG_MSG)
                 
                 wchar_t
                 wsTmp[450] ={0};
                 wsprintf(wsTmp, _T("sID =%d, k=%ls, w=%ls\r\n"), chk.sID, chk.wsKeys.c_str(), chk.wsPhrs.c_str());
                 
                 dw_pintf_ex(wsTmp);
                 
                 #endif
                 
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
int  phraseimp_v2::getKeyFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");
     
     if ( vdict.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa,       L"vdict total =%d\r\n", vdict.size());
          #else
          swprintf(aa, 1024, L"vdict total =%d\r\n", vdict.size());
          #endif
          
          dw_pintf_ex(aa);
     }
     #endif
     
     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);
     
     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :vdict.size());
     
     set<wstring>
     vdictSet;
     
     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet;
     
     PHRS_UI
     ui(this, sID, wsKeyIn, L"");
     
     PHRS_V1_POS_VTR
     ui_found =find_if(v1dpos.begin(), v1dpos.end(), v2_PHRS_UI_keyMatch_Vpos(ui));
     
     int
     getTotal =0;
     
     // if found any
     if ( ui_found !=v1dpos.end() )
     {
          while ( getTotal <total && ui_found !=v1dpos.end() )
          {
                  PHRS_UI
                  ui2(this, ui_found->itr);
                  
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
                  if ( (int)ui_found->itr->wsKeys.size() !=cmpLen )
                  {
                       break;
                  }
                  
                  int
                  cmp =wcsncmp(ui.wsKeys.c_str(), ui_found->itr->wsKeys.c_str(), cmpLen);
                  
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
     
     #if defined(__USE_V2_KLEN)
     
     ui_found =find_if(v2klen.begin(), v2klen.end(), v2_PHRS_UI_keyMatch_Vpos(ui));
     
     // if found any
     if ( ui_found !=v1dpos.end() )
     {
          while ( getTotal <total && ui_found !=v2klen.end() )
          {
                  PHRS_UI
                  ui2(this, ui_found->itr);
                  
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
                  if ( (int)ui_found->itr->wsKeys.size() <cmpLen )
                  {
                       break;
                  }
                  
                  int
                  cmp =wcsncmp(ui.wsKeys.c_str(), ui_found->itr->wsKeys.c_str(), cmpLen);
                  
                  if ( cmp !=0 )
                  {
                       #if defined(_WIN32) && defined(__DEBUG_MSG)
                       
                       wchar_t
                       wsTmp[450] ={0};
                       wsprintf(wsTmp, _T("ik =%ls, sID =%d, k=%ls, w=%ls\r\n"), ui.wsKeys.c_str(), ui_found->itr->sID, ui_found->itr->wsKeys.c_str(), ui_found->itr->wsPhrs.c_str());
                       
                       dw_pintf_ex(wsTmp);
                       #endif
                       
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
     
     #else
     
     if ( getTotal <total )
     {
          vector<PHRS_UI>::iterator
          bi2 =vdict.begin();
          
          while ( vdictGet.size() <getRangeItem )
          {
                  PHRS_ITR
                  ui_found =find_if(bi2, vdict.end(), v2_PHRS_UI_keyMatchFuzzyLen(ui, cmpLen));
                  
                  if ( ui_found ==vdict.end() )
                  {
                       break;
                  }
                  
                  PHRS_UI
                  ui2(this, ui_found);
                  
                  #if defined(_WIN32) && defined(__DEBUG_MSG)
                  {
                       const int     sID =ui2.sID;
                       const wchar_t *p1 =ui2.wsKeys.c_str();
                       const wchar_t *p2 =ui2.wsPhrs.c_str();
                       
                       wchar_t
                       wsTmp[256] ={0};
                       wsprintf(wsTmp, _T("sID =%d, k=%ls, w=%ls\r\n"), sID, p1, p2);
                       
                       dw_pintf_ex(wsTmp);
                  }
                  #endif
                  
                  bool
                  ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();
                  
                  if ( ok ==true )
                  {
                       vdictGet2.push_back(ui2);
                       vdictSet.insert(ui2.wsPhrs);
                       
                       #if defined(__DEBUG_MSG)
                       {
                            wchar_t
                            aa[1024] ={0};
                            
                            #if defined(_WIN32)
                            swprintf(aa,       L"2, %d, %ls, %ls\r\n", ui2.sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                            #else
                            swprintf(aa, 1024, L"2, %d, %ls, %ls\r\n", ui2.sID, ui2.wsKeys.c_str(), ui2.wsPhrs.c_str());
                            #endif
                            
                            dw_pintf_ex(aa);
                       }
                       #endif
                  }
                  bi2 =ui_found +1;
          }
     }
     
     #endif
     
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
     std::sort(vdictGet.begin(), vdictGet.end(), v2_target_cmpID());
     fillData(vdictGet, v);
     
     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_target_cmpID());
     fillData(vdictGet2, v);
     
     #if defined(__APPLY_UNIQUE_RULE)
     
     v.erase(std::unique(v.begin(), v.end(), v2_target_equal()), v.end());
     
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
struct v2_PHRS_UI_fuzzyMatch
{
       PHRS_UI               In;
       
       v2_PHRS_UI_fuzzyMatch(PHRS_UI input) : In(input)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
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
int  phraseimp_v2::getFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
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
     
     V2_DICT_UI_ITR
     bi =v2dict.begin();
     
     while ( v.size() <(unsigned long)total )
     {
             V2_DICT_UI_ITR
             ui_found =find_if(bi, v2dict.end(), v2_PHRS_UI_fuzzyMatch(ui));
             
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
struct v2_PHRS_UI_keyMatchFuzzyExx
{
       PHRS_UI               In;
       
       v2_PHRS_UI_keyMatchFuzzyExx(PHRS_UI input) : In(input)
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
int  phraseimp_v2::getKeyWildc(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsWildcard, int total)
{
     setlocale( LC_ALL, "");
     
     if ( vdict.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa, L"vdict total =%d\r\n", vdict.size());
          #else
          swprintf(aa, 1024, L"vdict total =%d\r\n", vdict.size());
          #endif
          
          dw_pintf_ex(aa);
     }
     #endif
     
     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);
     
     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :vdict.size());
     
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
          PHRS_ITR
          bi_next =vdict.begin();
          
          while ( vdictGet2.size() <getRangeItem )
          {
                  PHRS_ITR
                  ui_found =find_if(bi_next, vdict.end(), v2_PHRS_UI_keyMatchFuzzyExx(ui));
                  
                  if ( ui_found ==vdict.end() )
                  {
                       break;
                  }
                  
                  PHRS_UI
                  ui2(this, ui_found->sID, ui_found->wsKeys.c_str(), ui_found->wsPhrs.c_str());
                  
                  #if defined(_WIN32) && defined(__DEBUG_MSG)
                  {
                       const wchar_t *p4 =ui.wsKeys.c_str();
                       const wchar_t *p5 =ui_found->wsKeys.c_str();
                       const wchar_t *p6 =ui_found->wsPhrs.c_str();
                       
                       wchar_t
                       wsTmp[450] ={0};
                       wsprintf(wsTmp, _T("%ls, %ls, %ls\r\n"), p4, p5, p6);
                       
                       //TRACE(wsTmp);
                  }
                  #endif
                  
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
                                  
                                  if ( wCH !=ui2.wsKeys[i] )
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
     
     std::sort(vdictGet2.begin(), vdictGet2.end(), v2_target_cmpID());
     fillData(vdictGet2, v);
     
     #if defined(__APPLY_UNIQUE_RULE)
     
     v.erase(std::unique(v.begin(), v.end(), v2_target_equal()), v.end());
     
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
int  phraseimp_v2::get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total, bool fuzzy)
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
     v.erase(std::unique(v.begin(), v.end(), v2_target_equal()), v.end());
     
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
bool phraseimp_v2::isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     return false;
}
