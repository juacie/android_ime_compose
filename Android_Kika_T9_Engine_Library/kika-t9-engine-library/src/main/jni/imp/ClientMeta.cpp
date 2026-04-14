// ClientMeta.cpp: implementation of the CClientMeta class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientMeta.h"

#include "Data_Host.h"
#include "Data_Meta.h"
#include "Data_BRkey.h"
#include "BufferHandler.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
CClientMeta::CClientMeta()
{
     
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
CClientMeta::~CClientMeta()
{
     
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
INT  CClientMeta::data_pack(LPAgent_Data_Init dui)
{
/*
     mta.free();
     
     mta.put(0);
     mta.put(1);
     mta.putString(dui->d_sdk_version.getMem());
     mta.putString(dui->d_appKey.getMem());  
     mta.putString(dui->d_language.getMem());
     mta.putString(dui->d_duid.getMem());    
     
     //------------------------------------------------------------------------
     META_LOG
     data;
     
     data.items[ID_app      ].set("app",         dui->d_app_version.getMem());
     data.items[ID_res      ].set("res",         dui->d_resolution.getMem());
     data.items[ID_os       ].set("os",          dui->d_os_version.getMem());
     data.items[ID_tacc     ].set("tacc",        "{}");
     data.items[ID_model    ].set("model",       "sdk");
     data.items[ID_config_id].set("config_id",   "0");
     data.items[ID_ts       ].set("ts",          dui->d_ts.getMem());
     data.items[ID_op       ].set("op",          dui->d_op.getMem());
     data.items[ID_aid      ].set("aid",         "");
     data.items[ID_net      ].set("net",         "0");
     
     data.items[ID_agent    ].set("agent",       dui->d_agt_version.getMem());
     data.items[ID_in_i     ].set("in_+",        "[]");
     data.items[ID_lang     ].set("lang",        dui->d_lang.getMem());
     data.items[ID_in_o     ].set("in_-",        "[]");
     data.items[ID_fits     ].set("fits",        dui->d_firstIns_ts.getMem());
     data.items[ID_gms      ].set("gms",         "");
     data.items[ID_app_vcode].set("app_vcode",   KERNEL_VCODE);
     data.items[ID_simop    ].set("simop",       "");
     data.items[ID_pf       ].set("pf",          dui->d_pf.getMem());
     data.items[ID_fiv      ].set("fiv",         dui->d_firstIns_ap_ver.getMem());
     
     data.items[ID_gaid     ].set("gaid",        "");
     data.items[ID_latype   ].set("launch_type", "keyboard");
     data.items[ID_na       ].set("na",          dui->d_lang.getMem());
     data.items[ID_netop    ].set("netop",       "");
     data.items[ID_pn       ].set("pn",          "sdk");
     data.items[ID_ch       ].set("ch",          dui->d_ch.getMem());
     data.items[ID_mf       ].set("mf",          "unknown");
     
     data.toString();
     
     //------------------------------------------------------------------------
     //------------------------------------------------------------------------
     CData_BRkey
     key;
     key.generateKey();

     //------------------------------------------------------------------------
     int
     rsa_len   =0,
     bfish_len =0;
     
     unsigned char
     *bfish_buffer =NULL;

     BufferHandler
     buffHandler;
     buffHandler.encodeData(key.getBKey(), rsa_len, (unsigned char *)data.getMem(), data.getLen(), &bfish_buffer, bfish_len);
     
     mta.putString(key.getRsaBKey(), 128);
     mta.putString((LPCSTR)bfish_buffer, bfish_len);
     
     delete[] bfish_buffer;
*/
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
#if defined(__GPP)

INT  CClientMeta::data_unpack(std::string s)
{
/*
     std::vector<BYTE>
     decoded = base64_decode(s);
     
     LPBYTE
     pByteSz = &decoded[0];
     
     CMeta_Parser
     pas;
     
     INT
     ret =pas.ParseData(pByteSz, decoded.size());

     if ( ret !=-1 )
     {
          fmt.putString(pas.ToStringEx());
     }
     else
     {
          fmt.putString("decode error !!");
     }
     return ret;
*/
return 0;
}

#endif









//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
CMeta_Parser::CMeta_Parser()
{
     dui.d_type    =A_type;
     dui.d_dayHour =A_dayHour;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
CMeta_Parser::~CMeta_Parser()
{
     
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
INT  CMeta_Parser::ParseData(LPBYTE pByte, UINT MaxLen)
{
     /*
     pByte +=2;
     
     MaxLen =ParseHeadData(pByte, MaxLen -2);

     return ReadEncryptContent(pByte, MaxLen);

     */
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
INT  CMeta_Parser::ReadEncryptContent(LPBYTE &pByte, UINT MaxLen)
{
     /*
     BufferHandler
     buffHandler;

     return buffHandler.decodeData(dui.d_blowfishKeyEncrypt.getByte(), pByte, MaxLen, 0, 0);
     */
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
INT  CMeta_Parser::ParseHeadData(LPBYTE &pByte, UINT MaxLen)
{
     /*
     CDepack_Data
     data(pByte, MaxLen);
     
     dui.d_sdk_version =data.getMem();

     //------------------------------------------------------------------------
     data.parse(pByte);
     
     dui.d_appKey =data.getMem();
     
     //------------------------------------------------------------------------
     data.parse(pByte);
     
     dui.d_language =data.getMem();
     
     dui.d_language ="";
     
     //------------------------------------------------------------------------
     data.parse(pByte);

     dui.d_duid =data.getMem();

     //------------------------------------------------------------------------
     data.parse(pByte);
          
     dui.d_blowfishKeyEncrypt =data.getMem();
     
     return data.getLessLen();

     */
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
LPCSTR CMeta_Parser::ToString()
{
     CHAR
     data[8072];

     sprintf(data, "%s\r\napp_version:%s\r\nsdk_version:%s\r\n%s\r\n%s\r\n%s\r\nlanguage:%s\r\nnation:%s", 
                   dui.d_type.c_str(),        dui.d_app_version.c_str(), 
                   dui.d_sdk_version.c_str(), dui.d_appKey.c_str(), 
                   dui.d_duid.c_str(),        dui.d_dayHour.c_str(),
                   dui.d_language.c_str(),    dui.d_nation.c_str());

     fmt.putString(data);

     return fmt.getMem();
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
LPCSTR CMeta_Parser::ToStringEx()
{
     CHAR
     data[8072];

     sprintf(data, "%s\tapp_version:%s\tsdk_version:%s\t%s\t%s\t%s\tlanguage:%s\tnation:%s", 
                   dui.d_type.c_str(),        dui.d_app_version.c_str(), 
                   dui.d_sdk_version.c_str(), dui.d_appKey.c_str(), 
                   dui.d_duid.c_str(),        dui.d_dayHour.c_str(),
                   dui.d_language.c_str(),    dui.d_nation.c_str());

     fmt.putString(data);

     return fmt.getMem();
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
LPCSTR CMeta_Parser::ToStringEx2()
{
     CHAR
     data[8072];

     sprintf(data, "%s\r\napp_version:%s\r\nsdk_version:%s\r\n%s\r\n%s\r\n%s\r\nlanguage:%s\r\nnation:%s", 
                   dui.d_type.c_str(),        dui.d_app_version.c_str(), 
                   dui.d_sdk_version.c_str(), dui.d_appKey.c_str(), 
                   dui.d_duid.c_str(),        dui.d_dayHour.c_str(),
                   dui.d_language.c_str(),    dui.d_nation.c_str());

     fmt.putString(data);

     return fmt.getMem();
}
