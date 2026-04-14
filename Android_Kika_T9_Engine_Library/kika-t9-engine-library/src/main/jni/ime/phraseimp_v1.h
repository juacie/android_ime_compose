// phraseimp_v1.h: interface for the phraseimp_v1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHRASEIMP_V1_H__964C1511_A70F_4207_8500_4440885388E8__INCLUDED_)
#define AFX_PHRASEIMP_V1_H__964C1511_A70F_4207_8500_4440885388E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "phraseimp.h"

class phraseimp_v1 :public phraseimp
{
protected:
      void qsortPos();

      void setupBeginEnd(const wchar_t *wsKeyIn);

private:
      bool                   mWKLen;

      vector<PHRS_V1_POS>    v1wlen; // key len for wildcard

      set<USER_V1_DAT>       udict;

      #if defined(__FOR_NEW_SET)
      set<PHRS_V1_POS>       v1dgrp;
      #else
      vector<PHRS_V1_POS>    v1dgrp;
      #endif

      vector<PHRS_V1_POS>    v1dpos;
      PHRS_V1_POS_VTR        v1dpos_end;
      PHRS_V1_POS_VTR        v1dpos_begin;
      wchar_t                mLastCH;

public:
      phraseimp_v1();
      virtual ~phraseimp_v1();

      virtual long count()
      {
           return vdict.size();
      }

      bool getWildKLen()
      {
           return mWKLen;
      }

      void attachWildKLen(bool flag)
      {
           mWKLen =flag;
      }

      void clear();

      bool loadDict(const char *dictName);

      void clear_u();
      int  total_u() { return udict.size(); }
      void getAll_u(int sID, set<USER_V1_DAT> &All);
      MEM_INT32 getOrderID_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);

      bool checkLimit_u(int limit);
      bool save_u(const char *fName);
      bool load_u(const char *fName);
      bool isExist_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);
      bool remove_u (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);
      bool append_u (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 orderID =INVALID_MEM_OFFSET);

      bool isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);

      int  getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut);

      int  getGrouping(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);

      int  getKeyFuzzyByTimesN(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getKeyFuzzyByTimesNVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, int total =20, bool checkPrior =true);

      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);

      int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);
      int  getKeyWildc(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsWildcard, int total =20);

      int  getIdiom(int sID, const wchar_t *wsKey, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);

      int  getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  getFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20);
      int  get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20, bool fuzzy =false);
};

#endif // !defined(AFX_PHRASEIMP_V1_H__964C1511_A70F_4207_8500_4440885388E8__INCLUDED_)
