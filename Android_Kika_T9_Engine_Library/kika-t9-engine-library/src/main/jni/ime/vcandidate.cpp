// vcandidate.cpp: implementation of the vcandidate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vcandidate.h"

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_sameWord
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            if ( a.ws.compare(b.ws) ==0 )
                 return true;

            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool VCandidate::operator ==(const VCandidate& rd) const
{
     wstring::const_iterator lit = ws.begin(), rit = rd.ws.begin();

      for ( ;lit != ws.end() && rit != rd.ws.end(); ++lit, ++rit )
      {
            if ( tolower( *lit ) !=tolower( *rit ) )
                 return false;
      }
      if ( ws.size() !=rd.ws.size() )
      {
           return false;
      }
      return true;
}


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool VCandidate::operator <(const VCandidate& rd) const
{
     int
     cmp =ws.compare(rd.ws);

     if ( cmp ==0 )
     {
          return order <rd.order;
     }
     return cmp <0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_order
{
       template<class It>
       bool operator()(It const &a, It const &b) const
       {
            if ( a.vtype >b.vtype )
                 return true;

            if ( a.order <b.order )
                 return true;

            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vcandidate_applyRule(vector<VCandidate> &v)
{
     if ( v.size() >1 )
     {
          std::sort(v.begin(), v.end());
          v.erase(std::unique(v.begin(), v.end(), target_sameWord()), v.end());

          std::sort(v.begin(), v.end(), target_order());
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_sameWordEx
{
       int                   cmpLen;

       target_sameWordEx(int _cmpLen) :cmpLen(_cmpLen)
       {
       }

       bool operator()(VCandidate const &a, VCandidate const &b) const
       {
            if ( a.ws.substr(0, cmpLen).compare(b.ws.substr(0, cmpLen)) ==0 )
                 return true;

            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vcandidate_applyRuleEx(vector<VCandidate> &v, int cmpLen)
{
     if ( v.size() >1 )
     {
          std::sort(v.begin(), v.end());
          v.erase(std::unique(v.begin(), v.end(), target_sameWordEx(cmpLen)), v.end());

          std::sort(v.begin(), v.end(), target_order());
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_orderFullStrokesV
{
       bool operator()(VCandidate const &a, VCandidate const &b) const
       {
            if ( a.vtype >b.vtype )
                 return true;

            if ( a.sID <b.sID )
                 return true;

//            if ( a.order <b.order )
//                 return true;

            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vcandidate_applyRuleFullStroke(vector<VCandidate> &v, int cmpLen)
{
     if ( v.size() >1 )
     {
          std::sort(v.begin(), v.end());
          v.erase(std::unique(v.begin(), v.end(), target_sameWordEx(cmpLen)), v.end());

          std::sort(v.begin(), v.end(), target_orderFullStrokesV());
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_sameWordFullStrokesX
{
       int                   cmpLen;

       target_sameWordFullStrokesX(int _cmpLen) :cmpLen(_cmpLen)
       {
       }

       bool operator()(Combine_Strokes_UI const &a, Combine_Strokes_UI const &b) const
       {
            if ( a.wWord.substr(0, cmpLen).compare(b.wWord.substr(0, cmpLen)) ==0 )
                 return true;

            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_orderFullStrokesX
{
       bool operator()(Combine_Strokes_UI const &a, Combine_Strokes_UI const &b) const
       {
            if ( a.sID <b.sID )
                 return true;

//            if ( a.order <b.order )
//                 return true;

            return false;
       }
};

bool Combine_Strokes_UI::operator <(const Combine_Strokes_UI & rd) const
{
     if ( isFullMatch ==true && rd.isFullMatch ==false )
     {
          return true;
     }
     else if ( isFullMatch ==false && rd.isFullMatch ==true )
     {
          return false;
     }
     else if ( isFullMatch ==rd.isFullMatch )
     {
          int
          cmp =wKeyin.compare(rd.wKeyin);
          if ( cmp <0 )
          {
               return false;
          }
          cmp =wStroke.compare(rd.wStroke);
          if ( cmp <0 )
          {
               return false;
          }
     }
     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void vcandidate_applyRuleFullStroke(vector<Combine_Strokes_UI> &v, int cmpLen)
{
     if ( v.size() >1 )
     {
          //std::sort(v.begin(), v.end());
          v.erase(std::unique(v.begin(), v.end(), target_sameWordFullStrokesX(cmpLen)), v.end());

          std::sort(v.begin(), v.end(), target_orderFullStrokesX());
     }
}
