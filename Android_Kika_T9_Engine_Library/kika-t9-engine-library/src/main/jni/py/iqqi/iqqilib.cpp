#if defined(_MSC_VER) && _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <codecvt>
#include <algorithm>
#include <locale>
#include <cstring>
#include "pinyinime.h"
#include "dictdef.h"
#include "zhuyinToPinyin.h"
#include "kt9ToPinyin.h"

#include "iqqilib_v1.h"

// codecvt bug in Visual Studio c++ 2015 and 2017
#if !_DLL && _MSC_VER >= 1900
std::locale::id std::codecvt<char16_t, char, _Mbstatet>::id;
#endif

static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter_wchar;

static const std::wstring validZhuyinToneInput = L"ˉˊˇˋ˙";
static const std::wstring validZhuyinInput = L"ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙㄧㄨㄩㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦˉˊˇˋ˙";

static bool isInited = false;
static bool isZhuyin = false;
static char cachedUserDBPath[256] = {'\0'};

static wchar_t cachedKeyCode[ime_pinyin::kMaxLemmaSize+1];
static size_t cachedLength;
static size_t cachedCount;
static char cachedBuffer[ime_pinyin::kMaxLemmaSize+1];

static std::vector<std::pair<std::string, int>> cachedKT9PinyinComposings;
static std::vector<Kt9Candidate> cachedKT9Candidates;

static wchar_t cachedKT9KeyCodeComposingAppend[ime_pinyin::kMaxLemmaSize+1];
static size_t cachedKT9LengthComposingAppend;
static std::string cachedKT9ComposingAppend;

static size_t splStart[ime_pinyin::kMaxLemmaSize+1];
static ime_pinyin::char16 (*predict_buf)[ime_pinyin::kMaxPredictSize + 1] = NULL;

void resetCache() {
    cachedKeyCode[0] = 0;
    cachedLength = 0;
    cachedCount = 0;
    cachedBuffer[0] = 0;
    cachedKT9PinyinComposings.clear();
    cachedKT9Candidates.clear();
    
    cachedKT9KeyCodeComposingAppend[0] = 0;
    cachedKT9LengthComposingAppend = 0;
    cachedKT9ComposingAppend.clear();
}

size_t char16ToWchar(wchar_t* buf, const char16_t* str) {
    std::u16string u16s(str);
    std::wstring ws = converter_wchar.from_bytes(converter.to_bytes(u16s));
    ws.copy(buf, ws.length());
    buf[ws.length()] = 0;
    return ws.length();
}

size_t wcharToChar16(char16_t* buf, const wchar_t* str) {
    std::u16string u16s = converter.from_bytes(converter_wchar.to_bytes(str));
    u16s.copy(buf, u16s.length());
    buf[u16s.length()] = 0;
    return u16s.length();
}

size_t wcharToChar(char* buf, const wchar_t* str) {
    std::string bytes = converter_wchar.to_bytes(str);
    bytes.copy(buf, bytes.length());
    buf[bytes.length()] = 0;
    return bytes.length();
}

void reset() {
    ime_pinyin::im_reset_search();
    resetCache();
}

std::u16string buildLemma(const char* pinyin, const char* word, float para) {
    std::stringstream lemma;
    lemma << pinyin << "," << word << "," << para;
    std::u16string u16lemma(converter.from_bytes(lemma.str().c_str()));
    return u16lemma;
}

int _IQQI_Initial(int _IME, char* PredicDBPath, char* UserDBPath, char* IME_ErrorList)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isInited)
        return _IQQI_INIT_STATE_ERROR;

    if (strlen(UserDBPath) >= sizeof(cachedUserDBPath))
        return _IQQI_PARA_ERROR;

    isZhuyin = (_IME == _IME_ID_ZHUYIN);
    ime_pinyin::im_set_spl_homo(isZhuyin);
    isInited = ime_pinyin::im_open_decoder(PredicDBPath, UserDBPath);

    if (isInited) {
        strncpy(cachedUserDBPath, UserDBPath, sizeof(cachedUserDBPath));
    } else {
        cachedUserDBPath[0] = 0;
    }

    return isInited? _IQQI_SUCCESS: _IQQI_FAIL;
}


