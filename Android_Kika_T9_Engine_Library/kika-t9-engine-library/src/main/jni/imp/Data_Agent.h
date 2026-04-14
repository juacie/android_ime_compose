// Data_Agent.h: interface for the CData_Agent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_AGENT_H__3514035B_4586_4380_BED2_C768998AAB60__INCLUDED_)
#define AFX_DATA_AGENT_H__3514035B_4586_4380_BED2_C768998AAB60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data_Mem.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
struct Agent_Data_Header
{
       CData_Mem        d_type;

       CData_Mem        d_sdk_version;
       CData_Mem        d_app_version;
       CData_Mem        d_appKey;
       CData_Mem        d_language;

       CData_Mem        d_duid;
       CData_Mem        d_dayHour;
       CData_Mem        d_nation;

       CData_Mem        d_blowfishKeyEncrypt;
};
typedef Agent_Data_Header*             LPAgent_Data_Header;


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
struct Agent_Data_Init :Agent_Data_Header
{
       CData_Mem        d_ch;
       CData_Mem        d_pf; // platform
       CData_Mem        d_op; // operator system
       CData_Mem        d_os_version;
       CData_Mem        d_agt_version;

       CData_Mem        d_resolution;
       CData_Mem        d_ts;
       CData_Mem        d_firstIns_ts;
       CData_Mem        d_firstIns_ap_ver;
       CData_Mem        d_lang;
};
typedef Agent_Data_Init*               LPAgent_Data_Init;


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CData_Agent
{
public:
      Agent_Data_Init        dui;

public:
      CData_Agent() { }
      virtual ~CData_Agent() { }
};


#endif // !defined(AFX_DATA_AGENT_H__3514035B_4586_4380_BED2_C768998AAB60__INCLUDED_)
