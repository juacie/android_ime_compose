// phraseimp_v2_2w.h: interface for the phraseimp_v2_2w class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHRASEIMP_V2_2W_H__72267F61_61E3_4D83_9EAC_487B7A2A0777__INCLUDED_)
#define AFX_PHRASEIMP_V2_2W_H__72267F61_61E3_4D83_9EAC_487B7A2A0777__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "phraseimp_v2_1w.h"

//-----------------------------------------------------------------------------
class phraseimp_v2_2w :public phraseimp_v2_1w
{
private:
      set<V2_2W_HWCHAR_IDX>
      chr_HCharIndex;

      set<V2_2W_HWCHAR_IDX>
      voc_HCharIndex;

      vector<MEM_INT32>
      vdictW_Pos;

public:
      phraseimp_v2_2w();
      virtual ~phraseimp_v2_2w();

      void clear();

      bool loadDict(const char *dictName);

      int  getKeyFuzzyW(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);

      int  getKeyAllFuzzyW(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20);

      int  getKeyFuzzyEx(vector<V2W_DICT_UI> *pV2dict, CMP_FUNC_FLAG flag, int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20);
};

#endif // !defined(AFX_PHRASEIMP_V2_2W_H__72267F61_61E3_4D83_9EAC_487B7A2A0777__INCLUDED_)
