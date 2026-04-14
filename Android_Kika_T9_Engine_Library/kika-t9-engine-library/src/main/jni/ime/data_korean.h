// data_korean.h: interface for the data_korean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_KOREAN_H__9E5CFB35_F259_4357_A151_7D237F4457C2__INCLUDED_)
#define AFX_DATA_KOREAN_H__9E5CFB35_F259_4357_A151_7D237F4457C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string_util.h"
#include "data_ui.h"

#define INVALID_DATA         0

//-----------------------------------------------------------------------------
struct
mapL_Base
{
     MEM_INT                 k1; // key1
     MEM_INT                 cb; // combine code

     void set(MEM_INT _k1, MEM_INT _cb)
     {
          k1 =_k1;
          cb =_cb;
     }
};

//-----------------------------------------------------------------------------
struct
mapData_Korean3
{
     MEM_INT                 k1; // key1
     MEM_INT                 k2; // key2
     MEM_INT                 cb; // combine code

     void set(MEM_INT _k1, MEM_INT _k2 =INVALID_DATA, MEM_INT _cb =INVALID_DATA)
     {
          k1 =_k1;
          k2 =_k2;
          cb =_cb;
     }

     void set(wstring wKeyin)
     {
          if ( wKeyin.size() >0 )
               k1 =wKeyin[0];

          if ( wKeyin.size() >1 )
               k2 =wKeyin[1];
          else
               k2 =INVALID_DATA;
     }

     bool operator <(const mapData_Korean3& chk) const
     {
          if ( k1 <chk.k1 )
          {
               return true;
          }
          else if ( k2 <chk.k2 )
          {
               return true;
          }
          return false;
     }
};

struct
mapData_Korean4
{
     MEM_INT                 k1; // key1
     MEM_INT                 k2; // key2
     MEM_INT                 k3; // key3
     MEM_INT                 cb; // combine code

     void set(MEM_INT _k1, MEM_INT _k2 =INVALID_DATA, MEM_INT _k3 =INVALID_DATA, MEM_INT _cb =INVALID_DATA)
     {
          k1 =_k1;
          k2 =_k2;
          k3 =_k3;
          cb =_cb;
     }

     void set(wstring wKeyin)
     {
          if ( wKeyin.size() >0 )
               k1 =wKeyin[0];

          if ( wKeyin.size() >1 )
               k2 =wKeyin[1];
          else
               k2 =INVALID_DATA;
          
          if ( wKeyin.size() >2 )
               k3 =wKeyin[2];
          else
               k3 =INVALID_DATA;
     }

     bool operator ==(const mapData_Korean4 & chk) const
     {
          if ( k1 ==chk.k1 )
               return true;
          else
               return false;
     }

     bool operator <(const mapData_Korean4& chk) const
     {
          if ( k1 <chk.k1 )
          {
               return true;
          }
          else if ( k2 <chk.k2 )
          {
               return true;
          }
          else if ( k3 <chk.k3 )
          {
               return true;
          }

          return false;
     }
};

struct
mapData_Korean4_Base
{
     MEM_INT                 k1; // key1
     MEM_INT                 k2; // key2
     MEM_INT                 k3; // key3
     MEM_INT                 cb; // combine code

     void set(MEM_INT _cb)
     {
          cb =_cb;
     }

     bool operator ==(const mapData_Korean4_Base & chk) const
     {
          if ( cb ==chk.cb )
               return true;
          else
               return false;
     }

     bool operator <(const mapData_Korean4_Base& chk) const
     {
          if ( cb <chk.cb )
          {
               return true;
          }
          return false;
     }
};

extern mapData_Korean4_Base  lvBase[];
extern mapData_Korean4       lv0[];
extern mapData_Korean3       lv[];
extern mapData_Korean3       lvl[];
extern mapData_Korean4       lvll[];

#endif // !defined(AFX_DATA_KOREAN_H__9E5CFB35_F259_4357_A151_7D237F4457C2__INCLUDED_)