int _IQQI_Free()
{
    if (!isInited)
        return _IQQI_INIT_STATE_ERROR;

    ime_pinyin::im_close_decoder();
    isInited = false;

    cachedUserDBPath[0] = 0;

    return _IQQI_SUCCESS;
}


bool _IQQI_WordExist( int _IME, const wchar_t* wsContent)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited)
        return _IQQI_INIT_STATE_ERROR;

    size_t length = wcslen(wsContent);

    if (length > ime_pinyin::kMaxSentenceLength)
        return false; // exceed max sentence size

    ime_pinyin::char16 buf[ime_pinyin::kMaxSentenceLength + 1];
    wcharToChar16((char16_t*)buf, wsContent);

    return ime_pinyin::im_get_lemma_id(buf, length) > 0;
}


int _IQQI_GetCandidateCount(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited)
        return _IQQI_INIT_STATE_ERROR;

    size_t length = wcslen(KeyCode);

    if (length > ime_pinyin::kMaxLemmaSize)
        return _IQQI_PARA_ERROR; // exceed max support size

    if (isZhuyin) {
        bool meetTone = false;
        for (size_t i = 0; i < length; i++) {
            if (validZhuyinToneInput.find(KeyCode[i]) != std::string::npos) {
                if (meetTone)
                    return _IQQI_PARA_ERROR;
                meetTone = true;
                continue;
            }
            meetTone = false;
            if (validZhuyinInput.find(KeyCode[i]) == std::string::npos)
                return _IQQI_PARA_ERROR;
        }
    } else {
        bool meetTone = false;
        for (size_t i = 0; i < length; i++) {
            if (wcsncmp(&KeyCode[i], L"'", 1) == 0) {
                if (meetTone)
                    return _IQQI_PARA_ERROR;
                meetTone = true;
                continue;
            }
            meetTone = false;
            if (iswalpha(KeyCode[i]) == 0)
                return _IQQI_PARA_ERROR; // input KeyCode error
        }
    }


    cachedLength = length;
    wcsncpy(cachedKeyCode, KeyCode, length);
    cachedKeyCode[length] = 0;

    if (length == 0) {
        ime_pinyin::im_reset_search();
        resetCache();
        return 0;
    }

    if (isZhuyin) {
        int hanziCount = zhuyinToPinyin(cachedBuffer, splStart, ime_pinyin::kMaxLemmaSize+1, KeyCode);
        if (hanziCount <= 0 || hanziCount > ime_pinyin::kMaxSentenceLength) { // this check ensures the input should not exceed engine limitations
            ime_pinyin::im_reset_search();
            resetCache();
            return _IQQI_PARA_ERROR; // exceed max support size
        }
    } else {
        wcharToChar(cachedBuffer, KeyCode);
    }

    cachedCount = ime_pinyin::im_search(cachedBuffer, strlen(cachedBuffer));

    // the engine could drop too long pinyin string
    size_t pyStrLen;
    const char* pyStr = ime_pinyin::im_get_sps_str(&pyStrLen);
    if (pyStrLen != strlen(cachedBuffer) || strcmp(pyStr, cachedBuffer) != 0) {
        ime_pinyin::im_reset_search();
        resetCache();
        return _IQQI_PARA_ERROR; // exceed max support size
    }

    return cachedCount;
}

int _IQQI_GetCandidates(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited)
        return _IQQI_INIT_STATE_ERROR;

    size_t length = wcslen(KeyCode);

    if (length != cachedLength || wcscmp(KeyCode, cachedKeyCode) != 0) {
        int count = _IQQI_GetCandidateCount(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
        if (count <= 0)
            return count;
    }

    size_t j = 0;
    for (size_t i = Begin; i < cachedCount && i < (size_t)Begin + (size_t)Number; i++, j++) {
        ime_pinyin::char16 candidateBuffer[ime_pinyin::kMaxLemmaSize];
        ime_pinyin::im_get_candidate(i, candidateBuffer, ime_pinyin::kMaxLemmaSize);
        char16ToWchar(Candidates[j], (const char16_t*) candidateBuffer);
    }

    return j;
}


