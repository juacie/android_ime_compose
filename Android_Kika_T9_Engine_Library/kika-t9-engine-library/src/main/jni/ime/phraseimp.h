// phraseimp.h: interface for the phraseimp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHRASEIMP_H__9F709165_915F_451B_913F_2752B774D827__INCLUDED_)
#define AFX_PHRASEIMP_H__9F709165_915F_451B_913F_2752B774D827__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string_util.h"
#include "data_ui.h"
#include "vcandidate.h"

#define SPLIT_NUMBER_KEY               L"##"
#define SPLIT_NUMBER_HW_KEY            L"@@"

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
enum CMP_FUNC_FLAG
{
     cmp_for_T9,
     cmp_for_Qwerty,
     cmp_for_HeadMapping,
     cmp_for_FullMatchQwerty,
     cmp_for_FullMatchHeadMapping
};

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class phraseimp
{
private:
      bool                   isQwerty;

protected:
      static short           wrdLen;
      static short           keyLen;
      static short           nwdLen;
      string                 dictName;

      //-----------------------------------------------------------------------
      vector<PHRS_UI>        vdict;
      vector<IDIOM_UI>       vnext;

      bool                   preLoad;
      FMT_SORT_ID            sortID;

      LPMEM_UI               memPhrs;
      LPMEM_UI               memKeys;
      LPMEM_UI               memNext;

protected:
      void fillData(vector<PHRS_UI> &vfrom, vector<wstring> &vto);
      void fillDataVc(vector<PHRS_UI> &vfrom, vector<VCandidate> &vto, bool isFull =false);
      void fillDataVc2(vector<VCandidate> &vfrom, vector<VCandidate> &vto, bool isFull =false);
      void fillIdiomData(const wchar_t *wsKeys, vector<PHRS_UI> &vfrom, vector<wstring> &vto, int cmpLen);
      void fillGroupingData(const wchar_t *wsKeys, vector<PHRS_UI> &vfrom, vector<wstring> &vto, int cmpLen);

      bool appendDirectly(int sID, const wstring wsKeys, const wstring wsPhrs, const MEM_INT32 isNext);

      void qsort();
      void qsortAndUnique();

public:
      phraseimp();
      virtual ~phraseimp();

      //-----------------------------------------------------------------------
      // for append user word
      //-----------------------------------------------------------------------
      MEM_INT32 getTimeStamp();

      //-----------------------------------------------------------------------
      bool getQwerty()
      {
           return isQwerty;
      }

      void setQwerty(bool val)
      {
           isQwerty =val;
      }

      //-----------------------------------------------------------------------
      bool getPreLoad()
      {
           return preLoad;
      }

      void setPreLoad(bool val)
      {
           preLoad =val;
      }

      //-----------------------------------------------------------------------
      short maxPhrLen()
      {
          return wrdLen;
      }
      void setMaxPhrLen(short val)
      {
           wrdLen =val;
      }

      //-----------------------------------------------------------------------
      short maxKeyLen()
      {
           return keyLen;
      }
      void setMaxKeyLen(short val)
      {
          keyLen =val;
      }

      //-----------------------------------------------------------------------
      short maxNwdLen()
      {
          return nwdLen;
      }
      void setMaxNwdLen(short val)
      {
           nwdLen =val;
      }

      //-----------------------------------------------------------------------
      const char* getDictName()
      {
           return dictName.c_str();
      }

      void setDictName(const char* fname)
      {
           dictName =fname;
      }

      //-----------------------------------------------------------------------
      void setSortID(FMT_SORT_ID sortID)
      {
           this->sortID =sortID;
      }

      //-----------------------------------------------------------------------
      std::wstring encryptDecrypt(const wchar_t* msg, const char* key);
      std::wstring encryptDecryptEx(const wchar_t* msg, const char* key);

      std::wstring decrypt(std::wstring msg, const std::string key);
      std::wstring encrypt(std::wstring msg, const std::string key);

      //-----------------------------------------------------------------------
      virtual void clear();

      virtual bool phrsSaveData(const char *fName);
      virtual int  phrsLoadData(const char *fName);

      virtual void clear_u() { }
      virtual int  total_u() { return 0; }
      virtual void getAll_u(int sID, set<USER_V1_DAT> &All) { }
      virtual MEM_INT32 getOrderID_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs) { return 0; }

      virtual bool checkLimit_u(int limit) { return false; }
      virtual bool save_u(const char *fName) { return false; }
      virtual bool load_u(const char *fName) { return false; }
      virtual bool isExist_u(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs) { return false; }
      virtual bool remove_u (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs) { return false; }
      virtual bool append_u (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 orderID =INVALID_MEM_OFFSET)  { return false; }

      virtual bool isExistMain(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs) { return false; }

      virtual bool isExist(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);
      virtual bool remove (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);
      virtual bool append (int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 isNext =INVALID_MEM_OFFSET);

      int  getMemData_Phrs(MEM_INT32 iOffset, wstring &wsOut);
      int  getMemData_Keys(MEM_INT32 iOffset, wstring &wsOut);
      int  getMemData_Next(MEM_INT32 iOffset, wstring &wsOut);
      int  getMemData_HVoc(MEM_INT32 iOffset, wstring &wsOut);

      //-----------------------------------------------------------------------
      int  getMemData_Phrs(V2_DICT_UI &ui, wstring &wsOut);
      int  getMemData_Keys(V2_DICT_UI &ui, wstring &wsOut);

      int  getMemData_Phrs(V2_DICT_UI_ITR itr, wstring &wsOut);
      int  getMemData_Keys(V2_DICT_UI_ITR itr, wstring &wsOut);

      int  getMemData_CPhrs(const V2_DICT_UI &ui, wstring &wsOut);
      int  getMemData_CKeys(const V2_DICT_UI &ui, wstring &wsOut);

      //-----------------------------------------------------------------------
      int  getMemData_Phrs(V2W_DICT_UI &ui, wstring &wsOut);
      int  getMemData_Keys(V2W_DICT_UI &ui, wstring &wsOut);

      int  getMemData_Phrs(V2W_DICT_UI_ITR itr, wstring &wsOut);
      int  getMemData_Keys(V2W_DICT_UI_ITR itr, wstring &wsOut);

      int  getMemData_CPhrs(const V2W_DICT_UI &ui, wstring &wsOut);
      int  getMemData_CKeys(const V2W_DICT_UI &ui, wstring &wsOut);

      int  getMemData_QKey(V2W_DICT_UI &ui, wstring &wsOut);
      int  getMemData_QKey(V2W_DICT_UI_ITR itr, wstring &wsOut);
      int  getMemData_CQKey(const V2W_DICT_UI &ui, wstring &wsOut);

      int  getMemData_HVoc(V2W_DICT_UI &ui, wstring &wsOut);
      int  getMemData_HKey(V2W_DICT_UI &ui, wstring &wsOut);

      int  getMemData_HVoc(V2W_DICT_UI_ITR itr, wstring &wsOut);
      int  getMemData_HKey(V2W_DICT_UI_ITR itr, wstring &wsOut);

      int  getMemData_CHVoc(const V2W_DICT_UI &ui, wstring &wsOut);
      int  getMemData_CHKey(const V2W_DICT_UI &ui, wstring &wsOut);

      //-----------------------------------------------------------------------
      int  getID(const wchar_t *wsKeys);
      int  getData(int sID, wstring &v1, wstring &v2, MEM_INT32 &v3);

      virtual long count() { return 0; }

      virtual void qsortPos() { }

      virtual void attachWildKLen(bool flag) { }

      virtual bool getWildKLen() { return false; }

      virtual bool loadDict(const char *dictName) { return false; }

      virtual int  getMemData(LPCMEM_CHR pMemData, MEM_INT32 iOffset, wstring &wsOut) { return 0; }

      virtual int  getVocKey(int sID, const wchar_t *wsVoc, vector<wstring> &v) { return 0; }
      virtual int  deComposingKey(int sID, const wchar_t *wsVoc, vector<wstring> &v) { return 0; }

      virtual int  addGrouping(int sID, const wchar_t *wsKeys) { return 0; }
      virtual int  getGrouping(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20) { return 0; }

      virtual int  getKeyFuzzyByTimesN(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20) { return 0; }
      virtual int  getKeyFuzzyByTimesNVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, int total =20, bool checkPrior =true) { return 0; }

      virtual int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20) { return 0; }
      virtual int  getKeyFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }

      virtual int  getKeyFuzzyH(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }
      virtual int  getKeyFuzzyHVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }

      virtual int  getKeyFuzzyW(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }
      virtual int  getKeyAllFuzzyW(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }

      virtual int  getKeyFuzzyWVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }
      virtual int  getKeyFuzzyWVcTW(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20, bool checkFull =true) { return 0; }

      virtual int  getKeyFullMatchW(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }

      virtual int  getKeyFullMatchH(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }
      virtual int  getKeyFullMatchWVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20, bool checkFull =true) { return 0; }

      virtual int  getKeyFuzzyEx(vector<V2W_DICT_UI> *pV2dict, CMP_FUNC_FLAG flag, int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }

      virtual int  getKeyChrFullVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20, bool checkFull =true) { return 0; }
      virtual int  getKeyVocFullVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20, bool checkFull =true) { return 0; }
      virtual int  getKeyNumFullVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20, bool checkFull =true) { return 0; }
      virtual int  getKeyNumHeadVc(int sID, const wchar_t *wsKeys, vector<VCandidate> &v, const wchar_t *wsKeyMatch, int total =20, bool checkFull =true) { return 0; }

      virtual int  getKeyWildc(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsWildcard, int total =20) { return 0; }

      virtual int  getIdiom(int sID, const wchar_t *wsKeys, vector<wstring> &v, const wchar_t *wsKeyMatch, int total =20) { return 0; }

      virtual int  getKey(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20) { return 0; }
      virtual int  getFuzzy(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20) { return 0; }
      virtual int  get(int sID, const wchar_t *wsKeys, vector<wstring> &v, int total =20, bool fuzzy =false) { return 0; }
};

#endif // !defined(AFX_PHRASEIMP_H__9F709165_915F_451B_913F_2752B774D827__INCLUDED_)
