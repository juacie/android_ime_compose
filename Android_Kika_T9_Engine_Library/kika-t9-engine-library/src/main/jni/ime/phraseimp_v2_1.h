// phraseimp_v2_1.h: interface for the phraseimp_v2_1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHRASEIMP_V2_1_H__7BC190E2_4969_4065_AD47_B74759A5D187__INCLUDED_)
#define AFX_PHRASEIMP_V2_1_H__7BC190E2_4969_4065_AD47_B74759A5D187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "phraseimp.h"

class phraseimp_v2_1 :public phraseimp
{
protected:
      void qsortPos();

protected:
      vector<V2_DICT_UI>     v2dict;

      set<PHRS_V2_POS>       udict;

      #if defined(__FOR_NEW_SET)
      set<PHRS_V2_POS>       v2dgrp_dynload;
      set<PHRS_UI>           v2dgrp_preload;
      #else
      vector<PHRS_V2_POS>    v2dgrp_dynload;
      vector<PHRS_UI>        v2dgrp_preload;
      #endif

      bool loadDictV2_Base(const char *dictName, const char * HeadID, int IDSize);

public:
      phraseimp_v2_1();
      virtual ~phraseimp_v2_1();

      virtual long count()
      {
           return v2dict.size();
      }

      virtual void clear();

      virtual bool loadDict(const char *dictName);

      bool isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);

      #if defined(__TRY_NEW_2_1)
      void clear_u();
      bool isExist_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);
      bool remove_u (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);
      bool append_u (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 orderID =INVALID_MEM_OFFSET);
      #endif

      int  getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut);

      int  getGrouping_dynamic(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getGrouping_preload(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getGrouping(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  addGrouping(int sID, const wchar_t *wsKeys);

      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);
      int  getKeyWildc(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsWildcard, int total =20);

      int  getIdiom(int sID, const wchar_t *wsKey, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);

      int  getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20, bool fuzzy =false);
};

#endif // !defined(AFX_PHRASEIMP_V2_1_H__7BC190E2_4969_4065_AD47_B74759A5D187__INCLUDED_)