int _IQQI_GetCandidates_Ex(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, int* Scores)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited)
        return _IQQI_INIT_STATE_ERROR;

    size_t length = wcslen(KeyCode);

    if (length != cachedLength || wcscmp(KeyCode, cachedKeyCode) != 0) {
        int count = _IQQI_GetCandidateCount(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
        if (count <= 0)
            return count;
    }

    size_t j = 0;
    for (size_t i = Begin; i < cachedCount && i < (size_t)Begin + (size_t)Number; i++, j++) {
        ime_pinyin::char16 candidateBuffer[ime_pinyin::kMaxLemmaSize];
        ime_pinyin::uint16 score;
        ime_pinyin::uint16 id;

        ime_pinyin::im_get_candidate_ex(i, candidateBuffer, ime_pinyin::kMaxLemmaSize, &score, &id);
        char16ToWchar(Candidates[j], (const char16_t*) candidateBuffer);
        Scores[j] = (int)score;
    }

    return j;
}

int _IQQI_GetCandidateCount_KT9(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type) {
    if (!isInited)
        return _IQQI_INIT_STATE_ERROR;

    if (_IME != _IME_ID_PINYIN) // pinyin only
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    const size_t length = wcslen(KeyCode);

    if (length == 0 || length > T9MaxInputSize)
        return _IQQI_PARA_ERROR;

    if (length == cachedLength && wcscmp(KeyCode, cachedKeyCode) == 0)
        return cachedKT9Candidates.size();
    
    if (length == cachedKT9LengthComposingAppend && wcscmp(KeyCode, cachedKT9KeyCodeComposingAppend) == 0)
        return cachedKT9Candidates.size();
    
    std::vector<std::pair<std::string, int>> pinyinComposings;

    if (kt9ToPinyin(KeyCode, pinyinComposings) < 0)
        return _IQQI_PARA_ERROR;

    if (cachedKT9PinyinComposings.size() > 0 && cachedKT9PinyinComposings == pinyinComposings)
        return cachedKT9Candidates.size();
        
    std::vector<Kt9Candidate> KT9Candidates;

    // reset engine if new search, trick
    if (length == 1)
        ime_pinyin::im_reset_search();
    
    bool hasComplete = false;
    std::vector<bool> candidateIdTable(0xFFFF, false);
    std::vector<bool> candidateIsCompleteTable(0xFFFF, false);
    std::vector<std::string> candidateComposingTable(0xFFFF);

    for(const std::pair<std::string, int>& composing: pinyinComposings) {
        if (composing.second > T9MaxChLimit)
            continue;

        const std::string& s = composing.first;

        int count = ime_pinyin::im_search(s.c_str(), s.size());
        if (count <= 0)
            continue;
        
        std::string splComposing = composing.first;
        const unsigned short *spl_start;
        const int len = ime_pinyin::im_get_spl_start_pos(spl_start);
        if (len > 0) {
            size_t r = 0;
            for (int index = 1; index <= len; index++, r++) {
                splComposing.insert(spl_start[index]+r, " ");
            }
        }
        
        for (int j = 0; j < count; j++) {
            ime_pinyin::char16 candidateBuffer[ime_pinyin::kMaxLemmaSize];
            wchar_t candidate[ime_pinyin::kMaxLemmaSize];
            ime_pinyin::uint16 score;
            ime_pinyin::uint16 id;

            ime_pinyin::im_get_candidate_ex(j, candidateBuffer, ime_pinyin::kMaxLemmaSize, &score, &id);
            char16ToWchar(candidate, (const char16_t*) candidateBuffer);

            if (id == 0)
                continue;

            const std::wstring wcandidate(candidate);
            const bool isComplete = (wcandidate.length() == composing.second);
            hasComplete |= isComplete;
            
            if (!isComplete) // remove this line to output all possible results
                continue;
            
            if (isComplete)
                candidateIsCompleteTable[id] = true;
            
            if (isComplete || candidateComposingTable[id].empty())
                candidateComposingTable[id] = splComposing;
            
            if (candidateIdTable[id]) {
                //std::cout << converter_wchar.to_bytes(candidate) << " " << id << " " << score << std::endl;
                continue;
            }
            
            KT9Candidates.push_back(Kt9Candidate(wcandidate, id, wcandidate.length(), score));
            candidateIdTable[id] = true;
        }
    }

    for (auto& candidate: KT9Candidates) {
        candidate.mIsComplete = candidateIsCompleteTable[candidate.mId];
        candidate.mComposing = candidateComposingTable[candidate.mId];
    }

    //if (hasComplete)
        std::sort(std::begin(KT9Candidates), std::end(KT9Candidates), Kt9CandidateCompareMix);
    //else
        //std::sort(std::begin(cachedKT9Candidates), std::end(cachedKT9Candidates), Kt9CandidateCompareNoComplete);
    
    if (KT9Candidates.size() > 0) {
        resetCache();

        cachedLength = length;
        wcsncpy(cachedKeyCode, KeyCode, length);
        cachedKeyCode[length] = 0;
        cachedKT9PinyinComposings = pinyinComposings;
        cachedKT9Candidates = KT9Candidates;
        
        return cachedKT9Candidates.size();
    }

    // find no complete candidates for current input, try to use old input and results
    if (cachedKT9Candidates.size() > 0
        && length > cachedLength && wcsncmp(KeyCode, cachedKeyCode, cachedLength) == 0) {
        std::string extendComposing;
        int status = kt9ToComposingDisplay(KeyCode+cachedLength, extendComposing);
        if (status < 0)
            return _IQQI_PARA_ERROR;

        cachedKT9ComposingAppend = extendComposing;
        cachedKT9LengthComposingAppend = length;
        wcsncpy(cachedKT9KeyCodeComposingAppend, KeyCode, length);
        cachedKT9KeyCodeComposingAppend[length] = 0;

        return cachedKT9Candidates.size();
    }

    // - if there is no old input results, drop input tails to find usable results (recompute all, expensive)
    if (length - 1 == 0)
        return 0;

    std::wstring _keycode(KeyCode, length - 1);
    return _IQQI_GetCandidateCount_KT9(_IME, (wchar_t*)_keycode.c_str(), FirstCandidateIsUserTyping, Type);
    // the cache should have some results
}


