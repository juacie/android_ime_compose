// vmap_imp.cpp: implementation of the vmap_imp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include "vmap_imp.h"
#include "string_util.h"

#if defined(_DEBUG) && defined(_Win32)
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define __ENCODE_MSG2
#define MAX_BIT                        2


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
vmap_imp::vmap_imp()
{
     mTotal =0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
vmap_imp::~vmap_imp()
{
     mVc.clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vmap_imp::clear()
{
     mVc.clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  vmap_imp::loadDict(const char *dictName)
{
     mVc.clear();

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, vmap, dictName =%s\r\n", dictName);
     #endif

     FILE
     *fp =fopen(dictName, "rb");

     if ( fp )
     {
          fseek(fp, 0L, SEEK_END);

          long
          sz = ftell(fp);

          #if defined(__FOR_ANDROID)
          KIKA_LOGD("Kika-ENGINE, vmap-loadDict, total =%d\r\n", (int)sz);
          #endif

          fseek(fp, 0L, SEEK_SET);

          char*
          pMem =new char[sz +1];

          if ( pMem == nullptr )
          {
               return -2;
          }

          long
          ReadSz =fread(pMem, sizeof(char) *sz, 1, fp);

          fclose(fp);

          #if defined(__FOR_ANDROID)
          KIKA_LOGD("Kika-ENGINE, vmap-loadDict, ReadSz =%d\r\n", (int)ReadSz);
          #endif

          if ( ReadSz !=sz )
          {
               //delete pMem;
               //return -3;
          }

          //-------------------------------------------------------------------
          wstring
          wAll =UTF8_to_wchar(pMem);

          const wchar_t*
          wMemID =wAll.c_str() +1;

          //-------------------------------------------------------------------
          int
          idLen =wcslen(VMAP_FILE_ID);

          if ( wcsncmp(wMemID, VMAP_FILE_ID, idLen) !=0 )
          {
               return -4;
          }

          #if defined(__TOTAL)

          const wchar_t*
          wMem =wAll.c_str() +idLen +sizeof(MEM_TOTAL) +1;

          #else
          const wchar_t*
          wMem =wAll.c_str() +idLen +1;

          #endif

          #if defined(__TOTAL)
          mTotal =*(MEM_TOTAL*)&wMemID[idLen];
          #endif

          //-------------------------------------------------------------------
          #if defined(__FOR_ANDROID)
          KIKA_LOGD("Kika-ENGINE, vmap-loadDict, loading....\r\n");
          #endif

          #if defined(__TOTAL)
          for ( long offs =0; offs <sz && idx <mTotal; idx++ )
          #else
          for ( long offs =0; offs <sz; )
          #endif
          {
                int
                sLen =wMem[offs++];

                if ( sLen ==0 )
                {
                     break;
                }

                wchar_t
                mData[1024] ={0};

                memcpy(mData, wMem +offs, sLen *sizeof(wchar_t));

                wstring
                wsA =mData;

                encrypt(wsA);

                if ( isExist(wsA.c_str()) ==false )
                {
                     mVc.insert(mapVoca(wsA.c_str(), offs));
                }
                offs +=sLen;
          }
          delete[] pMem;

          #if defined(__FOR_ANDROID)
          KIKA_LOGD("Kika-ENGINE, vmap-loadDict, load  OK\r\n");
          #endif

          return 0;
     }

     return -1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool vmap_imp::isExist(const wchar_t* wsVoca)
{
     mapVoca
     ui(wsVoca, -1);

     MAP_VOCA_ITR
     found =mVc.find(ui);

     return found !=mVc.end();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vmap_imp::encrypt(wstring &wVoca)
{
     int
     SLen =wVoca.size();

     unsigned int
     iBits[MAX_BIT] ={0xA, 0xC};

     long
     Idx =0;

     while ( Idx <SLen )
     {
             int
             CH =wVoca[Idx];

             for ( int NIdx =0; NIdx <MAX_BIT; NIdx++ )
             {
                   CH =CH ^iBits[NIdx];
             }
             wVoca[Idx++] =CH;
     }
     wVoca[SLen] =0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vmap_imp::erase(const wchar_t *wsVoca)
{
     mapVoca
     ui(wsVoca, -1);

     mVc.erase(ui);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vmap_imp::erase(const char *sVoca_utf8)
{
     wstring
     wVoca =UTF8_to_wchar(sVoca_utf8);

     erase(wVoca.c_str());
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vmap_imp::append(const wchar_t *wsVoca, long offset)
{
     mapVoca
     ui(wsVoca, offset);

     mVc.insert(ui);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vmap_imp::append(const char *sVoca_utf8, long offset)
{
     wstring
     wVoca =UTF8_to_wchar(sVoca_utf8);

     append(wVoca.c_str());
}

