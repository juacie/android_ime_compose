// data_korean2.h: interface for the data_korean2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_KOREAN2_H__287E3B56_EE54_40B1_A5E3_4CDCE6EDF557__INCLUDED_)
#define AFX_DATA_KOREAN2_H__287E3B56_EE54_40B1_A5E3_4CDCE6EDF557__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "data_korean.h"

struct
mapData_Korean2
{
     MEM_INT                 cb[2];

     void set(wstring wKeyin)
     {
          for ( unsigned i =0; i <2; i++ )
          {
               if ( wKeyin.size() >i )
                    cb[i] =wKeyin[i];
               else
                    cb[i] =0;
          }
     }

     bool operator ==(const mapData_Korean2 & chk) const
     {
          for ( unsigned i =0; i <2; i++ )
          {
               if ( cb[i] !=chk.cb[i] )
                    return false;
          }
          return true;
     }

     bool operator <(const mapData_Korean2& chk) const
     {
          for ( unsigned i =0; i <2; i++ )
          {
               if ( cb[i] <chk.cb[i] )
                    return true;

               if ( chk.cb[i] >=cb[i] )
                    return true;
          }
          return false;
     }
};

typedef vector<mapData_Korean2>::iterator
S_mapData_Korean2;

//-----------------------------------------------------------------------------
struct
mapData_Korean5
{
     MEM_INT                 cb[5];

     void set(wstring wKeyin)
     {
          for ( unsigned i =0; i <5; i++ )
          {
               if ( wKeyin.size() >i )
                    cb[i] =wKeyin[i];
               else
                    cb[i] =0;
          }
     }

     bool operator ==(const mapData_Korean5 & chk) const
     {
          for ( unsigned i =0; i <5; i++ )
          {
               if ( cb[i] !=chk.cb[i] )
                    return false;
          }
          return true;
     }

     bool operator <(const mapData_Korean5& chk) const
     {
          for ( unsigned i =0; i <5; i++ )
          {
               if ( cb[i] <chk.cb[i] )
                    return true;

               if ( chk.cb[i] >=cb[i] )
                    return true;
          }
          return false;
     }
};

typedef vector<mapData_Korean5>::iterator
S_mapData_Korean5;

//-----------------------------------------------------------------------------
struct
mapData_Korean8
{
     MEM_INT                 cb[8];

     void set(wstring wKeyin)
     {
          for ( unsigned i =0; i <8; i++ )
          {
               if ( wKeyin.size() >i )
                    cb[i] =wKeyin[i];
               else
                    cb[i] =0;
          }
     }

     bool operator ==(const mapData_Korean8 & chk) const
     {
          for ( unsigned i =0; i <8; i++ )
          {
               if ( cb[i] !=chk.cb[i] )
                    return false;
          }
          return true;
     }

     bool operator <(const mapData_Korean8& chk) const
     {
          for ( unsigned i =0; i <8; i++ )
          {
               if ( cb[i] <chk.cb[i] )
                    return true;

               if ( chk.cb[i] >=cb[i] )
                    return true;
          }
          return false;
     }
};

typedef vector<mapData_Korean8>::iterator
S_mapData_Korean8;

extern mapData_Korean5       vowel5[];
extern mapData_Korean2       nocut2[];
extern mapData_Korean8       nocut8[];

#endif // !defined(AFX_DATA_KOREAN2_H__287E3B56_EE54_40B1_A5E3_4CDCE6EDF557__INCLUDED_)
