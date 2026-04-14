// phraseimp_v3.h: interface for the phraseimp_v3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHRASEIMP_V3_H__9F189D14_AF9B_4720_AE94_28203D57C6B9__INCLUDED_)
#define AFX_PHRASEIMP_V3_H__9F189D14_AF9B_4720_AE94_28203D57C6B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "phraseimp.h"

class phraseimp_v3 :public phraseimp
{
protected:
      void qsortPos();

private:
      vector<PHRS_V3_POS>    v3klen; // key len
      vector<V2_DICT_UI>     v3dict;

      #if defined(__FOR_NEW_SET)
      set<PHRS_V2_POS>       v2dgrp_dynload;
      set<PHRS_UI>           v3dgrp_preload;
      #else
      vector<PHRS_V2_POS>    v2dgrp_dynload;
      vector<PHRS_UI>        v3dgrp_preload;
      #endif

      vector<PHRS_V3_POS>    v3oSID;
      PHRS_V3_POS_ITR        v3oSID_end;
      PHRS_V3_POS_ITR        v3oSID_begin;

      vector<PHRS_V3_POS>    v3kSID;
      PHRS_V3_POS_ITR        v3kSID_end;
      PHRS_V3_POS_ITR        v3kSID_begin;
      wchar_t                mLastCH;

      void setupOSID();
      void setupBeginEnd(const wchar_t *wsKeyIn, const wchar_t *wsWildcard);

public:
      phraseimp_v3();
      virtual ~phraseimp_v3();

      void clear();

      //-----------------------------------------------------------------------
      long count()
      {
           return v3dict.size();
      }

      bool loadDict(const char *dictName);

      bool isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);

      int  getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut);

      int  getGrouping_dynamic(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getGrouping_preload(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getGrouping(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);

      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);
      int  getKeyWildc(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsWildcard, int total =20);

      int  getIdiom(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);

      int  getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20, bool fuzzy =false);
};

#endif // !defined(AFX_PHRASEIMP_V3_H__9F189D14_AF9B_4720_AE94_28203D57C6B9__INCLUDED_)
