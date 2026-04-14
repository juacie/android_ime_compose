// phraseimp_v3.cpp: implementation of the phraseimp_v3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "string.h"
#include "phraseimp_v3.h"

#define __ENCODE_MSG
#define __APPLY_UNIQUE_RULE
//#define __DEBUG_ORG_LOG_KSID
//#define __DEBUG_ORG_LOG_OSID

#if defined(_DEBUG_LOG) || defined(__DEBUG_ORG_LOG_KSID) || defined(__DEBUG_ORG_LOG_OSID)
#include "WfxFile.h"
#endif

#include "debug_util.h"


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
phraseimp_v3::phraseimp_v3()
{
     mLastCH =0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
phraseimp_v3::~phraseimp_v3()
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
void phraseimp_v3::clear()
{
     v3dict.clear();
     v2dgrp_dynload.clear();
     v3dgrp_preload.clear();
     v3klen.clear();
     v3kSID.clear();
     v3oSID.clear();

     #if defined(__LINUX_BASE)
     v3dict.shrink_to_fit();
     v2dgrp_dynload.shrink_to_fit();
     v3dgrp_preload.shrink_to_fit();
     v3klen.shrink_to_fit();
     v3kSID.shrink_to_fit();
     v3oSID.shrink_to_fit();
     #endif

     phraseimp::clear();
     
     mLastCH =0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v3_target_equal
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
struct v3_PHRS_UI_keyMatchFuzzy
{
       PHRS_UI               In;
       int                   cmpLen;
       phraseimp*            pIme;
       
       v3_PHRS_UI_keyMatchFuzzy(phraseimp* pime, PHRS_UI input, int cmpLenIn) : pIme(pime), In(input), cmpLen(cmpLenIn)
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
struct v3_PHRS_UI_keyMatch
{
       PHRS_UI               In;
       
       v3_PHRS_UI_keyMatch(PHRS_UI input) : In(input)
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
bool phraseimp_v3::loadDict(const char* dictName)
{
     clear();
     
     FILE
     *fp =fopen(dictName, "rb");
     
     if ( fp )
     {
          char
          headSz[sizeof(HEAD_ID_DICT_V3) +1] ={0};
          fread(headSz, sizeof(HEAD_ID_DICT_V3) *sizeof(char), 1, fp);
          
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
          
          if ( sizePhrs >MAX_ALLOC_MEM || sizeKeys >MAX_ALLOC_MEM || sizeNext >MAX_ALLOC_MEM )
          {
               // alloc too big, impossible
               return false;
          }
          
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
          
          unsigned int idx;
          for ( idx =0; idx <vSize; idx++ )
          {
                V2_DICT_UI_LOAD
                u2Load;
                
                fread(&u2Load, sizeof(V2_DICT_UI_LOAD), 1, fp);
                
                V2_DICT_UI
                u2(u2Load);
                
                v3dict.push_back(u2);
                
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
          // 2018/04/12
          //-------------------------------------------------------------------
          vSize =0;
          fread(&vSize, sizeof(MEM_INT32), 1, fp);
          
          for ( idx =0; idx <vSize; idx++ )
          {
                MEM_INT32
                pos;
                
                fread(&pos, sizeof(MEM_INT32), 1, fp);
                
                V2_DICT_UI_ITR
                ii =v3dict.begin() +pos;
                
                PHRS_V2_POS
                ex;
                ex.itr =ii;

                #if defined(__FOR_NEW_SET)
                v2dgrp_dynload.insert(ex);
                #else
                v2dgrp_dynload.push_back(ex);
                #endif
          }
          
          //-------------------------------------------------------------------
          // 2018/6/21  read KeyLen
          //-------------------------------------------------------------------
          vSize =0;
          fread(&vSize, sizeof(MEM_INT32), 1, fp);
          
          for ( idx =0; idx <vSize; idx++ )
          {
                MEM_INT32
                pos;
                
                fread(&pos, sizeof(MEM_INT32), 1, fp);
                
                V2_DICT_UI_ITR
                ii =v3dict.begin() +pos;
                
                PHRS_V3_POS
                ex;
                ex.itr =ii;
                
                v3klen.push_back(ex);
          }
          
          //-------------------------------------------------------------------
          // 2018/6/21  read KeySID
          //-------------------------------------------------------------------
          vSize =0;
          fread(&vSize, sizeof(MEM_INT32), 1, fp);
          
          for ( idx =0; idx <vSize; idx++ )
          {
                MEM_INT32
                pos;
                
                fread(&pos, sizeof(MEM_INT32), 1, fp);
                
                V2_DICT_UI_ITR
                ii =v3dict.begin() +pos;
                
                PHRS_V3_POS
                ex;
                ex.itr =ii;
                
                v3kSID.push_back(ex);
          }
          
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
struct v3_EN_target_Same_Key_ID_Strokes
{
       phraseimp_v3*         pIme;
       
       v3_EN_target_Same_Key_ID_Strokes(phraseimp_v3* pime) : pIme(pime)
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
struct v3_CH_target_Same_Key_Strokes_ID
{
       phraseimp_v3*         pIme;
       
       v3_CH_target_Same_Key_Strokes_ID(phraseimp_v3* pime) : pIme(pime)
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
//
//-----------------------------------------------------------------------------
struct v3_target_cmpKeyID
{
       bool operator()( PHRS_V3_POS const &a, PHRS_V3_POS const &b) const
       {
            if ( a.itr->wKeys[0] <b.itr->wKeys[0] )
                 return true;
            
            else if ( a.itr->wKeys[0] ==b.itr->wKeys[0] )
            {
                 if ( a.itr->sID <b.itr->sID )
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
struct v3_target_cmpSID
{
       bool operator()( PHRS_V3_POS const &a, PHRS_V3_POS const &b) const
       {
            if ( a.itr->sID <b.itr->sID )
                 return true;
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
void phraseimp_v3::setupOSID()
{
     if ( v3oSID.size() ==0 )
     {
          for ( V2_DICT_UI_ITR ii =v3dict.begin(); ii !=v3dict.end(); ii++ )
          {
                // preLoad
                wstring
                wsKeys;
                if ( getMemData_Keys(ii, wsKeys) ==0 )
                {
                     break;
                }
                
                PHRS_V3_POS
                ex;
                ex.itr =ii;
                
                v3oSID.push_back(ex);
          }
          std::sort(v3oSID.begin(), v3oSID.end(), v3_target_cmpSID());
     }
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_ORG_LOG_OSID)
     {
          CString
          fname =WfxCurrPath(_T("\\70-Chinese-Data-osID.log"));
          
          CWfxFileW
          fx;
          fx.SaveHeader(fname);
          
          for ( PHRS_V3_POS_ITR ii =v3oSID.begin(); ii !=v3oSID.end(); ii++ )
          {
                wstring
                wsKeys;
                if ( getMemData_Keys(ii->itr, wsKeys) ==0 )
                {
                     break;
                }
                
                wstring
                wsPhrs;
                if ( getMemData_Phrs(ii->itr, wsPhrs) ==0 )
                {
                     break;
                }
                
                CString
                CstrK =wsKeys.c_str();
                
                for ( int cc =CstrK.GetLength(); cc <10; cc++ )
                {
                      CstrK +=' ';
                }
                
                CString
                CstrSID;
                CstrSID.Format(_T("%#6d"), ii->itr->sID);
                
                fx.Append(CstrSID);
                fx.Append(L"\t");
                fx.Append(L",");
                fx.Append(CstrK);
                fx.Append(L",");
                fx.AppendLn(wsPhrs.c_str());
          }
     }
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp_v3::qsortPos()
{
     if ( v3kSID.size() ==0 )
     {
          for ( V2_DICT_UI_ITR ii =v3dict.begin(); ii !=v3dict.end(); ii++ )
          {
                // preLoad
                wstring
                wsKeys;
                if ( getMemData_Keys(ii, wsKeys) ==0 )
                {
                     break;
                }
                
                PHRS_V3_POS
                ex;
                ex.itr =ii;
                
                v3kSID.push_back(ex);
          }
          std::sort(v3kSID.begin(), v3kSID.end(), v3_target_cmpKeyID());
     }
     
     v3kSID_begin =v3kSID.begin(),
     v3kSID_end   =v3kSID.end();
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_ORG_LOG_KSID)
     {
          CString
          fname =WfxCurrPath(_T("\\70-Chinese-Data-ksID.log"));
          
          CWfxFileW
          fx;
          fx.SaveHeader(fname);
          
          for ( PHRS_V3_POS_ITR ii =v3kSID.begin(); ii !=v3kSID.end(); ii++ )
          {
                wstring
                wsKeys;
                if ( getMemData_Keys(ii->itr, wsKeys) ==0 )
                {
                     break;
                }
                
                wstring
                wsPhrs;
                if ( getMemData_Phrs(ii->itr, wsPhrs) ==0 )
                {
                     break;
                }
                
                CString
                CstrK =wsKeys.c_str();
                
                for ( int cc =CstrK.GetLength(); cc <10; cc++ )
                {
                      CstrK +=' ';
                }
                fx.Append(CstrK);
                fx.Append(L",");
                fx.AppendLn(wsPhrs.c_str());
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
int  phraseimp_v3::getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut)
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
int  phraseimp_v3::getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");
     
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa, L"v3dict.size() =%d\r\n", v3dict.size());
          #else
          swprintf(aa, 1024, L"v3dict.size() =%d\r\n", v3dict.size());
          #endif
          
          dw_pintf_ex(aa);
     }
     #endif
     
     PHRS_UI
     ui(this, sID, wsKeys, (wchar_t *)L"");
     
     V2_DICT_UI_ITR
     bi =v3dict.begin();
     
     #if defined(_WIN32) && defined(__DEBUG_MSG)
     //TRACE(_T("vdict2 size =%d\r\n"), v3dict.size());
     #endif
     
     while ( v.size() <(unsigned long)total )
     {
             V2_DICT_UI_ITR
             ui_found =find_if(bi, v3dict.end(), v3_PHRS_UI_keyMatch(ui));
             
             if ( ui_found ==v3dict.end() )
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
struct v3_PHRS_UI_idiomKeyMatch
{
       PHRS_UI               In;
       
       v3_PHRS_UI_idiomKeyMatch(PHRS_UI input) : In(input)
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
struct v3_IDIOM_UI_idiomKeyMatch
{
       IDIOM_UI              In;
       
       v3_IDIOM_UI_idiomKeyMatch(IDIOM_UI input) : In(input)
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
int  phraseimp_v3::getIdiom(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
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
             ui_found =find_if(bi, vnext.end(), v3_IDIOM_UI_idiomKeyMatch(ui));
             
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
struct v3_PHRS_UI_keyMatchGrouping_preload
{
       int                   cmpLen;
       PHRS_UI               In;
       
       v3_PHRS_UI_keyMatchGrouping_preload(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
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
int  phraseimp_v3::getGrouping_preload(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");
     
     if ( v3dgrp_preload.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     PHRS_GRP_ITR
     bi =v3dgrp_preload.begin();
     
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa, L"v3dict total =%d\r\n", v3dgrp_preload.size());
          #else
          swprintf(aa, 1024, L"v3dict total =%d\r\n", v3dgrp_preload.size());
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
     ui_found =find_if(v3dgrp_preload.begin(), v3dgrp_preload.end(), v3_PHRS_UI_keyMatchGrouping_preload(ui, cmpLen));
     
     vector<PHRS_UI>
     vdictGet;
     
     while ( ui_found !=v3dgrp_preload.end() )
     {
             PHRS_GRP_ITR
             bi1 =ui_found;
             
             if ( ui.wsKeys.compare(0, cmpLen, bi1->wsKeys) )
             {
                  break;
             }
             
             PHRS_UI
             ui2(this, bi1->sID, bi1->wsKeys.c_str(), bi1->wsPhrs.c_str(), INVALID_MEM_OFFSET);
             
             #if defined(_WIN32) && defined(__DEBUG_MSG)
             //TRACE(_T("ui wsK =%ls, keyin wsK =%ls\r\n"), ui2.wsKeys.c_str(), bi1->wsKeys.c_str());
             #endif
             
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
struct v3_PHRS_UI_keyMatchGrouping_dynamic
{
       int                   cmpLen;
       PHRS_UI               In;
       
       v3_PHRS_UI_keyMatchGrouping_dynamic(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }
       
       bool operator()(const PHRS_V2_POS& chk) const
       {
            #if defined(_WIN32) && defined(__DEBUG_MSG_DATA)
            //TRACE(_T("In =%s, Chk =%s\r\n"), In.wsKeys.c_str(), chk.itr->wsKeys.c_str());
            #endif
            
            wstring
            wsKeys;
            if ( In.pIme->getMemData_Keys(chk.itr->iKeys, wsKeys) ==0 )
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
int  phraseimp_v3::getGrouping_dynamic(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");
     
     if ( v3dict.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     V2_DICT_UI_ITR
     bi =v3dict.begin();
     
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa, L"v3dict total =%d\r\n", v3dict.size());
          #else
          swprintf(aa, 1024, L"v3dict total =%d\r\n", v3dict.size());
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
     ui_found =find_if(v2dgrp_dynload.begin(), v2dgrp_dynload.end(), v3_PHRS_UI_keyMatchGrouping_dynamic(ui, cmpLen));
     
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
int  phraseimp_v3::getGrouping(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     return getGrouping_dynamic(sID, wsKeyIn, v, total);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v3_PHRS_UI_CmpKeyLen
{
       PHRS_UI               In;
       phraseimp_v3*         pIme;
       
       v3_PHRS_UI_CmpKeyLen(phraseimp_v3* pime, PHRS_UI input) : pIme(pime), In(input)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( pIme->getMemData_CKeys(chk, wsKeys) !=0 )
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
//
//-----------------------------------------------------------------------------
struct v3_target_cmpID
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
int  phraseimp_v3::getKeyFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     setlocale( LC_ALL, "");
     
     if ( v3dict.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa, L"v3dict total =%d\r\n", v3dict.size());
          #else
          swprintf(aa, 1024, L"v3dict total =%d\r\n", v3dict.size());
          #endif
          
          dw_pintf_ex(aa);
     }
     #endif
     
     //-----------------------------------------------------------------------
     int
     cmpLen      =wcslen(wsKeyIn),
     cmpLenMatch =wcslen(wsKeyMatch);
     
     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :v3dict.size());
     
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
     ui_found =find_if(v3dict.begin(), v3dict.end(), v3_PHRS_UI_keyMatchFuzzy(this, ui, cmpLenMatch));
     
     int
     getTotal =0;
     
     if ( ui_found !=v3dict.end() )
     {
          V2_DICT_UI_ITR
          bi1 =ui_found -1;
          
          // 先找到首個 keys & phrase 都一樣的Index
          while ( ui_found !=v3dict.begin() )
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
          while ( getTotal <total && ui_found !=v3dict.end() )
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
          bi_next =v3dict.begin();
          
          while ( vdictGet.size() <getRangeItem )
          {
                  V2_DICT_UI_ITR
                  ui_found =find_if(bi_next, v3dict.end(), v3_PHRS_UI_CmpKeyLen(this, ui));
                  
                  if ( ui_found ==v3dict.end() )
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
     
     std::sort(vdictGet2.begin(), vdictGet2.end(), v3_target_cmpID());
     fillData(vdictGet2, v);
     
     #if defined(__APPLY_UNIQUE_RULE)
     
     v.erase(std::unique(v.begin(), v.end(), v3_target_equal()), v.end());
     
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
struct v3_PHRS_UI_keyMatch_Vpos
{
       PHRS_UI               In;
       
       v3_PHRS_UI_keyMatch_Vpos(PHRS_UI input) : In(input)
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
            
            return wsKeys ==In.wsKeys;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v3_PHRS_UI_keyMatch_Key1
{
       PHRS_UI               In;
       int                   cmpLen;
       
       v3_PHRS_UI_keyMatch_Key1(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
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
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v3::getKeyFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");
     
     if ( v3dict.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa,       L"v3dict total =%d\r\n", v3dict.size());
          #else
          swprintf(aa, 1024, L"v3dict total =%d\r\n", v3dict.size());
          #endif
          
          dw_pintf_ex(aa);
     }
     #endif
     
     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);
     
     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :v3dict.size());
     
     set<wstring>
     vdictSet;
     
     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet;
     
     PHRS_UI
     ui(this, sID, wsKeyIn, L"");
     
     V2_DICT_UI_ITR
     ui_found =find_if(v3dict.begin(), v3dict.end(), v3_PHRS_UI_keyMatch_Vpos(ui));
     
     int
     getTotal =0;
     
     // if found any
     if ( ui_found !=v3dict.end() )
     {
          while ( getTotal <total && ui_found !=v3dict.end() )
          {
                  V2_DICT_UI_ITR
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
               find_if(v3dict.begin(), v3dict.end(), v3_PHRS_UI_keyMatch_Key1(ui, cmpLen));
          }
          
          // if found any
          if ( ui_found !=v3dict.end() )
          {
               while ( getTotal <total2 && ui_found !=v3dict.end() )
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
     std::sort(vdictGet.begin(), vdictGet.end(), v3_target_cmpID());
     fillData(vdictGet, v);
     
     std::sort(vdictGet2.begin(), vdictGet2.end(), v3_target_cmpID());
     fillData(vdictGet2, v);
     
     #if defined(__APPLY_UNIQUE_RULE)
     
     v.erase(std::unique(v.begin(), v.end(), v3_target_equal()), v.end());
     
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
struct v3_PHRS_UI_MatchLen_By_KLen
{
       PHRS_UI               In;
       bool                  isWildcard;
       wstring               wsWildcard;
       
       v3_PHRS_UI_MatchLen_By_KLen(PHRS_UI _input, bool _isWildcard, wstring _wsWildcard) : In(_input), isWildcard(_isWildcard), wsWildcard(_wsWildcard)
       {
       }
       
       bool operator()(const PHRS_V3_POS& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_Keys(chk.itr, wsKeys) ==0 )
            {
                 return false;
            }
            
            wstring
            wsPhrs;
            if ( In.pIme->getMemData_Phrs(chk.itr, wsPhrs) ==0 )
            {
                 return false;
            }
            
            if ( isWildcard ==false )
            {
                 return wsKeys ==In.wsKeys;
            }
            if ( wsKeys.size() !=In.wsKeys.size() )
                 return false;
            
            for ( size_t i =0; i <wsKeys.size(); i++ )
            {
                  if ( In.wsKeys[0] ==wsWildcard[0] )
                       return true;
                  
                  if ( In.wsKeys[i] ==wsWildcard[0] )
                       continue;
                  
                  if ( wsKeys[i] !=In.wsKeys[i] )
                       return false;
            }
            return true;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v3_PHRS_UI_MatchLen_By_KLen_Begin
{
       PHRS_UI               In;
       
       v3_PHRS_UI_MatchLen_By_KLen_Begin(PHRS_UI _input) : In(_input)
       {
       }
       
       bool operator()(const PHRS_V3_POS& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_Keys(chk.itr, wsKeys) ==0 )
            {
                 return false;
            }
            return wsKeys[0] ==In.wsKeys[0];
       }
};


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v3_PHRS_UI_MatchLen_By_KLen_End
{
       PHRS_UI               In;
       
       v3_PHRS_UI_MatchLen_By_KLen_End(PHRS_UI _input) : In(_input)
       {
       }
       
       bool operator()(const PHRS_V3_POS& chk) const
       {
            wstring
            wsKeys;
            if ( In.pIme->getMemData_Keys(chk.itr, wsKeys) ==0 )
            {
                 return false;
            }
            return wsKeys[0] !=In.wsKeys[0];
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v3_PHRS_UI_MatchLen_By_Data
{
       PHRS_UI               In;
       phraseimp_v3*         pIme;
       
       v3_PHRS_UI_MatchLen_By_Data(phraseimp_v3* _pIme, PHRS_UI _input) : pIme(_pIme), In(_input)
       {
       }
       
       bool operator()(const V2_DICT_UI& chk) const
       {
            wstring
            wsKeys;
            if ( pIme->getMemData_CKeys(chk, wsKeys) !=0 )
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
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
static
void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
{
     if ( from.empty() )
          return;
     
     size_t start_pos = 0;
     
     while ( (start_pos = str.find(from, start_pos)) != std::string::npos)
     {
             str.replace(start_pos, from.length(), to);
             start_pos += to.length();
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
void phraseimp_v3::setupBeginEnd(const wchar_t *wsKeyIn, const wchar_t *wsWildcard)
{
     if ( mLastCH !=wsKeyIn[0] )
     {
          mLastCH =wsKeyIn[0];
          
          v3kSID_begin =v3kSID.begin(),
          v3kSID_end   =v3kSID.end();
          
          wstring
          wsCH =wsKeyIn;
          wsCH.resize(1);
          
          PHRS_UI
          ui2(this, 0, wsCH, L"");
          
          //-------------------------------------------------------------------
          PHRS_V3_POS_ITR
          ui_found_begin =find_if(v3kSID.begin(), v3kSID.end(), v3_PHRS_UI_MatchLen_By_KLen_Begin(ui2));
          
          if ( ui_found_begin !=v3kSID.end() )
          {
               while ( 1 )
               {
                       PHRS_V3_POS_ITR
                       ui_found =find_if(v3kSID.begin(), ui_found_begin, v3_PHRS_UI_MatchLen_By_KLen_Begin(ui2));
                       
                       if ( ui_found ==ui_found_begin )
                       {
                            break;
                       }
                       ui_found_begin =ui_found;
               }
               v3kSID_begin =ui_found_begin;
          }
          
          //-------------------------------------------------------------------
          PHRS_V3_POS_ITR
          ui_found_end =find_if(v3kSID_begin, v3kSID_end, v3_PHRS_UI_MatchLen_By_KLen_End(ui2));
          
          if ( ui_found_end !=v3kSID_end )
          {
               while ( 1 )
               {
                       PHRS_V3_POS_ITR
                       ui_found =find_if(v3kSID_begin, ui_found_end, v3_PHRS_UI_MatchLen_By_KLen_End(ui2));
                       
                       if ( ui_found ==ui_found_end )
                       {
                            break;
                       }
                       ui_found_end =ui_found;
               }
               v3kSID_end =ui_found_end;
          }
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v3::getKeyWildc(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsWildcard, int total)
{
     setlocale( LC_ALL, "");
     
     if ( v3dict.size() ==0 )
     {
          return 0;
     }
     
     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa, L"v3dict total =%d\r\n", v3dict.size());
          #else
          swprintf(aa, 1024, L"v3dict total =%d\r\n", v3dict.size());
          #endif
          
          dw_pintf_ex(aa);
     }
     #endif
     
     //------------------------------------------------------------------------
     int
     cmpLen =wcslen(wsKeyIn);
     
     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :v3dict.size());
     
     set<wstring>
     vdictSet;
     
     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet;
     
     PHRS_UI
     ui(this, sID, wsKeyIn, L"");
     
     bool
     isWildcard =wcsstr((wchar_t*)wsKeyIn, L"6") !=0;
     
     PHRS_V3_POS_ITR
     ui_found =find_if(v3klen.begin(), v3klen.end(), v3_PHRS_UI_MatchLen_By_KLen(ui, isWildcard, wsWildcard));
     
     int
     wcLen    =wcslen(wsWildcard),
     getTotal =0;
     
     // if found any
     if ( ui_found !=v3klen.end() )
     {
          PHRS_V3_POS_ITR
          bi1 =ui_found -1;
          
          // 先找到首個等長
          while ( ui_found !=v3klen.begin() )
          {
                  wstring
                  wsKeys;
                  
                  if ( getMemData_Keys(bi1->itr, wsKeys) ==0 )
                  {
                       break;
                  }
                  if ( (int)wsKeys.size() !=cmpLen )
                  {
                       break;
                  }
                  ui_found =bi1--;
          }
          
          // 再往下找
          while ( getTotal <total && ui_found !=v3klen.end() )
          {
                  PHRS_V3_POS_ITR
                  bi1 =ui_found;
                  
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
                  
                  //-----------------------------------------
                  // Must be equal to Input Key Length
                  //-----------------------------------------
                  if ( (int)wsKeys.size() !=cmpLen )
                  {
                       break;
                  }
                  
                  int
                  cmp =0;
                  
                  if ( wcLen ==0 )
                  {
                       //cmp =wcsncmp(wsKeyIn, wsKeys.c_str(), cmpLen);
                       cmp =wcscmp(wsKeyIn, wsKeys.c_str());
                  }
                  else
                  {
                       for ( int i =0; i <cmpLen; i++ )
                       {
                             wchar_t
                             wCH =wsKeyIn[i];
                             
                             if ( wCH ==wsWildcard[0] )
                             {
                                  continue;
                             }
                             if ( wCH !=wsKeys[i] )
                             {
                                  cmp =-1;
                                  break;
                             }
                       }
                  }
                  
                  if ( cmp !=0 )
                  {
                       ui_found++;
                       continue;
                  }
                  
                  vdictGet.push_back(ui2);
                  vdictSet.insert(ui2.wsPhrs);
                  
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
                  
                  getTotal++;
                  ui_found++;
          }
     }
     
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;
     
     //------------------------------------------------------------------------
     // 如果第一個字是6. 就不能走v3kSID, 而必須用原始已經排好順序的資料
     //------------------------------------------------------------------------
     if ( wsKeyIn[0] ==wsWildcard[0] )
     {
          setupOSID();

          PHRS_V3_POS_ITR
          bi_next =v3oSID.begin();
          
          while ( getTotal <total && bi_next !=v3oSID.end() )
          {
                  wstring
                  wsKeys;
                  if ( getMemData_Keys(bi_next->itr, wsKeys) ==0 )
                  {
                       break;
                  }
                  wstring
                  wsPhrs;
                  if ( getMemData_Phrs(bi_next->itr, wsPhrs) ==0 )
                  {
                       break;
                  }
                  
                  PHRS_UI
                  ui2(this, bi_next->itr->sID, bi_next->itr->wKeys.c_str(), bi_next->itr->wPhrs.c_str());
                  
                  #if defined(_WIN32) && defined(__DEBUG_MSG)
                  {
                       const wchar_t *p4 =ui2.wsKeys.c_str();
                       const wchar_t *p0 =bi_next->wsKeys.c_str();
                       const wchar_t *p5 =bi_next->itr->wKeys.c_str();
                       const wchar_t *p6 =bi_next->itr->wPhrs.c_str();
                       
                       wchar_t
                       wsTmp[450] ={0};
                       wsprintf(wsTmp, _T("sID =%d, %ls, %ls, %ls, %ls\r\n"), bi_next->itr->sID, p0, p4, p5, p6);
                       
                       TRACE(wsTmp);
                  }
                  #endif
                  
                  if ( (int)bi_next->itr->wKeys.size() <cmpLen )
                  {
                       bi_next++;
                       continue;
                  }
                  
                  int
                  cmp =0;
                  
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
                  if ( cmp !=0 )
                  {
                       bi_next++;
                       continue;
                  }
                  
                  bool
                  ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();
                  
                  if ( ok ==true )
                  {
                       getTotal++;
                       vdictGet2.push_back(ui2);
                       vdictSet.insert(ui2.wsPhrs);
                  }
                  bi_next++;
          }
     }
     else
     {
          if ( getTotal <total )
          {
               setupBeginEnd(wsKeyIn, wsWildcard);
               
               if ( v3kSID_begin ==v3kSID_end )
               {
                    v3kSID_begin =v3kSID.begin();
                    v3kSID_end   =v3kSID.end();
               }
               
               PHRS_V3_POS_ITR
               bi_nextEx =v3kSID_begin;
               
               while ( getTotal <total && bi_nextEx !=v3kSID_end )
               {
                       V2_DICT_UI_ITR
                       bi_next =bi_nextEx->itr;
                       
                       wstring
                       wsKeys;
                       if ( getMemData_Keys(bi_next, wsKeys) ==0 )
                       {
                            break;
                       }
                       if ( wsKeys[0] !=wsKeyIn[0] && wsKeyIn[0] !=wsWildcard[0] )
                       {
                            break;
                       }
                       if ( (int)wsKeys.size() <cmpLen )
                       {
                            bi_nextEx++;
                            continue;
                       }
                       
                       wstring
                       wsPhrs;
                       if ( getMemData_Phrs(bi_next, wsPhrs) ==0 )
                       {
                            break;
                       }
                       
                       #if defined(_WIN32) && defined(__DEBUG_MSG_KOG)
                       TRACE(_T("sID =%d, Phrs =%ls, Keys =%ls\r\n"), bi_next->sID, wsPhrs.c_str(), wsKeys.c_str());
                       #endif
                       
                       PHRS_UI
                       ui2(this, bi_next->sID, wsKeys.c_str(), wsPhrs.c_str());
                       
                       int
                       cmp =0;
                       
                       if ( isWildcard ==0 )
                       {
                            cmp =wcsncmp(wsKeyIn, wsKeys.c_str(), cmpLen);
                       }
                       else
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
                            bi_nextEx++;
                            continue;
                       }
                       bool
                       ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();
                       
                       if ( ok ==true )
                       {
                            getTotal++;
                            vdictGet2.push_back(ui2);
                            vdictSet.insert(ui2.wsPhrs);
                       }
                       // find next
                       bi_nextEx++;
               }
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
     std::sort(vdictGet.begin(), vdictGet.end(), v3_target_cmpID());
     fillData(vdictGet, v);
     
     std::sort(vdictGet2.begin(), vdictGet2.end(), v3_target_cmpID());
     fillData(vdictGet2, v);
     
     #if defined(__APPLY_UNIQUE_RULE)
     
     v.erase(std::unique(v.begin(), v.end(), v3_target_equal()), v.end());
     
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
struct v3_PHRS_UI_fuzzyMatch
{
       PHRS_UI               In;
       
       v3_PHRS_UI_fuzzyMatch(PHRS_UI input) : In(input)
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
int  phraseimp_v3::getFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");
     
     if ( v3dict.size() ==0 )
     {
          return 0;
     }
     
     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};
          
          #if defined(_WIN32)
          swprintf(aa, L"v3dict.size() =%d\r\n", v3dict.size());
          #else
          swprintf(aa, 1024, L"v3dict.size() =%d\r\n", v3dict.size());
          #endif
          
          dw_pintf_ex(aa);
     }
     #endif
     
     PHRS_UI
     ui(this, sID, wsKeys, (wchar_t *)L"");
     
     V2_DICT_UI_ITR
     bi =v3dict.begin();
     
     while ( v.size() <(unsigned long)total )
     {
             V2_DICT_UI_ITR
             ui_found =find_if(bi, v3dict.end(), v3_PHRS_UI_fuzzyMatch(ui));
             
             if ( ui_found ==v3dict.end() )
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
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp_v3::get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total, bool fuzzy)
{
     if ( v3dict.size() ==0 )
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
     v.erase(std::unique(v.begin(), v.end(), v3_target_equal()), v.end());
     
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
bool phraseimp_v3::isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     return false;
}
