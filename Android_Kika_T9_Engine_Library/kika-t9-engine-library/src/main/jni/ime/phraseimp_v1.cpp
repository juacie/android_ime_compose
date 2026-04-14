// phraseimp_v1.cpp: implementation of the phraseimp_v1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "string.h"
#include "phraseimp_v1.h"

// #define __DEBUG_MSG
// #define __DEBUG_MSG_EX

#define __ENCODE_MSG
#define __APPLY_UNIQUE_RULE
//#define __DEBUG_ORG_LOG_DATA
//#define __DEBUG_ORG_LOG_VPOS
//#define __DEBUG_ORG_LOG_KLEN

#if defined(_DEBUG_LOG) || defined(__DEBUG_ORG_LOG_DATA) || defined(__DEBUG_ORG_LOG_VPOS) || defined(__DEBUG_ORG_LOG_KLEN)
#include "WfxFile.h"
#endif

#include "debug_util.h"

#if defined(__FOR_HUAWEI)
#include "imebase.h"
extern imebase               gImeX;
#endif // defined

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
phraseimp_v1::phraseimp_v1()
{
     mWKLen   =false;
     mLastCH =0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
phraseimp_v1::~phraseimp_v1()
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
void phraseimp_v1::clear()
{
     udict.clear();
     v1wlen.clear();
     v1dgrp.clear();
     v1dpos.clear();

     #if defined(__LINUX_BASE)
     v1wlen.shrink_to_fit();
     v1dgrp.shrink_to_fit();
     v1dpos.shrink_to_fit();
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
struct v1_target_equal
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
struct v1_PHRS_UI_keyMatch
{
       PHRS_UI               In;

       v1_PHRS_UI_keyMatch(PHRS_UI input) : In(input)
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
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp_v1::loadDict(const char* dictName)
{
     clear();

     FILE
     *fp =fopen(dictName, "rb");

     if ( fp )
     {
          unsigned short
          vSize =0;

          fread(&vSize, sizeof(unsigned short), 1, fp);

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
          NxtSize =0;

          fread(&NxtSize, sizeof(MEM_INT32), 1, fp);

          if ( NxtSize >MAX_ALLOC_MEM )
          {
               // alloc too big, impossible
               return false;
          }
          else if ( NxtSize >0 )
          {
               if ( memNext.alloc((NxtSize +1) *sizeof(MEM_INT)) ==false )
               {
                    return false;
               }
          }

          for ( unsigned int idx =0; idx <vSize; idx++ )
          {
                V1_DICT_UI
                ux(maxPhrLen(), maxKeyLen(), maxNwdLen());

                fread(ux.bMem(), ux.bMemSz(), 1, fp);

                int vLen1 =0;
                int vLen2 =0;
                int vLen3 =sizeof(MEM_INT32);

                for ( vLen1 =0; ux.wsWLen[vLen1]; vLen1++ );
                for ( vLen2 =0; ux.wsKLen[vLen2]; vLen2++ );

                //-------------------------------------------------------------
                // first is KLen
                //-------------------------------------------------------------
                wchar_t
                key[MAX_DICT_KLEN] ={0};
                _wcsu16_utf162wcs(ux.wsKLen, key, vLen2 +1);

                #if defined(__DEBUG_MSG2)
                {
                     for ( int idx =0; idx <vLen2; idx++ )
                     {
                           #if defined(_WIN32)
                           //TRACE(L"%d =%x\r\n", idx, ux.wsKLen[idx]);
                           #else
                           printf("%d =%x\r\n", idx, ux.wsKLen[idx]);
                           #endif
                     }
                }
                #endif

                //-------------------------------------------------------------
                // second is Word
                //-------------------------------------------------------------
                wchar_t
                phs[MAX_DICT_KLEN] ={0};
                _wcsu16_utf162wcs(ux.wsWLen, phs, vLen1 +1);

                #if defined(__DEBUG_MSG)
                {
                     wchar_t
                     aa[1024] ={0};

                     #if defined(_WIN32)
                     swprintf(aa, L"key =%ls, phs =%ls\r\n", key, phs);
                     #else
                     swprintf(aa, 1024, L"key =%ls, phs =%ls\r\n", key, phs);
                     #endif

                     dw_pintf_ex(aa);
                }
                #endif

                //-------------------------------------------------------------
                std::wstring
                wsKeys =encryptDecrypt(key, ENC_KEY_SZ),
                wsPhrs =encryptDecrypt(phs, ENC_KEY_SZ);

                #if defined(__DEBUG_MSG2)
                {
                     wchar_t
                     aa[1024] ={0};

                     #if defined(_WIN32)
                     swprintf(aa, L"wsKeys =%ls, wsPhrs= %ls\r\n", wsKeys.c_str(), wsPhrs.c_str());
                     #else
                     swprintf(aa, 1024, L"wsKeys =%ls, wsPhrs= %ls\r\n", wsKeys.c_str(), wsPhrs.c_str());
                     #endif

                     dw_pintf_ex(aa);
                }
                #endif

                MEM_INT32
                iNext =*(MEM_INT32*)ux.wsNLen;

                //TRACE(_T("x=%ls\r\n"), wsKeys.c_str());
                //TRACE(_T("x=%ls\r\n"), wsPhrs.c_str());

                appendDirectly(idx, wsKeys, wsPhrs, iNext);
          }

          //-------------------------------------------------------------------
          // 2018/04/02
          //-------------------------------------------------------------------
          fread(memNext.bmem(), NxtSize *sizeof(MEM_INT), 1, fp);

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
struct v1_target_Same_Key_ID_Strokes
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
struct v1_target_Same_Key_Strokes_ID
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
struct v1_target_Same_Key_Len_Wildcard
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            int
            sz01 =a.itr->wsKeys[0],
            sz02 =b.itr->wsKeys[0];

            //if ( sz01 <sz02 )
            //     return true;

            //if ( sz01 ==sz02 )
            {
                 int
                 sz1 =a.itr->wsKeys.size(),
                 sz2 =b.itr->wsKeys.size();

                 // 2. 再比 Key 長度
                 if ( sz1 <sz2 )
                 {
                      return true;
                 }
                 else if ( sz1 ==sz2 )
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
                           cmp =wcscmp(a.itr->wsKeys.c_str(), b.itr->wsKeys.c_str());

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
void phraseimp_v1::qsortPos()
{
     v1dgrp.clear();
     v1dpos.clear();
     v1wlen.clear();

     for ( PHRS_ITR ii =vdict.begin(); ii !=vdict.end(); ii++ )
     {
           PHRS_V1_POS
           ex;
           ex.itr       =ii;
           ex.wsKeys    =ii->wsKeys;
           ex.wsStrokes =ii->wsPhrs;

           ex.wsStrokes.erase(0, 2);

           #if defined(__FOR_NEW_SET)
           v1dgrp.insert(ex);
           #else
           v1dgrp.push_back(ex);
           #endif

           v1dpos.push_back(ex);

           if ( mWKLen ==true )
           {
                v1wlen.push_back(ex);
           }
     }

     if ( sortID ==SORT_BY_EN )
     {
          std::sort( v1dpos.begin(), v1dpos.end(), v1_target_Same_Key_ID_Strokes());
     }
     else
     {
          std::sort( v1dpos.begin(), v1dpos.end(), v1_target_Same_Key_Strokes_ID());
     }

     #if !defined(__FOR_NEW_SET)
     std::sort( v1dgrp.begin(), v1dgrp.end(), v1_target_Same_Key_Strokes_ID());
     v1dgrp.erase(std::unique(v1dgrp.begin(), v1dgrp.end()), v1dgrp.end());
     #endif

     std::sort( v1wlen.begin(), v1wlen.end(), v1_target_Same_Key_Len_Wildcard());

     //------------------------------------------------------------------------
     #if defined(__DEBUG_ORG_LOG_DATA)
     {
          CString
          fname =WfxCurrPath(_T("\\70-Chinese_HK-Data.log"));

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
     #if defined(__DEBUG_ORG_LOG_VPOS)
     {
          CString
          fname =WfxCurrPath(_T("\\70-Chinese_HK-VPos.log"));

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
          fname =WfxCurrPath(_T("\\70-Chinese_HK-KLen.log"));

          CWfxFileW
          fx;
          fx.SaveHeader(fname);

          for ( PHRS_V1_POS_ITR ii =v1wlen.begin(); ii !=v1wlen.end(); ii++ )
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
// Remarks      :
//
//-----------------------------------------------------------------------------
void phraseimp_v1::clear_u()
{
     udict.clear();
}

//-----------------------------------------------------------------------------
bool phraseimp_v1::checkLimit_u(int limit)
{
     while ( (int)udict.size() >=limit )
     {
             udict.erase(--udict.end());
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
bool phraseimp_v1::save_u(const char *fName)
{
     MEM_INT32
     total   =0,
     memSize =0;

     FILE*
     fp =fopen(fName, "w");

     if ( fp )
     {
          fwrite(HEAD_ID_PHRS_V2, sizeof(HEAD_ID_PHRS_V2) *sizeof(char), 1, fp);

          USER_V1_DAT_ITR
          ii =udict.begin();

          for ( ii = udict.begin(); ii != udict.end(); ii++ )
          {
                // sID +Keys +Phrs
                memSize +=sizeof(MEM_INT) +ii->wsKeys.size() +ii->wsPhrs.size();
          }

          MEM_INT*
          pMem =new MEM_INT[memSize +1];

          if ( pMem !=0 )
          {
               memset(pMem, 0, memSize +1);

               PHRS_IDX*
               ux =new PHRS_IDX[(udict.size()<<1) +1];

               if ( ux ==0 )
               {
                    delete [] pMem;

                    fwrite(&total, sizeof(MEM_INT32), 1, fp);
                    fclose(fp);
                    return false;
               }

               MEM_INT32
               offs =0;

               // one times save 2 records
               for ( ii = udict.begin(); ii != udict.end(); ii++, total +=2 )
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
bool phraseimp_v1::load_u(const char *fName)
{
     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp_v1::append_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 orderID)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     USER_V1_DAT
     ui2(sID, wsKeys, wsContent.c_str(), 0);

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

     USER_V1_DAT_ITR
     ui2_found =std::find(udict.begin(), udict.end(), ui2);

     if ( ui2_found ==udict.end() )
     {
          USER_V1_DAT
          ex(sID, wsKeys, wsPhrs, orderID);

          udict.insert(ex);

          return true;
     }

     MEM_INT32
     LastOrder =ui2_found->orderID;

     if ( LastOrder !=orderID )
     {
          USER_V1_DAT
          ex(sID, wsKeys, wsPhrs, orderID);

          udict.erase(ui2_found);

          udict.insert(ex);
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
bool phraseimp_v1::remove_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     USER_V1_DAT
     ui2(sID, wsKeys, wsContent.c_str(), 0);

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
          KIKA_LOGD("Kika-ENGINE, phraseimp_v1::remove, %ls\r\n", aa);
     }
     #endif

     #if defined(__DEBUG_MSG_EX)
     USER_V1_DAT_ITR
     bi =udict.begin();

     while ( bi !=udict.end() )
     {
             KIKA_LOGD("Kika-ENGINE, phraseimp_v1::get k=%ls, c=%ls", bi->wsKeys.c_str(), bi->wsPhrs.c_str());
             bi++;
     }
     #endif

     USER_V1_DAT_ITR
     ui2_found =std::find(udict.begin(), udict.end(), ui2);

     if ( ui2_found !=udict.end() )
     {
          udict.erase(ui2_found);

          #if defined(__DEBUG_MSG)
          KIKA_LOGD("Kika-ENGINE, phraseimp_v1::remove, OK!");
          #endif

          return true;
     }
     #if defined(__DEBUG_MSG)
     KIKA_LOGD("Kika-ENGINE, phraseimp_v1::remove, FAIL!");
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
void phraseimp_v1::getAll_u(int sID, set<USER_V1_DAT> &All)
{
     USER_V1_DAT_ITR
     bi =udict.begin();

     while ( bi !=udict.end() )
     {
             if ( bi->sID ==sID )
                  All.insert(*bi);
             bi++;
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
MEM_INT32 phraseimp_v1::getOrderID_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     USER_V1_DAT
     ui2(sID, wsKeys, wsContent.c_str(), 0);

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

     USER_V1_DAT_ITR
     ui2_found =std::find(udict.begin(), udict.end(), ui2);

     if ( ui2_found !=udict.end() )
     {
          return ui2_found->orderID;
     }
     return this->getTimeStamp();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool phraseimp_v1::isExist_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     USER_V1_DAT
     ui2(sID, wsKeys, wsContent.c_str(), 0);

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

     USER_V1_DAT_ITR
     ui2_found =std::find(udict.begin(), udict.end(), ui2);

     if ( ui2_found !=udict.end() )
     {
          return true;
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
int  phraseimp_v1::getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut)
{
     if ( iOffset ==INVALID_MEM_OFFSET )
     {
          return -1;
     }

     LPCMEM_INT
     pMem =(LPCMEM_INT)memNext.bmem() +(iOffset -1);

     int
     vLen =0;

     for ( vLen =0; pMem[vLen]; vLen++ );

     //------------------------------------------------------------------------
     wchar_t*
     vMem =new wchar_t[vLen +1];

     if ( vMem ==0 )
     {
          return 0;
     }

     memset(vMem, 0, (vLen +1) *sizeof(MEM_INT));

     for ( int xIdx =0; xIdx <vLen; xIdx++ )
     {
           vMem[xIdx] =pMem[xIdx];
     }
     vMem[vLen] =0;

     if ( wcslen(vMem) >3 && vMem[0] =='@' && vMem[1] =='x' && vMem[2] =='@' )
     {
          wsOut =vMem;
          delete [] vMem;

          wsOut.erase(0, 3);
          return wsOut.size();
     }

     wsOut =encryptDecrypt(vMem, ENC_KEY_SZ);

     delete [] vMem;

     return wsOut.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  phraseimp_v1::getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"vdict.size() =%d\r\n", vdict.size());
          #else
          swprintf(aa, 1024, L"vdict.size() =%d\r\n", vdict.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_UI
     ui(this, sID, wsKeys, (wchar_t *)L"");

     PHRS_ITR
     bi =vdict.begin();

     #if defined(_WIN32) && defined(__DEBUG_MSG)
     //TRACE(_T("vdict size =%d\r\n"), vdict.size());
     #endif

     while ( v.size() <(unsigned long)total )
     {
             PHRS_ITR
             ui_found =find_if(bi, vdict.end(), v1_PHRS_UI_keyMatch(ui));

             if ( ui_found ==vdict.end() )
             {
                  break;
             }
             wstring
             wsContent =ui_found->wsPhrs;

             #if defined(_WIN32) && defined(__DEBUG_MSG)
             //TRACE(_T("wsContent =%s\r\n"), wsContent.c_str());
             #endif

             v.push_back(decrypt(wsContent, string(ENC_KEY_SZ)));

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
struct v1_PHRS_UI_idiomKeyMatch
{
       PHRS_UI               In;

       v1_PHRS_UI_idiomKeyMatch(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            int
            cmp =In.wsKeys ==chk.wsKeys;

            if ( cmp <0 )
            {
                 return false;
            }

            return In.wsPhrs ==chk.wsPhrs;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v1_IDIOM_UI_idiomKeyMatch
{
       IDIOM_UI              In;

       v1_IDIOM_UI_idiomKeyMatch(IDIOM_UI input) : In(input)
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
int  phraseimp_v1::getIdiom(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
{
     setlocale( LC_ALL, "");

     if ( vdict.size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     #if defined(__USE_FULL_MATCH)

     PHRS_UI
     ui(sID, wsKeys, (wchar_t *)wsKeyMatch);

     PHRS_ITR
     bi =vdict.begin();

     #else

     IDIOM_UI
     ui(wsKeys, INVALID_MEM_OFFSET);

     IDIOM_ITR
     bi =vnext.begin();

     #endif

     vector<PHRS_UI>
     vdictGet;

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
             #if defined(__USE_FULL_MATCH)

             PHRS_ITR
             ui_found =find_if(bi, vdict.end(), v1_PHRS_UI_idiomKeyMatch(ui));

             if ( ui_found ==vdict.end() )
             {
                  break;
             }
             PHRS_UI
             ui2(ui_found->sID, ui_found->wsKeys.c_str(), ui_found->wsPhrs.c_str(), ui_found->iNext);

             bool
             match =true;

             if ( wsKeyMatch !=0 && wcslen(wsKeyMatch) >0 )
             {
                  int
                  end =ui2.wsPhrs.find(wsKeyMatch);

                  match =(end !=std::string::npos) && end ==0;
             }

             if ( match ==true )
             {
                  vdictGet.push_back(ui2);

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

                  found =true;

                  break;
             }

             #else

             IDIOM_ITR
             ui_found =find_if(bi, vnext.end(), v1_IDIOM_UI_idiomKeyMatch(ui));

             if ( ui_found ==vnext.end() )
             {
                  break;
             }
             PHRS_UI
             ui2(this, sID, wsKeys, L"", ui_found->iNext);

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

             #endif

             bi =ui_found +1;
     }
     if ( found ==false )
          return 0;

     fillIdiomData(wsKeys, vdictGet, v, wcslen(wsKeys));

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
struct v1_PHRS_UI_keyMatchGrouping
{
       int                   cmpLen;
       PHRS_UI               In;

       v1_PHRS_UI_keyMatchGrouping(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }

       bool operator()(const PHRS_V1_POS& chk) const
       {
            #if defined(_WIN32) && defined(__DEBUG_MSG_DATA)
            //TRACE(_T("In =%s, Chk =%s\r\n"), In.wsKeys.c_str(), chk.itr->wsKeys.c_str());
            #endif

            int
            cmp =wcsncmp(In.wsKeys.c_str(), chk.itr->wsKeys.c_str(), cmpLen);

            return cmp ==0;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct target_cmpKeyGrouping
{
       int            cmpLen;

       target_cmpKeyGrouping(int cmpLenIn)
       {
            cmpLen =cmpLenIn;
       }

       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            wstring
            wsKey1 =a.wsPhrs,
            wsKey2 =b.wsPhrs;

            int
            pos =wsKey1.find(L":");

            if ( pos >0 )
            {
                 wsKey1 =wsKey1.substr(++pos, wsKey1.length() -pos);
            }
            pos =wsKey2.find(L":");

            if ( pos >0 )
            {
                 wsKey2 =wsKey2.substr(++pos, wsKey2.length() -pos);
            }

            int
            cmp =wcsncmp(wsKey1.c_str(), wsKey2.c_str(), cmpLen);

            if ( cmp <0 )
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
int  phraseimp_v1::getGrouping(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     if ( vdict.size() ==0 )
     {
          return 0;
     }

     //------------------------------------------------------------------------
     PHRS_ITR
     bi =vdict.begin();

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
     unsigned long
     getRangeItem =(wcslen(wsKeys) ==1 ? 500 :vdict.size());

     PHRS_UI
     ui(this, sID, wsKeys, L"");

     PHRS_V1_POS_ITR
     ui_found =find_if(v1dgrp.begin(), v1dgrp.end(), v1_PHRS_UI_keyMatchGrouping(ui, wcslen(wsKeys)));

     vector<PHRS_UI>
     vdictGet;

     while ( ui_found !=v1dgrp.end() )
     {
             PHRS_V1_POS_ITR
             bi1 =ui_found;

             #if defined(_WIN32) && defined(__DEBUG_MSG)
             //TRACE(_T("%ls, %ls, %ls\r\n"), ui.wsKeys.c_str(), bi1->itr->wsKeys.c_str(), bi1->itr->wsPhrs.c_str());
             #endif

             if ( ui.wsKeys.compare(0, wcslen(wsKeys), bi1->itr->wsKeys) )
             {
                  break;
             }

             PHRS_UI
             ui2(this, bi1->itr->sID, bi1->itr->wsKeys.c_str(), bi1->itr->wsPhrs.c_str(), bi1->itr->iNext);

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

     fillGroupingData(wsKeys, vdictGet, v, wcslen(wsKeys));

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
struct v1_PHRS_UI_CmpKeyLen
{
       PHRS_UI               In;

       v1_PHRS_UI_CmpKeyLen(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            //if ( chk.wsKeys.length() >In.wsKeys.length() )
            {
                 int
                 pos =chk.wsKeys.find(In.wsKeys);

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
struct v1_PHRS_UI_keyMatchFuzzy
{
       PHRS_UI               In;
       int                   cmpLen;

       v1_PHRS_UI_keyMatchFuzzy(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }

       bool operator()(const PHRS_V1_POS& chk) const
       {
            if ( In.wsKeys ==chk.itr->wsKeys )
            {
                 if ( chk.itr->wsPhrs.size() >1 )
                 {
                      if ( chk.itr->wsPhrs.compare(2, cmpLen, In.wsPhrs) ==0 )
                      {
                           return true;
                      }
                 }
                 else
                 {
                      if ( chk.itr->wsKeys.compare(0, cmpLen, In.wsKeys) ==0 )
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
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v1::getKeyFuzzy(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsKeyMatch, int total)
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
     cmpLen      =wcslen(wsKeyIn),
     cmpLenMatch =wcslen(wsKeyMatch);

     unsigned long
     getRangeItem =(cmpLen ==1 ? 500 :vdict.size());

     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet;

     PHRS_UI
     ui(this, sID, wsKeyIn, wsKeyMatch);

     PHRS_V1_POS_VTR
     ui_found =find_if(v1dpos.begin(), v1dpos.end(), v1_PHRS_UI_keyMatchFuzzy(ui, cmpLenMatch));

     int
     getTotal =0;

     if ( ui_found !=v1dpos.end() )
     {
          bool
          match =true;

          do
          {
                PHRS_V1_POS_VTR
                bi1 =ui_found;

                PHRS_UI
                ui2(this, bi1->itr->sID, bi1->itr->wsKeys.c_str(), bi1->itr->wsPhrs.c_str(), bi1->itr->iNext);

                #if defined(_WIN32) && defined(__DEBUG_MSG)
                //TRACE(_T("%ls, %ls, %ls\r\n"), ui.wsKeys.c_str(), bi1->itr->wsKeys.c_str(), bi1->itr->wsPhrs.c_str());
                #endif

                //int
                //cmp =wcsncmp(ui.wsKeys.c_str(), bi1->itr->wsKeys.c_str(), cmpLen);

                int
                cmp =wcscmp(ui.wsKeys.c_str(), bi1->itr->wsKeys.c_str());

                if ( cmp ==0 )
                {
                     cmp =wcsncmp(wsKeyMatch, bi1->wsStrokes.c_str(), wcslen(wsKeyMatch));

                     if ( cmp !=0 )
                     {
                          break;
                     }
                }
                else
                {
                     break;
                }

                vdictGet.push_back(ui2);

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

                ui_found++;

          } while ( ui_found !=v1dpos.end() && match ==true && getTotal <total );
     }

     //------------------------------------------------------------------------
     // get match key all of rest
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;

     if ( getTotal <total )
     {
          PHRS_ITR
          bi_next =vdict.begin();

          while ( vdictGet.size() <getRangeItem )
          {
                  PHRS_ITR
                  ui_found =find_if(bi_next, vdict.end(), v1_PHRS_UI_CmpKeyLen(ui));

                  if ( ui_found ==vdict.end() )
                  {
                       break;
                  }

                  PHRS_UI
                  ui2(this, ui_found->sID, ui_found->wsKeys.c_str(), ui_found->wsPhrs.c_str());

                  if ( std::find(vdictGet.begin(), vdictGet.end(), ui2) ==vdictGet.end() )
                  {
                       bool
                       match =true;

                       if ( wsKeyMatch !=0 && wcslen(wsKeyMatch) >0 )
                       {
                            int
                            end =ui2.wsPhrs.find(wsKeyMatch, 2);

                            match =(end !=std::string::npos) && end ==2;
                       }

                       if ( match ==true )
                       {
                            vdictGet2.push_back(ui2);

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
     fillData(vdictGet2, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v1_target_equal()), v.end());

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
struct v1_PHRS_UI_keyMatch_Vpos
{
       PHRS_UI               In;

       v1_PHRS_UI_keyMatch_Vpos(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const PHRS_V1_POS& chk) const
       {
            return In.wsKeys ==chk.itr->wsKeys;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v1_PHRS_UI_keyMatchFuzzyEx
{
       PHRS_UI               In;

       v1_PHRS_UI_keyMatchFuzzyEx(PHRS_UI input) : In(input)
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
//
//-----------------------------------------------------------------------------
struct v1_findTimesN
{
       USER_V1_DAT           In;

       v1_findTimesN(USER_V1_DAT input) : In(input)
       {
       }

       bool operator()(const USER_V1_DAT& chk) const
       {
            #if defined(__USE_OLD_STYLE_TO_LOWER)
            wstring
            nws1,
            nws2,
            ws1 =chk.wsKeys.c_str(),
            ws2 =In.wsKeys.c_str();

            for ( auto c1: ws1 )
            {
                  nws1 +=static_cast<wchar_t>(std::towlower(c1));
            }
            for ( auto c2: ws2 )
            {
                  nws2 +=static_cast<wchar_t>(std::towlower(c2));
            }

            string::size_type
            position =nws1.find(nws2);
            #else

            wstring
            ws1 =chk.wsKeys.c_str(),
            ws2 =In.wsKeys.c_str();

            #if defined(__FOR_HUAWEI)
            if ( ws2.size() <ws1.size() )
                 return false;
            #endif

            std::transform(ws1.begin(), ws1.end(), ws1.begin(), ::tolower);
            std::transform(ws2.begin(), ws2.end(), ws2.begin(), ::tolower);

            string::size_type
            position =ws1.find(ws2);

            #endif

            if ( position ==0 )
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
int  phraseimp_v1::getKeyFuzzyByTimesN(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     if ( udict.size() ==0 )
     {
          return 0;
     }

     #if defined(_WIN32) && defined(_DEBUG_MSG)
     {
          USER_V1_DAT_ITR
          ui_next =udict.begin();

          while ( ui_next !=udict.end() )
          {
                  TRACE(_T("orderID =%d\r\n"),  ui_next->orderID);
                  TRACE(_T("word    =%ls\r\n"), ui_next->wsStrokes.c_str());

                  ui_next++;
          }
     }
     #endif

     //------------------------------------------------------------------------
     int
     cmpLen   =wcslen(wsKeys),
     getTotal =0;

     //------------------------------------------------------------------------
     // get match key
     //------------------------------------------------------------------------
     USER_V1_DAT
     ui(sID, wsKeys, L"", 0);

     vector<PHRS_UI>
     vdictGet;

     USER_V1_DAT_ITR
     bi =udict.begin();

     #if defined(_WIN32)
     const wchar_t * pu1k =ui.wsKeys.c_str();
     const wchar_t * pu1p =ui.wsPhrs.c_str();
     #endif

     while ( getTotal <total ) // && bi !=udict.end() )
     {
             USER_V1_DAT_ITR
             ui_found =std::find_if(bi, udict.end(), v1_findTimesN(ui));

             if ( ui_found ==udict.end() )
                  break;

             wstring
             wKeys =ui_found->wsKeys,
             wPhrs =ui_found->wsPhrs;

             wstring::size_type
             position =wKeys.find(L":");

             if ( position !=-1 )
             {
                  const wchar_t * pu2kx =wKeys.c_str();
                  const wchar_t * pu2px =wPhrs.c_str();

                  wKeys  =ui_found->wsKeys.substr(0, position);
                  wPhrs +=ui_found->wsKeys.substr(position, ui_found->wsKeys.size() -position);
             }

             PHRS_UI
             ui2(this, sID, wKeys.c_str(), wPhrs.c_str(), ui_found->orderID);

             #if defined(_WIN32)
             const wchar_t * pu2k =ui_found->wsKeys.c_str();
             const wchar_t * pu2p =ui_found->wsPhrs.c_str();
             #endif

             #if defined(_WIN32) && defined(_DEBUG_MSG)
             TRACE(_T("Get =%ls\r\n"), ui_found->wsStrokes.c_str());
             #endif

             vdictGet.push_back(ui2);

             getTotal++;

             bi =ui_found;
             bi++;
     }

     //------------------------------------------------------------------------
     // fill back data
     //------------------------------------------------------------------------
     //std::sort(vdictGet.begin(), vdictGet.end());

     fillData(vdictGet, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v1_target_equal()), v.end());

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
struct v1_target_equalVc
{
       bool operator()(VCandidate const &a, VCandidate const &b) const
       {
            int
            yes =a.ws.compare(b.ws);

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
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v1::getKeyFuzzyByTimesNVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, int total, bool checkPrior)
{
     setlocale( LC_ALL, "");

     if ( udict.size() ==0 )
     {
          return 0;
     }

     #if defined(_WIN32) && defined(_DEBUG_MSG)
     {
          USER_V1_DAT_ITR
          ui_next =udict.begin();

          while ( ui_next !=udict.end() )
          {
                  TRACE(_T("orderID =%d\r\n"),  ui_next->orderID);
                  TRACE(_T("word    =%ls\r\n"), ui_next->wsStrokes.c_str());

                  ui_next++;
          }
     }
     #endif

     //------------------------------------------------------------------------
     int
     cmpLen   =wcslen(wsKeys),
     getTotal =0;

     //------------------------------------------------------------------------
     // get match key
     //------------------------------------------------------------------------
     USER_V1_DAT
     ui(sID, wsKeys, L"", 0);

     vector<VCandidate>
     vdictGet;

     USER_V1_DAT_ITR
     bi =udict.begin();

     #if defined(_WIN32)
     const wchar_t * pu1k =ui.wsKeys.c_str();
     const wchar_t * pu1p =ui.wsPhrs.c_str();
     #endif

     while ( getTotal <total ) // && bi !=udict.end() )
     {
             USER_V1_DAT_ITR
             ui_found =std::find_if(bi, udict.end(), v1_findTimesN(ui));

             if ( ui_found ==udict.end() )
                  break;

             wstring
             wKeys =ui_found->wsKeys,
             wPhrs =ui_found->wsPhrs;

             wstring::size_type
             position =wKeys.find(L":");

             if ( position !=-1 )
             {
                  const wchar_t * pu2kx =wKeys.c_str();
                  const wchar_t * pu2px =wPhrs.c_str();

                  wKeys  =ui_found->wsKeys.substr(0, position);
                  wPhrs +=ui_found->wsKeys.substr(position, ui_found->wsKeys.size() -position);
             }

             VCandidate
             ui2(wPhrs.c_str(), wKeys.c_str(), sID, 0, 0, false, ui_found->orderID);

             #if defined(_WIN32)
             const wchar_t * pu2k =ui_found->wsKeys.c_str();
             const wchar_t * pu2p =ui_found->wsPhrs.c_str();
             #endif

             #if defined(_WIN32) && defined(_DEBUG_MSG)
             TRACE(_T("Get =%ls\r\n"), ui_found->wsStrokes.c_str());
             #endif

             #if defined(__FOR_HUAWEI)
             vector<VCandidate>
             vcFullNumb;

             int
             countGet =gImeX->getKeyNumFullVc(sID, wKeys.c_str(), vcFullNumb, L"", 1, true);

             if ( countGet >0 )
             {
                  ui2.wKey =vcFullNumb[0].wKey;
             }
             #endif // defined

             vdictGet.push_back(ui2);

             getTotal++;

             bi =ui_found;
             bi++;
     }

     //------------------------------------------------------------------------
     // fill back data
     //------------------------------------------------------------------------
     fillDataVc2(vdictGet, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v1_target_equalVc()), v.end());

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
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
int  phraseimp_v1::getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
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
     unsigned long
     getRangeItem =(wcslen(wsKeys) ==1 ? 500 :vdict.size());

     int
     cmpLen   =wcslen(wsKeys),
     getTotal =0;

     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet;

     PHRS_UI
     ui(this, sID, wsKeys, L"");

     PHRS_V1_POS_VTR
     ui_found =find_if(v1dpos.begin(), v1dpos.end(), v1_PHRS_UI_keyMatch_Vpos(ui));

     #if defined(_DEBUG_LOG)
     CWfxFileW
     AA;
     AA.SaveHeader(WfxCurrPath(_T("\\Found.log")));
     #endif

     if ( ui_found !=v1dpos.end() )
     {
          bool
          match =true;

          do
          {
               PHRS_V1_POS_VTR
               bi1 =ui_found;

               PHRS_UI
               ui2(this, bi1->itr->sID, bi1->itr->wsKeys.c_str(), bi1->itr->wsPhrs.c_str(), bi1->itr->iNext);

               #if defined(_WIN32) && defined(__DEBUG_MSG)
               {
                    const wchar_t *p1 =ui.wsKeys.c_str();
                    const wchar_t *p2 =bi1->itr->wsKeys.c_str();
                    const wchar_t *p3 =bi1->itr->wsPhrs.c_str();

                    wchar_t
                    wsTmp[450] ={0};
                    wsprintf(wsTmp, _T("o=%ls, f=%ls, f=%ls, sID =%d, strokes =%ls\r\n"), p1, p2, p3, bi1->itr->sID, bi1->wsStrokes.c_str());

                    //TRACE(wsTmp);

                    #if defined(_DEBUG_LOG)
                    AA.Append(wsTmp);
                    #endif
               }
               #endif

               //-----------------------------------------
               // Must be equal to Input Key Length
               //-----------------------------------------
               if ( (int)bi1->itr->wsKeys.size() !=cmpLen )
               {
                    break;
               }

               int
               cmp =wcsncmp(ui.wsKeys.c_str(), bi1->itr->wsKeys.c_str(), cmpLen);

               if ( cmp !=0 )
               {
                    break;
               }

               {
                    vdictGet.push_back(ui2);

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

          } while ( match ==true && getTotal <total && ui_found !=v1dpos.end() );
     }

     //------------------------------------------------------------------------
     // get match key all of rest
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;

     if ( getTotal <total )
     {
          PHRS_ITR
          bi2 =vdict.begin();

          while ( vdictGet.size() <getRangeItem )
          {
                  PHRS_ITR
                  ui_found =find_if(bi2, vdict.end(), v1_PHRS_UI_keyMatchFuzzyEx(ui));

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

                       #if defined(_DEBUG_LOG)
                       AA.Append(wsTmp);
                       #endif
                  }
                  #endif

                  //TRACE(_T("sz =%d\r\n"), (int)vdictGet.size());

                  if ( std::find(vdictGet.begin(), vdictGet.end(), ui2) ==vdictGet.end() )
                  {
                       bool
                       match =true;

                       if ( match ==true )
                       {
                            vdictGet2.push_back(ui2);

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

                  bi2 =ui_found +1;
          }
     }

     #if defined(_DEBUG_LOG)
     AA.CloseFile();
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
     fillData(vdictGet, v);
     fillData(vdictGet2, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v1_target_equal()), v.end());

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
struct v1_PHRS_UI_fuzzyMatch
{
       PHRS_UI               In;

       v1_PHRS_UI_fuzzyMatch(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            //if ( chk.wsKeys.length() >In.wsKeys.length() )
            {
                 int
                 pos =chk.wsKeys.find(In.wsKeys);

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
int  phraseimp_v1::getFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total)
{
     setlocale( LC_ALL, "");

     if ( vdict.size() ==0 )
     {
          return 0;
     }

     #if defined(__DEBUG_MSG)
     {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)
          swprintf(aa, L"vdict.size() =%d\r\n", vdict.size());
          #else
          swprintf(aa, 1024, L"vdict.size() =%d\r\n", vdict.size());
          #endif

          dw_pintf_ex(aa);
     }
     #endif

     PHRS_UI
     ui(this, sID, wsKeys, (wchar_t *)L"");

     PHRS_ITR
     bi =vdict.begin();

     while ( v.size() <(unsigned long)total )
     {
             PHRS_ITR
             ui_found =find_if(bi, vdict.end(), v1_PHRS_UI_fuzzyMatch(ui));

             if ( ui_found ==vdict.end() )
                  break;

             wstring
             wsContent =ui_found->wsPhrs;

             v.push_back(decrypt(wsContent, string(ENC_KEY_SZ)));

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
int  phraseimp_v1::get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total, bool fuzzy)
{
     if ( vdict.size() ==0 )
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
     v.erase(std::unique(v.begin(), v.end(), v1_target_equal()), v.end());

     #endif

     return v.size();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v1_target_cmpID
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
struct v1_PHRS_UI_CmpKeyLenEx
{
       PHRS_UI               In;

       v1_PHRS_UI_CmpKeyLenEx(PHRS_UI input) : In(input)
       {
       }

       bool operator()(const V2_DICT_UI& chk) const
       {
            wstring
            wsKeys;

            {
                 int
                 pos =chk.wKeys.find(In.wsKeys);

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
struct v1_PHRS_UI_keyMatchLen
{
       PHRS_UI               In;
       int                   cmpLen;

       v1_PHRS_UI_keyMatchLen(PHRS_UI input, int cmpLenIn) : In(input), cmpLen(cmpLenIn)
       {
       }

       bool operator()(const PHRS_V1_POS& chk) const
       {
            if ( In.wsKeys.size() ==chk.itr->wsKeys.size() )
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
struct v1_PHRS_UI_MatchLen_By_KLen_Begin
{
       PHRS_UI               In;

       v1_PHRS_UI_MatchLen_By_KLen_Begin(PHRS_UI _input) : In(_input)
       {
       }

       bool operator()(const PHRS_V1_POS& chk) const
       {
            return chk.itr->wsKeys[0] ==In.wsKeys[0];
       }
};


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct v1_PHRS_UI_MatchLen_By_KLen_End
{
       PHRS_UI               In;

       v1_PHRS_UI_MatchLen_By_KLen_End(PHRS_UI _input) : In(_input)
       {
       }

       bool operator()(const PHRS_V1_POS& chk) const
       {
            return chk.itr->wsKeys[0] !=In.wsKeys[0];
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : first full match, then fuzzy
//
//-----------------------------------------------------------------------------
void phraseimp_v1::setupBeginEnd(const wchar_t *wsKeyIn)
{
     if ( mLastCH !=wsKeyIn[0] )
     {
          mLastCH =wsKeyIn[0];

          v1dpos_begin =v1dpos.begin(),
          v1dpos_end   =v1dpos.end();

          wstring
          wsCH =wsKeyIn;
          wsCH.resize(1);

          PHRS_UI
          ui2(this, 0, wsCH, L"");

          //-------------------------------------------------------------------
          PHRS_V1_POS_VTR
          ui_found_begin =find_if(v1dpos.begin(), v1dpos.end(), v1_PHRS_UI_MatchLen_By_KLen_Begin(ui2));

          if ( ui_found_begin !=v1dpos.end() )
          {
               while ( 1 )
               {
                       PHRS_V1_POS_VTR
                       ui_found =find_if(v1dpos.begin(), ui_found_begin, v1_PHRS_UI_MatchLen_By_KLen_Begin(ui2));

                       if ( ui_found ==ui_found_begin )
                       {
                            break;
                       }
                       ui_found_begin =ui_found;
               }
               v1dpos_begin =ui_found_begin;
          }

          //-------------------------------------------------------------------
          PHRS_V1_POS_VTR
          ui_found_end =find_if(v1dpos_begin, v1dpos_end, v1_PHRS_UI_MatchLen_By_KLen_End(ui2));

          if ( ui_found_end !=v1dpos_end )
          {
               while ( 1 )
               {
                       PHRS_V1_POS_VTR
                       ui_found =find_if(v1dpos_begin, ui_found_end, v1_PHRS_UI_MatchLen_By_KLen_End(ui2));

                       if ( ui_found ==ui_found_end )
                       {
                            break;
                       }
                       ui_found_end =ui_found;
               }
               v1dpos_end =ui_found_end;
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
int  phraseimp_v1::getKeyWildc(int sID, const wchar_t *wsKeyIn, vector<wstring> &v, const wchar_t *wsWildcard, int total)
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

     set<wstring>
     vdictSet;

     //------------------------------------------------------------------------
     // get exactly match key & lenght first
     //------------------------------------------------------------------------
     PHRS_UI
     ui(this, sID, wsKeyIn, L"");

     vector<PHRS_UI>
     vdictGet;

     PHRS_V1_POS_VTR
     ui_found =find_if(v1wlen.begin(), v1wlen.end(), v1_PHRS_UI_keyMatchLen(ui, cmpLen));

     int
     getTotal =0;

     while ( ui_found !=v1wlen.end() && getTotal <total )
     {
             PHRS_UI
             ui2(this, ui_found->itr->sID, ui_found->itr->wsKeys, ui_found->itr->wsPhrs, ui_found->itr->iNext);

             #if defined(_WIN32) && !defined(__DEBUG_MSG)
             //TRACE(_T("ik=%ls, k=%ls, w=%ls\r\n"), ui.wsKeys.c_str(), ui_found->itr->wsKeys.c_str(), ui_found->itr->wsPhrs.c_str());
             #endif

             if ( (int)ui2.wsKeys.size() !=cmpLen )
             {
                  ui_found++;
                  break;
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

             if ( cmp ==0 )
             {
                  bool
                  ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                  if ( ok ==true )
                  {
                       vdictGet.push_back(ui2);
                       vdictSet.insert(ui2.wsPhrs);

                       getTotal++;
                  }
             }
             ui_found++;
     }

     //------------------------------------------------------------------------
     // use hardcode word, performance is very bad, and freq answer not 100% right
     //------------------------------------------------------------------------
     if ( v1wlen.size() ==0 )
     {
          ui_found =find_if(v1dpos.begin(), v1dpos.end(), v1_PHRS_UI_keyMatchLen(ui, cmpLen));

          while ( ui_found !=v1dpos.end() && getTotal <total )
          {
                  PHRS_UI
                  ui2(this, ui_found->itr->sID, ui_found->itr->wsKeys, ui_found->itr->wsPhrs, ui_found->itr->iNext);

                  #if defined(_WIN32) && defined(__DEBUG_MSG)
                  //TRACE(_T("ik=%ls, k=%ls, w=%ls\r\n"), ui.wsKeys.c_str(), ui_found->itr->wsKeys.c_str(), ui_found->itr->wsPhrs.c_str());
                  #endif

                  if ( (int)ui2.wsKeys.size() !=cmpLen )
                  {
                       ui_found++;
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

                  if ( cmp ==0 )
                  {
                       bool
                       ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                       if ( ok ==true )
                       {
                            vdictGet.push_back(ui2);
                            vdictSet.insert(ui2.wsPhrs);

                            getTotal++;
                       }
                  }
                  ui_found++;
          }
     }

     //------------------------------------------------------------------------
     // Rest of others
     //------------------------------------------------------------------------
     vector<PHRS_UI>
     vdictGet2;

     if ( getTotal <total )
     {
          setupBeginEnd(wsKeyIn);

          PHRS_ITR
          bi_next =vdict.begin();

          //while ( vdictGet2.size() <getRangeItem && bi_next !=vdict.end() )
          while ( bi_next !=vdict.end() && getTotal <total )
          {
                  PHRS_UI
                  ui2(this, bi_next->sID, bi_next->wsKeys.c_str(), bi_next->wsPhrs.c_str());

                  if ( (int)ui2.wsKeys.size() <cmpLen )
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

                  if ( cmp ==0 )
                  {
                       bool
                       ok =std::find(vdictSet.begin(), vdictSet.end(), ui2.wsPhrs) ==vdictSet.end();

                       if ( ok ==true )
                       {
                            vdictGet2.push_back(ui2);
                            vdictSet.insert(ui2.wsPhrs);
                            getTotal++;
                       }
                  }
                  bi_next++;
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
     std::sort(vdictGet.begin(), vdictGet.end(), v1_target_cmpID());
     fillData(vdictGet, v);

     std::sort(vdictGet2.begin(), vdictGet2.end(), v1_target_cmpID());
     fillData(vdictGet2, v);

     #if defined(__APPLY_UNIQUE_RULE)

     v.erase(std::unique(v.begin(), v.end(), v1_target_equal()), v.end());

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
bool phraseimp_v1::isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     PHRS_UI
     ui(this, sID, wsKeys, wsPhrs);

     if ( find(vdict.begin(), vdict.end(), ui) ==vdict.end() )
          return false;

     return true;
}
