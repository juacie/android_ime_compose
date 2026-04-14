
//-----------------------------------------------------------------------------
//
// Return Value :OUT :版號 :ex, "Kika Pinyin Chinese Engine V2019.1208.1"
// Parameters   :
// Remarks      :取得版號
//
//-----------------------------------------------------------------------------

#ifdef _WIN32
    #define IQQILIBQTSHARED_EXPORT __declspec(dllexport)
#else
    #define IQQILIBQTSHARED_EXPORT __attribute__((visibility ("default")))
#endif


#ifdef __cplusplus
extern "C" {
#endif

const int _IME_ID_PINYIN = 28;//0;
const int _IME_ID_ZHUYIN = 71;//1;

// API return status
const int _IQQI_SUCCESS = 0;
const int _IQQI_FAIL = -1;
const int _IQQI_PARA_ERROR = -2;
const int _IQQI_INIT_STATE_ERROR = -3;
    
IQQILIBQTSHARED_EXPORT const char * IQQI_Version_Py();

//-----------------------------------------------------------------------------
//
// Return Value :OUT : 啟始化 成功/錯誤碼
// Parameters   :IN  : _IME          : 1.輸入法 ID
//                                   : 2.enum ID
//              :IN  : PredicDBPath  : 1.主詞典完整路徑檔案名稱
//                                     2.名稱使用者可自定 (ex. /sdcard/libEnglish_US.so)(也就是.dict)
//              :IN  : UserDBPath    : 1.自學詞完整路徑檔案名稱
//                                     2.名稱使用者可自定 (ex. /sdcard/libEnglish_US_User.so)(也就是.dict)
//                                     3.傳入路徑必須為可讀寫
//              :OUT : IME_ErrorList : 回傳錯誤描述Buffer
// Remarks      :啟始化 引擎Engine
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_Initial_Py(int _IME, char* PredicDBPath, char* UserDBPath, char* IME_ErrorList);

//-----------------------------------------------------------------------------
//
// Return Value :OUT : 釋放 正確/錯誤碼
// Parameters   :
// Remarks      :有無Engine資源要釋放
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_Free_Py();

//-----------------------------------------------------------------------------
//
// Return Value :OUT : true  存在, false 不存在
// Parameters   :IN  : _IME : 輸入法 ID
// Remarks      :判斷傳入的詞是否存在主詞典
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT bool IQQI_WordExist_Py( int _IME, const wchar_t* wsContent);

//-----------------------------------------------------------------------------
//
// Return Value :OUT : 取得之計數
// Parameters   :IN  : _IME                       : 輸入法 ID
//              :IN  : KeyCode                    : 傳入 InputKey :ex. ㄓㄨㄥ
//              :IN  : FirstCandidateIsUserTyping : 保留 false
//              :IN  : Type                       : 保留 : 0
// Remarks      :取得符合[ㄓㄨㄥ]的候選字數
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_GetCandidateCount_Py(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type);

//-----------------------------------------------------------------------------
//
// **Candidates 資料內容:
//  {[L"中"], [L"宗"], [L"鍾"], [L"中文"], [L"宗教"], [L"鍾馗"]}
// 高頻優先列
//
// Return Value :OUT : 取得之計數
// Parameters   :IN  : _IME       : 輸入法 ID
//              :IN  : KeyCode    : 傳入 InputKey :ex. ㄓㄨ
//              :IN  : Begin      : 從第Index開始
//              :IN  : Number     : 取出Number個
//              :OUT : Candidates : 取回[ㄓㄨ]之候選字陣列
// Remarks      :取回指定輸入碼之候選字陣列
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_GetCandidates_Py(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates);


//-----------------------------------------------------------------------------
//
// **Candidates 資料內容:
//  {[L"中"], [L"宗"], [L"鍾"], [L"中文"], [L"宗教"], [L"鍾馗"]}
// 高頻優先列
//
// Return Value :OUT : 取得之計數
// Parameters   :IN  : _IME       : 輸入法 ID
//              :IN  : KeyCode    : 傳入 InputKey :ex. ㄓㄨ
//              :IN  : Begin      : 從第Index開始
//              :IN  : Number     : 取出Number個
//              :OUT : Candidates : 取回[ㄓㄨ]之候選字陣列
//              :OUT : Scores     : 取回之候選字分數 (越低越好)
// Remarks      :取回指定輸入碼之候選字陣列
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_GetCandidates_Py_Ex(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, int* Scores);


//-----------------------------------------------------------------------------
//
// Return Value :OUT : 取得之計數
// Parameters   :IN  : _IME                       : 輸入法 ID
//              :IN  : KeyCode                    : 傳入 InputKey :ex. 746
//              :IN  : FirstCandidateIsUserTyping : 保留 false
//              :IN  : Type                       : 保留 : 0
// Remarks      :取得符合[746]的候選字數
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_GetCandidateCount_KT9(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type);


//-----------------------------------------------------------------------------
//
// **Candidates 資料內容:
//  {[L"中"], [L"宗"], [L"鍾"], [L"中文"], [L"宗教"], [L"鍾馗"]}
// 高頻優先列
//
// Return Value :OUT : 取得之計數
// Parameters   :IN  : _IME       : 輸入法 ID
//              :IN  : KeyCode    : 傳入 InputKey :ex. ㄓㄨ
//              :IN  : Begin      : 從第Index開始
//              :IN  : Number     : 取出Number個
//              :OUT : Candidates : 取回[ㄓㄨ]之候選字陣列
//              :OUT : Scores     : 取回之候選字分數 (越低越好)
// Remarks      :取回指定輸入碼之候選字陣列
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_GetCandidates_KT9(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, wchar_t** Composings);

//-----------------------------------------------------------------------------
//
// Return Value :OUT : 成功/錯誤碼
// Parameters   :IN  : _IME       : 輸入法 ID
//              :IN  : KeyCode    : 傳入 InputKey :ex. ㄓㄨㄥ
//              :IN  : Content    : 傳入 InputKey :ex. 郭
// Remarks      :1.學習新詞 (使用者輸入ㄓㄨㄥ, 會有[郭]候選字也在內)
//                2.不允許重複學習
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_LearnWord_Py(int _IME, wchar_t* KeyCode, wchar_t* Content);

//-----------------------------------------------------------------------------
//
// Return Value :OUT : 0: 不存在, 1: 存在
// Parameters   :IN  :_IME        : 輸入法 ID
//              :IN  : KeyCode    : 傳入 InputKey :ex. ㄓㄨㄥ
//              :IN  : Content    : 傳入 InputKey :ex. 郭
// Remarks      :學習新詞 (使用者輸入[ㄓㄨㄥ, 郭] 判斷自學詞是可否存在)
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_IsLearnWordExist_Py(int _IME, const wchar_t* KeyCode, const wchar_t* Content);

//-----------------------------------------------------------------------------
//
// Return Value :OUT :成功/錯誤碼
// Parameters   :IN  :_IME        : 輸入法 ID
//              :IN  :KeyCode     : 傳入 InputKey :ex. ㄓㄨㄥ
//              :IN  : Content    : 傳入 InputKey :ex. 郭
// Remarks      :刪除[ㄓㄨㄥ, 郭]的自學詞
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_DeleLearnedWord_Py( int _IME, wchar_t* KeyCode, wchar_t* Content);

//-----------------------------------------------------------------------------
//
// Return Value :OUT : 取得之計數
// Parameters   :IN  : U16Word    : 傳入中文字/詞 :ex. 中/中华
//              :IN  : Begin      : 從第Index開始
//              :IN  : Number     : 取出Number個
//              :OUT : Candidates : 取回[中]之聯想詞為[华, 国, 间, 文]
//              :                   取回[中华]之聯想詞[民国, 人民...]
// Remarks      :取回指定輸入字/詞之二元聯想詞
//
//-----------------------------------------------------------------------------
IQQILIBQTSHARED_EXPORT int IQQI_GetNextWordCandidates_Py( int _IME, wchar_t* U16Word, int Begin, int Number, wchar_t **Candidates);

#ifdef __cplusplus
}
#endif
