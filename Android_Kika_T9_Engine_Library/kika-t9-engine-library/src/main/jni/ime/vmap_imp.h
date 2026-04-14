// vmap_imp.h: interface for the vmap_imp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMAP_IMP_H__2D406B37_7EFF_4245_BFA2_4A9B50698123__INCLUDED_)
#define AFX_VMAP_IMP_H__2D406B37_7EFF_4245_BFA2_4A9B50698123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "debug_util.h"
#include "string_util.h"

#define VMAP_FILE_ID                   L"bict1"

typedef unsigned short                 MEM_TOTAL;


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct mapVoca
{
       int                   imeID;
       wstring               wVoca;

       mapVoca(const wchar_t * _wVoca, int _imeID)
       {
            imeID  =_imeID;
            wVoca  =_wVoca;
       }

       bool operator ==(const mapVoca & obj) const
       {
            if ( wVoca == obj.wVoca )
                 return true;
            else
                 return false;
       }

       int  getId() const
       {
            return imeID;
       }

       std::wstring getVoca() const
       {
            return wVoca;
       }
};

namespace std
{
     template<>
     struct hash<mapVoca>
     {
            size_t
            operator()(const mapVoca & obj) const
            {
                 return hash<wstring>()(obj.getVoca());
            }
     };
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
struct VMAP_UI
{
       wstring               wVoca;

       VMAP_UI(const wchar_t *_wsIn)
       {
             wVoca =_wsIn;
       }

       VMAP_UI(const wstring _wsIn)
       {
             wVoca =_wsIn;
       }

       bool operator <(const VMAP_UI& chk) const
       {
            if ( wVoca <chk.wVoca )
            {
                 return true;
            }
            return false;
       }
};
typedef VMAP_UI*             LPVMAP_UI;

typedef set<wstring>
VMAP_SC;

typedef vector<wstring>
VMAP_VC;

typedef set<wstring>::iterator
VMAP_VC_ITR;

typedef unordered_set<mapVoca>::iterator
MAP_VOCA_ITR;

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class vmap_imp
{
private:
      MEM_TOTAL              mTotal;
      unordered_set<mapVoca> mVc;

public:
      vmap_imp();
      virtual ~vmap_imp();

      int  loadDict(const char* dictName);

      void clear();

      void erase(const wchar_t *wsVoca);
      void append(const wchar_t *wsVoca, long offset =0);

      void erase(const char *sVoca_utf8);
      void append(const char *sVoca_utf8, long offset =0);

      bool isExist(const wchar_t* wsVoca);

      void encrypt(wstring &wVoca);
};

#endif // !defined(AFX_VMAP_IMP_H__2D406B37_7EFF_4245_BFA2_4A9B50698123__INCLUDED_)
