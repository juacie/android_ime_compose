// Timer_Linux.cpp: implementation of the CTimer_Linux class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Timer_Linux.h"

#if defined(__LINUX)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define SZ 46
long int overruns[SZ];

THREAD_PARAM_UI *         CTimer_Linux::param;


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
CTimer_Linux::CTimer_Linux()
{
     timerID =0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
CTimer_Linux::~CTimer_Linux()
{
     stop();
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
void callback_stopThread(union sigval arg)
{
/*
     timer_t * 
     p =(timer_t *)arg.sival_ptr;

     //int
     //delay =timer_getoverrun(*p);
     
     // count overruns, which are timer intervals skipped
     //if ( delay >3 )
     timer_delete(*p);

     if ( CTimer_Linux::param->action ==1 )
     {
          if ( CTimer_Linux::param->aThread !=NULL )
          {
               pthread_kill(CTimer_Linux::param->aThread, 0);
          }
          CTimer_Linux::param->action =0;
     }
*/
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
BOOL CTimer_Linux::start()
{
/*
     if ( timerID !=NULL )
     {
          stop();
     }

     long int nsec;
     long int msec;
     long int sec;
     
     msec =(1000 *60) *3;
     
     sec   =(msec / 1000);
     msec %=1000;
     nsec  =msec * 1e06;;
     
     struct sigevent
     se;
     
     se.sigev_notify            =SIGEV_THREAD;
     se.sigev_value.sival_ptr   =&timerID;
     se.sigev_notify_function   =callback_stopThread;
     se.sigev_notify_attributes =NULL;
     
     if ( -1 ==timer_create(CLOCK_REALTIME, &se, &timerID) )
     {
          return FALSE;
     }
     
     struct itimerspec
     ts;
     
     ts.it_value.tv_sec     =sec;
     ts.it_value.tv_nsec    =nsec;
     ts.it_interval.tv_sec  =sec;
     ts.it_interval.tv_nsec =nsec ;
     
     return timer_settime(timerID, 0, &ts, NULL) !=-1;
*/
return FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void CTimer_Linux::stop()
{
/*
(     if ( timerID !=NULL )
     {
          timer_delete(timerID);

          timerID =NULL;
     }
*/
}


#endif
