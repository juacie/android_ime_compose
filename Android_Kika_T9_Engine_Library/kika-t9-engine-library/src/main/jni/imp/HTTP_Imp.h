// HTTP_Imp.h: interface for the CHTTP_Imp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTP_IMP_H__72A52C94_20E2_4996_8BD5_1C10B8C380A2__INCLUDED_)
#define AFX_HTTP_IMP_H__72A52C94_20E2_4996_8BD5_1C10B8C380A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include "Data_Type.h"

#if defined(__LINUX)
 #include "pthread.h"
#endif

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CHTTP_Imp;

struct THREAD_PARAM_UI
{
       LPCSTR                duid;
       LPCBYTE               sData;
       INT                   iLen;

       INT                   tryN;
       INT                   iSucc;
       INT                   action;

       CHTTP_Imp*            pThis;

       #if defined(__LINUX)
       pthread_t             aThread;
       #endif
};

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
#define ONE_DAY              (24 *3600)
#define MAX_SOK_TIMES        1
#define MAX_TRY_TIMES        5
#define MAX_CHR_TIMES        10
#define MAX_PATH_LEN         400

class CHTTP_Imp
{
protected:
     time_t                  now;

     BOOL                    bDebug;
     INT                     timing;
     
     THREAD_PARAM_UI         param;

     char                    sDayCurr[40];
     char                    sLogPath[MAX_PATH_LEN];

public:
      CHTTP_Imp()
      {
           #if defined(__DEBUG) || defined(_DEBUG)
           bDebug =TRUE;
           #elif defined(__TPOEM)
           bDebug =FALSE;
           #else
           bDebug =TRUE;
           #endif

           timing       =0;
           param.tryN   =0;
           param.iSucc  =0;
           param.action =0;
           
           now =time(NULL);
      }

      //-----------------------------------------------------------------------
      void setLogPath(char *sPath)
      {
           if ( strlen(sPath) <MAX_PATH_LEN )
           {
                strcpy(sLogPath, sPath);
           }
      }

      //-----------------------------------------------------------------------
      BOOL isDebug()
      {
           return bDebug;
      }

      void setDebug(BOOL bFlag)
      {
           bDebug =bFlag;
      }

      //-----------------------------------------------------------------------
      INT  diff_tm(time_t last)
      {
           time_t
           now = time(NULL);
           
           double
           res = difftime(last, now);
           
           return (INT)(res /ONE_DAY);
      }

      //-----------------------------------------------------------------------
      BOOL isSucc()
      {
           return param.iSucc >0;
      }
      
      //-----------------------------------------------------------------------
      BOOL canPost(BOOL always)
      {
           if ( param.action ==1 )
           {
                return FALSE;
           }
           return canPostEx();
      }

      BOOL canPostEx()
      {
           INT
           ret =loadData();

           if ( ret ==0 )
           {
                param.action =0;
           }

           //param.action =(param.iSucc ==TRUE ? 0 :1);

           return ret;
      }
      
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      virtual INT  loadData() { }

      virtual void saveData() { }

      virtual void removeData() { }

      virtual INT  onPost(LPCSTR duid, LPCBYTE LstrData, INT iLen) { return 0; }
      
      virtual void onPostThread(LPCSTR duid, LPCBYTE LstrData, INT iLen, BOOL always =false) { }
};


#endif // !defined(AFX_HTTP_IMP_H__72A52C94_20E2_4996_8BD5_1C10B8C380A2__INCLUDED_)