int _IQQI_GetCandidates_KT9(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, wchar_t** Composings)
{
    int count = _IQQI_GetCandidateCount_KT9(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
    if (count <= 0)
        return count;
    
    const size_t length = wcslen(KeyCode);
    
    std::string composingAppend;
    if (length == cachedKT9LengthComposingAppend && wcscmp(KeyCode, cachedKT9KeyCodeComposingAppend) == 0)
        composingAppend = " " + cachedKT9ComposingAppend;


    size_t j = 0;
    for (size_t i = Begin; i < cachedKT9Candidates.size() && i < (size_t)Begin + (size_t)Number; i++, j++) {
        wcscpy(Candidates[j], cachedKT9Candidates[i].mCandidate.c_str());
        if (Composings != NULL) {
            std::string composing = cachedKT9Candidates[i].mComposing + composingAppend;
            std::wstring Composing = converter_wchar.from_bytes(composing.c_str());
            wcscpy(Composings[j], Composing.c_str());
        }
    }

    return j;
}


int _IQQI_GetNextWordCandidates( int _IME, wchar_t* U16Word, int Begin, int Number, wchar_t **Candidates)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited)
        return _IQQI_INIT_STATE_ERROR;

    resetCache();

    size_t length = wcslen(U16Word);

    if (length > ime_pinyin::kMaxSentenceLength) {
        U16Word += length - ime_pinyin::kMaxSentenceLength;
    }

    char16_t buf[ime_pinyin::kMaxSentenceLength];
    wcharToChar16(buf, U16Word);
    size_t count = ime_pinyin::im_get_predicts((const ime_pinyin::char16 *)buf, predict_buf);

    size_t j = 0;
    for (size_t i = Begin; i < count && i < (size_t)Begin + (size_t)Number; i++, j++) {
        char16ToWchar(Candidates[j], (const char16_t*) predict_buf[i]);
    }

    ime_pinyin::im_reset_search();
    return j;
}

