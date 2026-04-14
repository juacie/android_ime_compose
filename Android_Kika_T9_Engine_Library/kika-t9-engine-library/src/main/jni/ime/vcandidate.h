// vcandidate.h: interface for the vcandidate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VCANDIDATE_H__FADF4C1D_A462_40D0_B328_7BF3F9F0FBDA__INCLUDED_)
#define AFX_VCANDIDATE_H__FADF4C1D_A462_40D0_B328_7BF3F9F0FBDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string_util.h"

enum VTYPE_ID
{
     VTYPE_MAIN =1,
     VTYPE_USER =2,
     VTYPE_PHRS =3,
     VTYPE_PRIO =4
};

struct VCandidate
{
       wstring               ws;
       wstring               wKey;
       wstring               wStroke;
       bool                  isFullMatch;
       unsigned short        sID;
       unsigned short        order;
       unsigned char         vtype;
       unsigned int          timeID;

       VCandidate()
       {
            ws          =L"";
            wKey        =L"";
            wStroke     =L"";
            sID         =-1;
            order       =-1;
            vtype       =-1;
            timeID      =-1;
            isFullMatch =false;
       }

       VCandidate(wstring _ws, wstring _key, int _sID, int _index, int _vtype =0, bool _isFullMatch =false, unsigned int _timeID =-1)
       {
            ws          =_ws;
            wKey        =_key;
            sID         =_sID;
            order       =_index;
            vtype       =_vtype;
            wStroke     =L"";
            timeID      =_timeID;
            isFullMatch =_isFullMatch;
       }

       VCandidate(wstring _ws, int _index, int _vtype =0, bool _isFullMatch =false, unsigned int _timeID =-1)
       {
            ws          =_ws;
            order       =_index;
            vtype       =_vtype;

            wKey        =L"";
            wStroke     =L"";
            sID         =-1;
            timeID      =_timeID;
            isFullMatch =_isFullMatch;
       }

       void setSID(int _sID)
       {
           sID =_sID;
       }

       void setTimeID(unsigned int _timeID)
       {
           timeID =_timeID;
       }

       void setKey(const wchar_t *_key)
       {
           wKey =_key;
       }

       void setStroke(const wchar_t *_stroke)
       {
           wStroke =_stroke;
       }

       void setFullMatch(bool _isFullMatch)
       {
           isFullMatch =_isFullMatch;
       }

       bool operator ==(const VCandidate& rd) const;

       bool operator <(const VCandidate& rd) const;
};

//-----------------------------------------------------------------------------
// for Combine Keyin
//-----------------------------------------------------------------------------
struct Combine_Strokes_UI
{
       int                   sID;
       unsigned char         vtype;
       wstring               wWord;
       wstring               wKeyin;
       wstring               wStroke;
       unsigned int          timeID;
       bool                  isFullMatch;

       Combine_Strokes_UI(const wchar_t* _wsWord, const wchar_t* _wsKeyin, const wchar_t* _wsStroke, int _sID,
                          bool _isFullMatch =false, int _vtype =0, unsigned int _timeID =-1)
       {
            sID         =_sID;
            vtype       =_vtype;
            wWord       =_wsWord;

            wKeyin      =_wsKeyin;
            wStroke     =_wsStroke;
            isFullMatch =_isFullMatch;
            timeID      =_timeID;
       }

       Combine_Strokes_UI(wstring _wWord, wstring _wKeyin, wstring _wStroke, int _sID,
                          bool _isFullMatch =false, int _vtype =0, unsigned int _timeID =-1)
       {
            sID         =_sID;
            vtype       =_vtype;
            wWord       =_wWord;

            wKeyin      =_wKeyin;
            wStroke     =_wStroke;
            isFullMatch =_isFullMatch;
            timeID      =_timeID;
       }

       void setTimeID(unsigned int _timeID)
       {
            timeID =_timeID;
       }

       void setFullMatch(bool _isFullMatch)
       {
            isFullMatch =_isFullMatch;
       }

       bool operator <(const Combine_Strokes_UI & rd) const;
};

void vcandidate_applyRule(vector<VCandidate> &v);
void vcandidate_applyRuleEx(vector<VCandidate> &v, int cmpLen =1);
void vcandidate_applyRuleFullStroke(vector<VCandidate> &v, int cmpLen =1);
void vcandidate_applyRuleFullStroke(vector<Combine_Strokes_UI> &v, int cmpLen =1);
void vcandidate_applyRuleBySemicolumn(vector<VCandidate> &v);


#endif // !defined(AFX_VCANDIDATE_H__FADF4C1D_A462_40D0_B328_7BF3F9F0FBDA__INCLUDED_)
