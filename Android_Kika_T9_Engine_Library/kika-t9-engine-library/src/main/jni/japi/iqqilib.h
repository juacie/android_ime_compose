#ifndef IQQILIB_H
#define IQQILIB_H

extern "C"
{
    int IQQI_Initial(int _IME, char* PredicDBPath, char* UserDBPath, char* PhrsDBPath, char* IME_ErrorList);
    int IQQI_Free();

    //設定路徑
    int IQQI_SetUserDBPath(char* Path);
    int IQQI_SetPredicDBPath(char* Path);

    //取得candidate的總數
    int IQQI_GetCandidateCount(
                                       int       _IME,
                                       wchar_t*  KeyCode,
                                       bool      FirstCandidateIsUserTyping,
                                       int       Type
                                       );

    int IQQI_GetCandidates(
                                       int       _IME,
                                       wchar_t*  KeyCode,
                                       bool      FirstCandidateIsUserTyping,
                                       int       Type,
                                       int       Begin,
                                       int       Number,
                                       wchar_t** Candidates,
                                       wchar_t** Composings
                                       );

    //取得關聯候選字
    int IQQI_GetIdiomTradCandidates(
                                            wchar_t*  U16Word,
                                            int       Begin,
                                            int       Number,
                                            wchar_t** Candidates
                                            );//繁體

    int IQQI_GetIdiomSimpCandidates(
                                            wchar_t*  U16Word,
                                            int       Begin,
                                            int       Number,
                                            wchar_t** Candidates
                                            );//簡體

    //取得輸入字根之分組(限中文 拼音/注音)
    int IQQI_GetGrouping(int _IME, wchar_t* KeyCode, int Begin, int Number, wchar_t** Candidates);

    //取得已被處理的拼音
    wchar_t* IQQI_GetInProcessedCode(int Location);

    //取得尚未處理的拼音
    wchar_t* IQQI_GetWaitProcessCode(int Location);

    //字詞學習
    int IQQI_LearnWord(
                                  int       _IME,
                                  wchar_t*  PinyinCode,
                                  wchar_t*  Content
                                  );

    //關聯詞學習
    int IQQI_LearnIdiomTrad( wchar_t*  U16Word );

    int IQQI_LearnIdiomSimp( wchar_t*  U16Word );

    //更新DB版本資訊
    int IQQI_UpdateDBVersionInfo(int _IME);

    //取得輸入的文字顯示
    wchar_t* IQQI_GetComposingText(
                                           int        _IME,
                                           wchar_t*   KeyCode,
                                           int        Type,
                                           int        aIndicator
                                           );

    //ComposingText延伸處理功能
    wchar_t* IQQI_ComposingTextExtend( int _IME, wchar_t* ComposingText, int Type);

    //補碼功能
    int IQQI_StartInsertCodeMode(
                                         int               _IME,
                                         wchar_t*          AllInputCode,
                                         int               Number,
                                         wchar_t**         Candidates,
                                         wchar_t*          ComposingText
                                         );

    int IQQI_InsertCode(
                                wchar_t*    InsertKeyCode,
                                wchar_t**   Candidates,
                                wchar_t*    ComposingText
                                );

    int IQQI_BackspaceInsertCode(
                                         wchar_t**    Candidates,
                                         wchar_t*     ComposingText
                                         );

    wchar_t* IQQI_DoneInsertCodeMode();
    wchar_t* IQQI_CancelInsertCodeMode();
    bool IQQI_IsInsertCodeMode();

    //預測字串,自動組句子
    wchar_t* IQQI_GetPredictedSentence(wchar_t *KeyCode);

    //rename
    wchar_t* IQQI_GetSelectedCandidateCode(int Location);

    wchar_t* IQQI_GetRemainsCode(int Location);

    int IQQI_GetNextWordCandidates(
                                    int      Language,
                                    wchar_t  *U16Word,
                                    int      Begin,
                                    int      Number,
                                    wchar_t  **Candidates
                                    );

    int IQQI_LearnNextWord(
                            int Language,
                            wchar_t* U16Word
                            );
    //
    wchar_t* IQQI_MultiTap_Input(int _IME, int KeyCode, int PrevKeyCode, int Count);

    //
    void IQQI_SetOption(unsigned int Item, int value);

    //刪除已學習的關聯詞
    int IQQI_DeleLearnedNextWord(int Language, wchar_t* Content );

    //刪除使用者學習的詞
    int IQQI_DeleLearnedWord(int _IME,wchar_t* PinyinCode,wchar_t* Content);

    //切換到顧客詞庫
    int IQQI_SwitchToCustomDB( int _IME, char* ItemName);

    //版本資訊
    char* IQQI_Version();
}

#endif // IQQILIB_H

