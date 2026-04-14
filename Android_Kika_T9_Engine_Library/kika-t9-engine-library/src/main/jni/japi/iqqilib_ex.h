#ifndef IQQILIB_EX_H
#define IQQILIB_EX_H

#include "kikaime.h"

extern "C"
{
    //切換到顧客詞庫
    int IQQI_InitialDictionary(int _IME, const char* wsDictName);

    int IQQI_IsVocabularyExist(int _IME, const wchar_t* wsVoca);

    int IQQI_AppendVocabulary(int _IME, const wchar_t* wsVoca);

    int IQQI_DeleteVocabulary(int _IME, const wchar_t* wsVoca);

    int IQQI_IsLearnWordExist(int _IME, const wchar_t* KeyCode, const wchar_t* Content);

    //字詞學習
    int IQQI_LearnWordEx(int _IME, wchar_t*  PinyinCode, wchar_t*  Content, bool ForImport, bool IsMultiTap);

    //字詞學習
    int IQQI_LearnWordUpdate(int _IME, wchar_t*  PinyinCode, wchar_t*  Content, bool ForImport, bool IsMultiTap);

    //字詞學習
    int IQQI_LearnWordSaveAs(int _IME, const char* UserDBName, wchar_t*  PinyinCode, wchar_t*  Content, bool ForImport, bool IsMultiTap);

    //字詞學習
    int IQQI_GetLearnedWordFromFile(int _IME, const char* UserDBName, int Number, wchar_t** Candidates);

    //清除使用者學習的詞(from MultiTap)
    int IQQI_ResetLearnedWordFromFile(int _IME, const char* UserDBName);

    //刪除使用者學習的詞
    int IQQI_DeleLearnedWordFrom(int _IME, const char* UserDBName, wchar_t* PinyinCode, wchar_t* Content);
    
    //檢查主詞是否存在
    int IQQI_WordExist( int _IME, const wchar_t* wsContent);

    //取得T9數字轉字母按鍵的功能
    wchar_t* IQQI_NMapKeyCode(int _IME, const wchar_t* Content);

    //取得字母轉T9數字按鍵的功能
    wchar_t* IQQI_KeyCodeNMap(int _IME, const wchar_t* Content);
    
    //取得字母之大寫字母按鍵的功能
    wchar_t* IQQI_ShiftKeyCode(int _IME, const wchar_t* Content);

    //取得虛擬鍵盤的Layout
    key* IQQI_GetVirtualKeybdLayout(int _IME, bool isQwerty, int &Total);
}

#endif // IQQILIB_H
