// cachedata.cpp: implementation of the cachedata class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cachedata.h"

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
cachedata::cachedata()
{
     enable =true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
cachedata::~cachedata()
{
     clear();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct findCache
{
       CACHE_DATA            In;

       findCache(CACHE_DATA input) : In(input)
       {
       }

       bool operator()(const CACHE_DATA& chk) const
       {
            int
            cmp =chk.wsKeyinCode.compare(In.wsKeyinCode);

            return cmp ==0;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void cachedata::uncheck(int imeId, wstring wsKeyin)
{
     if ( enable ==false )
          return;

     CACHE_DATA
     ui(imeId, 0, wsKeyin);

     CACHE_DATA_ITR
     ui_found =find_if(data.begin(), data.end(), findCache(ui));

     if ( ui_found !=data.end() )
     {
          data.erase(ui_found);
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  cachedata::check(int imeId, wstring wsKeyin, wchar_t**Candidates, int Begin, int Number)
{
     if ( enable ==false )
          return 0;

     CACHE_DATA
     ui(imeId, 0, wsKeyin);

     CACHE_DATA_ITR
     ui_found =find_if(data.begin(), data.end(), findCache(ui));

     if ( ui_found !=data.end() )
     {
          int
          total =0;

          for ( int idx =0; idx <ui_found->count; idx++ )
          {
                if ( idx <Begin )
                     continue;

                if ( total >=Number )
                     break;

                wcscpy(Candidates[total++], ui_found->candidate[idx].c_str());
          }
          return total;
     }

     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void cachedata::push(int imeId, int count, wstring wsKeyin, wchar_t**Candidates)
{
     if ( enable ==false )
          return;

     if ( data.size() >MAX_CACHE_DATA )
     {
          CACHE_DATA_ITR_CHK
          first =dchk.begin();

          CACHE_DATA
          ui =first[0];

          CACHE_DATA_ITR
          ui_found =find_if(data.begin(), data.end(), findCache(first[0]));

          if ( ui_found !=data.end() )
          {
               data.erase(data.begin());
               dchk.erase(dchk.begin());
          }
     }

     CACHE_DATA
     ui(imeId, count, wsKeyin);

     for ( int idx =0; idx <count; idx++ )
     {
           ui.candidate.push_back(Candidates[idx]);
     }
     data.insert(ui);
     dchk.push_back(ui);
}

