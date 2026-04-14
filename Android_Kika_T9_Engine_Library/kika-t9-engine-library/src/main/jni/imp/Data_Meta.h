// Data_Meta.h: interface for the CData_Meta class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_META_H__3585B3F6_63ED_42A9_8405_A022CD8D6839__INCLUDED_)
#define AFX_DATA_META_H__3585B3F6_63ED_42A9_8405_A022CD8D6839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Data_Mem.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
struct META_KEY
{
public:
      CData_Mem              key;
      CData_Mem              val;
      
private:
      INT                    iLen;
      CData_Mem              fmt;
      
public:
      META_KEY()
      {
           iLen =0;
      }
      
      void set(LPCSTR pKey, LPCSTR pVal)
      {
           key.set(pKey);
           val.set(pVal);
      }
      
      void toString()
      {
           iLen =key.getLen() +val.getLen() +5;
           
           if ( fmt.alloc(iLen) ==TRUE )
           {
                sprintf(fmt.getMem(), "\"%s\":\"%s\"", key.getMem(), val.getMem());
           }
      }
      
      BOOL isOK()
      {
           return key.getMem() !='\0' && val.getMem() !='\0';
      }
      
      USHORT getLen()
      {
           return iLen;
      }
      
      LPCSTR getMem()
      {
           return fmt.getMem();
      }
};
typedef META_KEY*                        LPJS_KEY;
typedef LPJS_KEY*                      LPJS_KEY_PTR;

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
enum META_ID
{
     ID_app =0,
     ID_res,
     ID_os,    
     ID_tacc,      // third party account
     ID_model,
     ID_config_id,
     ID_ts,        // time stamp
     ID_op,        // TelephonyManager
     ID_aid,       // android id
     ID_net,       // network status

     ID_agent,
     ID_lang,      // system locate language
     ID_in_i,
     ID_in_o,
     ID_fits,      // fist install time
     ID_gms,
     ID_app_vcode,
     ID_simop,     // vendor of SIM
     ID_pf,        // platform
     ID_fiv,

     ID_gaid,      // google advertising ad 
     ID_latype,    // launch type
     ID_na,        // nation of SIM
     ID_netop,     // TelephonyManager
     ID_pn,        // Build.PRODUCT
     ID_ch,
     ID_mf         // Build.Manufacturer
};

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
struct META_LOG
{
public:
      USHORT                 itemN;
      LPJS_KEY               items;
      
private:
      CData_Mem              fmt;
      INT                    fmtLen;
      
public:
      META_LOG()
      {
           itemN =ID_mf +1;
           
           items =new META_KEY[itemN];
      }
      
      ~META_LOG()
      {
           if ( items !='\0' )
           {
                delete[] items;
                itemN =0;
           }
      }
      
      void toString()
      {
           INT
           maxLen =0;
           
           for ( int i =0; i <itemN; i++ )
           {
                 maxLen +=items[i].key.getLen() +items[i].val.getLen() +5 +3;
           }
           
           if ( fmt.alloc(maxLen) ==TRUE )
           {
                fmtLen =0;

                memcpy(fmt.getMem() +fmtLen, "{", 1 *sizeof(CHAR));
                fmtLen++;

                for ( int i =0; i <itemN; i++ )
                {
                      items[i].toString();
                      
                      if ( fmtLen >1 )
                      {
                           memcpy(fmt.getMem() +fmtLen, ",", 1 *sizeof(CHAR));
                           fmtLen++;
                      }
                      memcpy(fmt.getMem() +fmtLen, 
                             items[i].getMem(), 
                             items[i].getLen() *sizeof(CHAR));
                      
                      fmtLen +=items[i].getLen();
                }

                memcpy(fmt.getMem() +fmtLen, "}\n", 2 *sizeof(CHAR));
                fmtLen +=2;
           }
      }
      
      USHORT getLen()
      {
           return fmtLen;
      }
      
      LPCSTR getMem()
      {
           return fmt.getMem();
      }
};


#endif // !defined(AFX_DATA_META_H__3585B3F6_63ED_42A9_8405_A022CD8D6839__INCLUDED_)
