// Timer_Linux.h: interface for the CTimer_Linux class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMER_LINUX_H__685C37E0_FC16_4137_83E5_0DA860473A74__INCLUDED_)
#define AFX_TIMER_LINUX_H__685C37E0_FC16_4137_83E5_0DA860473A74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(__LINUX)

#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include "HTTP_Imp.h"


class CTimer_Linux
{
private:
      timer_t                timerID;
      
public:
      static
      THREAD_PARAM_UI*       param;
      
public:
      CTimer_Linux();
      virtual ~CTimer_Linux();
      
      BOOL start();
      void stop();

      void attach(THREAD_PARAM_UI* param)
      {
           this->param =param;
      }
};

#endif


#endif // !defined(AFX_TIMER_LINUX_H__685C37E0_FC16_4137_83E5_0DA860473A74__INCLUDED_)
