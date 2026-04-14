// phraseimp_v2.h: interface for the phraseimp_v2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHRASEIMP_V2_H__4274418D_1455_4A3B_BCCD_F72E771587BE__INCLUDED_)
#define AFX_PHRASEIMP_V2_H__4274418D_1455_4A3B_BCCD_F72E771587BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "phraseimp.h"

class phraseimp_v2 :public phraseimp
{
protected:
      void qsortPos();

private:
      vector<PHRS_V1_POS>    v1dpos;
      vector<PHRS_V1_POS>    v2klen; // key len for wildcard
      vector<V2_DICT_UI>     v2dict;

      #if defined(__FOR_NEW_SET)
      set<PHRS_V2_POS>       v2dgrp_dynload;
      set<PHRS_UI>           v2dgrp_preload;
      #else
      vector<PHRS_V2_POS>    v2dgrp_dynload;
      vector<PHRS_UI>        v2dgrp_preload;
      #endif

public:
      phraseimp_v2();
      virtual ~phraseimp_v2();

      virtual long count()
      {
           return vdict.size();
      }

      void clear();

      bool loadDict(const char *dictName);

      bool isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);

      int  getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut);

      int  getGrouping_dynamic(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getGrouping_preload(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getGrouping(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);

      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);
      int  getKeyWildc(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsWildcard, int total =20);

      int  getIdiom(int sID, const wchar_t *wsKey, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);

      int  getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20, bool fuzzy =false);
};

#endif // !defined(AFX_PHRASEIMP_V2_H__4274418D_1455_4A3B_BCCD_F72E771587BE__INCLUDED_)
