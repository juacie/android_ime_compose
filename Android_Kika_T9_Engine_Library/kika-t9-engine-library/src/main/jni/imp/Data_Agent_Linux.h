// Data_Agent_Linux.h: interface for the CData_Agent_Linux class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_AGENT_LINUX_H__46BC54C4_8125_4552_8E10_CD52B13AFDF3__INCLUDED_)
#define AFX_DATA_AGENT_LINUX_H__46BC54C4_8125_4552_8E10_CD52B13AFDF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data_Agent.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CData_Agent_Linux :public CData_Agent
{
public:
      CData_Agent_Linux();
      virtual ~CData_Agent_Linux();
      
      void init();
};

#endif // !defined(AFX_DATA_AGENT_LINUX_H__46BC54C4_8125_4552_8E10_CD52B13AFDF3__INCLUDED_)