int _IQQI_LearnWord(int _IME, wchar_t* KeyCode, wchar_t* Content)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited || strlen(cachedUserDBPath) == 0)
        return _IQQI_INIT_STATE_ERROR;

    reset();

    const size_t keycodeLength = wcslen(KeyCode);
    const size_t contentLengh = wcslen(Content);

    if (isZhuyin) {
        const int hanziCount = zhuyinToPinyin(cachedBuffer, splStart, ime_pinyin::kMaxLemmaSize+1, KeyCode);
        if (hanziCount != contentLengh ||
            hanziCount <= 0 || hanziCount > ime_pinyin::kMaxSentenceLength) {
            reset();
            return _IQQI_PARA_ERROR;
        }
    } else {
        if (keycodeLength > ime_pinyin::kMaxLemmaSize ||
            contentLengh > ime_pinyin::kMaxSentenceLength) {
            reset();
            return _IQQI_PARA_ERROR;
        }

        for (size_t i = 0; i < keycodeLength; i++) {
            if (!iswalpha(KeyCode[i]) && wcsncmp(&KeyCode[i], L"'", 1) != 0)
                return _IQQI_PARA_ERROR; // wrong pinyin keycode
        }

        wcharToChar(cachedBuffer, KeyCode);
    }

    size_t count = ime_pinyin::im_search(cachedBuffer, strlen(cachedBuffer));

    ime_pinyin::char16 char16Buf[ime_pinyin::kMaxLemmaSize+1];
    wchar_t wcharBuf[ime_pinyin::kMaxLemmaSize+1];

    // first check the word is already in results or not
    for (size_t i = 0; i < count; i++) {
        ime_pinyin::im_get_candidate(i, char16Buf, ime_pinyin::kMaxLemmaSize);
        char16ToWchar(wcharBuf, (const char16_t*) char16Buf);
        if (wcscmp(wcharBuf, Content) == 0) {
            reset();
            return _IQQI_SUCCESS; // already exists
        }
    }

    // try candidate selection to learn
    for (size_t i = 0; i < contentLengh; i++) {
        size_t id;
        for (id = 0; id < count; id++) {
            ime_pinyin::im_get_candidate(id, char16Buf, ime_pinyin::kMaxLemmaSize);
            char16ToWchar(wcharBuf, (const char16_t*) char16Buf);
            if (wcslen(wcharBuf) == 1 && wcharBuf[0] == Content[i]) {
                ime_pinyin::im_choose(id);
                break;
            }
        }

        if (id == count) {
            reset();
            return _IQQI_PARA_ERROR; // unmatched word in dictionary results
        }
    }

    ime_pinyin::im_flush_cache();

    ime_pinyin::im_reset_search();
    // checking
    count = ime_pinyin::im_search(cachedBuffer, strlen(cachedBuffer));

    for (size_t i = 0; i < count; i++) {
        ime_pinyin::im_get_candidate(i, char16Buf, ime_pinyin::kMaxLemmaSize);
        char16ToWchar(wcharBuf, (const char16_t*) char16Buf);
        if (wcscmp(wcharBuf, Content) == 0) {
            reset();
            return _IQQI_SUCCESS; // success
        }
    }

    reset();
    return _IQQI_FAIL;
}

int _IQQI_IsLearnWordExist(int _IME, const wchar_t* KeyCode, const wchar_t* Content)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited || strlen(cachedUserDBPath) == 0)
        return _IQQI_INIT_STATE_ERROR;

    reset();

    const size_t keycodeLength = wcslen(KeyCode);
    const size_t contentLengh = wcslen(Content);

    if (isZhuyin) {
        const int hanziCount = zhuyinToPinyin(cachedBuffer, splStart, ime_pinyin::kMaxLemmaSize+1, KeyCode);
        if (hanziCount != contentLengh ||
            hanziCount <= 0 || hanziCount > ime_pinyin::kMaxSentenceLength) {
            reset();
            return _IQQI_PARA_ERROR;
        }
    } else {
        if (keycodeLength > ime_pinyin::kMaxLemmaSize ||
            contentLengh > ime_pinyin::kMaxSentenceLength) {
            reset();
            return _IQQI_PARA_ERROR;
        }

        for (size_t i = 0; i < keycodeLength; i++) {
            if (!iswalpha(KeyCode[i]) && wcsncmp(&KeyCode[i], L"'", 1) != 0)
                return _IQQI_PARA_ERROR; // wrong pinyin keycode
        }

        wcharToChar(cachedBuffer, KeyCode);
    }

    ime_pinyin::char16 buf[ime_pinyin::kMaxSentenceLength + 1];
    size_t length = wcharToChar16((char16_t*)buf, Content);

    size_t id = ime_pinyin::im_get_user_lemma_id(buf, length, cachedBuffer);
    bool success = ime_pinyin::im_is_user_lemma_id(id);

    reset();

    return success? _IQQI_SUCCESS: _IQQI_FAIL;
}

