// trans_key_korean.h: interface for the trans_key_korean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANS_KEY_KOREAN_H__2573FE31_A8DC_4157_9A40_8244D8F6AEB5__INCLUDED_)
#define AFX_TRANS_KEY_KOREAN_H__2573FE31_A8DC_4157_9A40_8244D8F6AEB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "data_korean2.h"

//-----------------------------------------------------------------------------
#define   KOREAN_TEXT_NUM              33
#define   LEAD                         19
#define   VOWEL                        21
#define   TAIL                         28

//-----------------------------------------------------------------------------
class trans_key_korean
{
public:
      set<MEM_INT>              mV;
      set<MEM_INT>              mL;
      set<MEM_INT>              mKrUni;
      set<MEM_INT>              mKrSpecial;
      
      vector<mapData_Korean4_Base>   
      mLBbase;

      vector<mapData_Korean4>   mL0;
      vector<mapData_Korean3>   mLV;
      vector<mapData_Korean3>   mLVL;
      vector<mapData_Korean4>   mLVLL;
      vector<mapData_Korean5>   mVowel5;
      vector<mapData_Korean2>   mKeepRule2;
      vector<mapData_Korean8>   mKeepRule8;
      
      wstring                   mwCompText;
      
public:
      trans_key_korean();
      virtual ~trans_key_korean();
      
      void clear();
      void initData();

      void deleteKey(wstring &wKeyin);
      int  backspace(wstring &wKeyin);
      int  backspace(wchar_t* wsKeyin);

      wstring getComposingText_Head(wstring wKeyin);
      wstring getComposingText_Tail(wstring wKeyin);
      wstring getComposingText(wstring wKeyin, bool chk_seperate, bool chk_ui_Action =false);
      
      int  getComposingText_L0(wstring wKeyin, wstring &wOut);

      int  getComposingText_LV(wstring wKeyin, wstring &wOut);
      int  getComposingText_LVL(wstring wKeyin, wstring &wOut);
      int  getComposingText_LVLL(wstring wKeyin, wstring &wOut);

      int  deComposingText(wstring wKeyin, wstring &wOut);
      int  deComposingText_L3(vector<mapData_Korean3>* p, wstring wKeyin, wstring &wOut);
      int  deComposingText_L4(vector<mapData_Korean4>* p, wstring wKeyin, wstring &wOut);
      int  deComposingText_L4Base(vector<mapData_Korean4_Base>* p, wstring wKeyin, wstring &wOut);
};

typedef vector<mapData_Korean3>::iterator
S_mapData_Korean3;

typedef vector<mapData_Korean4>::iterator
S_mapData_Korean4;

typedef vector<mapData_Korean4_Base>::iterator
S_mapData_Korean4_Base;

#endif // !defined(AFX_TRANS_KEY_KOREAN_H__2573FE31_A8DC_4157_9A40_8244D8F6AEB5__INCLUDED_)
