// cachedata.h: interface for the cachedata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CACHEDATA_H__D8ADC906_0D28_47E1_B01E_2AD488473AD2__INCLUDED_)
#define AFX_CACHEDATA_H__D8ADC906_0D28_47E1_B01E_2AD488473AD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string_util.h"
#include "DataCandidate.h"

struct CACHE_DATA
{
       CACHE_DATA(int _imeId, int _count, wstring _wsKeyinCode)
       {
             imeId       =_imeId;
             count       =_count;
             wsKeyinCode =_wsKeyinCode;
       }

       ~CACHE_DATA()
       {
       }

       bool operator <(const CACHE_DATA& chk) const
       {
            if ( imeId <chk.imeId )
            {
                 return true;
            }
            else if ( wsKeyinCode.compare(chk.wsKeyinCode) <0 )
            {
                 return true;
            }
            return false;
       }

       int              imeId;
       int              count;
       wstring          wsKeyinCode;
       vector<wstring>  candidate;
};


#define MAX_CACHE_DATA       20

class cachedata
{
public:
      set<CACHE_DATA>        data;
      vector<CACHE_DATA>     dchk;
      bool                   enable;

public:
      cachedata();
      virtual ~cachedata();

      void clear()
      {
           data.clear();
           dchk.clear();
      }
      void setEnable(bool flag)
      {
           enable =flag;
      }

      void uncheck(int imeId, wstring wsKeyin);
      void push(int imeId, int count, wstring wsKeyin, wchar_t**Candidates);
      int  check(int imeId, wstring wsKeyin, wchar_t**Candidates, int Begin, int Number);
};

typedef set<CACHE_DATA>::iterator
CACHE_DATA_ITR;

typedef vector<CACHE_DATA>::iterator
CACHE_DATA_ITR_CHK;

#endif // !defined(AFX_CACHEDATA_H__D8ADC906_0D28_47E1_B01E_2AD488473AD2__INCLUDED_)