int _IQQI_DeleLearnedWord(int _IME, wchar_t* KeyCode, wchar_t* Content)
{
    if (_IME != _IME_ID_PINYIN && _IME != _IME_ID_ZHUYIN)
        return _IQQI_PARA_ERROR;

    if (isZhuyin && _IME == _IME_ID_PINYIN)
        return _IQQI_PARA_ERROR;

    if (!isInited || strlen(cachedUserDBPath) == 0)
        return _IQQI_INIT_STATE_ERROR;

    reset();

    const size_t keycodeLength = wcslen(KeyCode);
    const size_t contentLengh = wcslen(Content);

    if (isZhuyin) {
        const int hanziCount = zhuyinToPinyin(cachedBuffer, splStart, ime_pinyin::kMaxLemmaSize+1, KeyCode);
        if (hanziCount != contentLengh ||
            hanziCount <= 0 || hanziCount > ime_pinyin::kMaxSentenceLength) {
            reset();
            return _IQQI_PARA_ERROR;
        }
    } else {
        if (keycodeLength > ime_pinyin::kMaxLemmaSize ||
            contentLengh > ime_pinyin::kMaxSentenceLength) {
            reset();
            return _IQQI_PARA_ERROR;
        }

        for (size_t i = 0; i < keycodeLength; i++) {
            if (!iswalpha(KeyCode[i]) && wcsncmp(&KeyCode[i], L"'", 1) != 0)
                return _IQQI_PARA_ERROR; // wrong pinyin keycode
        }

        wcharToChar(cachedBuffer, KeyCode);
    }


    ime_pinyin::char16 buf[ime_pinyin::kMaxSentenceLength + 1];
    size_t length = wcharToChar16((char16_t*)buf, Content);

    size_t id = ime_pinyin::im_get_user_lemma_id(buf, length, cachedBuffer);
    if (!ime_pinyin::im_is_user_lemma_id(id))
        return _IQQI_FAIL;

    bool status = ime_pinyin::remove_lemmas(cachedUserDBPath, id);

    return status? _IQQI_SUCCESS: _IQQI_FAIL;
}

extern "C" {

const char * IQQI_Version_Py()
{
    return "Kika Pinyin Chinese Engine V2020.0331.1";
}

int IQQI_Initial_Py(int _IME, char* PredicDBPath, char* UserDBPath, char* IME_ErrorList)
{
    return _IQQI_Initial(_IME, PredicDBPath, UserDBPath, IME_ErrorList);
}

int IQQI_Free_Py()
{
    return _IQQI_Free();
}

bool IQQI_WordExist_Py( int _IME, const wchar_t* wsContent)
{
    return _IQQI_WordExist(_IME, wsContent);
}

int IQQI_GetCandidateCount_Py(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type)
{
    return _IQQI_GetCandidateCount(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
}

int IQQI_GetCandidates_Py(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates)
{
    return _IQQI_GetCandidates(_IME, KeyCode, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates);
}

int IQQI_GetCandidates_Py_Ex(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, int* Scores)
{
    return _IQQI_GetCandidates_Ex(_IME, KeyCode, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, Scores);
}

int IQQI_GetCandidateCount_KT9(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type)
{
    return _IQQI_GetCandidateCount_KT9(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
}

int IQQI_GetCandidates_KT9(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, wchar_t** Composings)
{
    return _IQQI_GetCandidates_KT9(_IME, KeyCode, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, Composings);
}

int IQQI_GetNextWordCandidates_Py( int _IME, wchar_t* U16Word, int Begin, int Number, wchar_t **Candidates)
{
    return _IQQI_GetNextWordCandidates(_IME, U16Word, Begin, Number, Candidates);
}

int IQQI_LearnWord_Py(int _IME, wchar_t* KeyCode, wchar_t* Content)
{
    return _IQQI_LearnWord(_IME, KeyCode, Content);
}

int IQQI_IsLearnWordExist_Py(int _IME, const wchar_t* KeyCode, const wchar_t* Content)
{
    return _IQQI_IsLearnWordExist(_IME, KeyCode, Content);
}

int IQQI_DeleLearnedWord_Py(int _IME, wchar_t* KeyCode, wchar_t* Content)
{
    return _IQQI_DeleLearnedWord(_IME, KeyCode, Content);
}

}
