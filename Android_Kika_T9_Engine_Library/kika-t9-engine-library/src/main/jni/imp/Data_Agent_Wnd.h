// Data_Agent_Wnd.h: interface for the CData_Agent_Wnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_AGENT_WND_H__7B6D75DF_0887_41D1_9738_A569034BC289__INCLUDED_)
#define AFX_DATA_AGENT_WND_H__7B6D75DF_0887_41D1_9738_A569034BC289__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data_Agent.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CData_Agent_Wnd :public CData_Agent
{
public:
      CData_Agent_Wnd();
      virtual ~CData_Agent_Wnd();
      
      void init();
};


#endif // !defined(AFX_DATA_AGENT_WND_H__7B6D75DF_0887_41D1_9738_A569034BC289__INCLUDED_)
