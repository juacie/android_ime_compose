#include "stdafx.h"
#include "kika_qwt9_inputmethod_Resource_qwt9ini.h"
#include "iqqilib.h"
#include "iqqilib_ex.h"
#include "DebugLog.h"
#include "DataCandidate.h"
#include "maptable.h"
#include "trans_key_korean.h"
#include "key_korean_data_h.h"
#include <map>

//#define __ANDROID_MSG
// #define __FOR_ANDROID

#define KIKA_OK                        0
#define KIKA_NOT_OK                    (-1)
#define CANDIDATE_INPUT_LIMIT          45 // 54
#define CANDIDATE_MAX_ROW_ZH           120
#define MAX_CANDIDATE_LIST             20

#if defined(__IQQI_PY)
#include "iqqilib_v1.h"
#define MAX_MEM_WORD_LENGTH           256  //Must same as MAX_CHI_WORD_LENGTH
#else
#define MAX_MEM_WORD_LENGTH           1
#endif // defined
map <wstring, wstring>
mKeyin;

#define LOG_KIKA_TAG "KIKA_QWT9"

#define KIKA_LOGE(fmt, ...)  __android_log_print(ANDROID_LOG_ERROR, LOG_KIKA_TAG , "%s(L%d): " fmt, __FUNCTION__, __LINE__,  ## __VA_ARGS__)

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1Version
(JNIEnv *env, jclass)
{
    return env->NewStringUTF(IQQI_Version());
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1Free
(JNIEnv *env, jclass)
{
    try
    {
        IQQI_Free();
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
jint getImeID(JNIEnv *env, jobject jime)
{
    try
    {
        jclass
        objectClass = env->GetObjectClass(jime);

        jmethodID
        getValueMethod = env->GetMethodID(objectClass, "getValue", "()I");

        jint
        imeID = env->CallIntMethod(jime, getValueMethod);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, getImeID IME =%d\r\n", imeID);
#endif

        return imeID;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1Initial
(JNIEnv *env, jclass, jobject jime, jstring jPredicDBPath, jstring jUserDBPath, jstring jPhrsDBPath, jobject)
{
    try
    {
        char
        IME_ErrorList[32] = {0};

        jint
        _IME = getImeID(env, jime);

        const char *
        sNullPath = "";

        const char *
        sPredicDBPath = nullptr;

        if (jPredicDBPath == nullptr)
        {
            sPredicDBPath = sNullPath;

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
            LOGD("Kika-ENGINE, PredicDBPath ==NULL\r\n");
#endif
        }
        else
        {
            sPredicDBPath = env->GetStringUTFChars(jPredicDBPath, JNI_FALSE);
        }

        const char *
        sUserDBPath = nullptr;

        if (jUserDBPath == nullptr)
        {
            sUserDBPath = sNullPath;

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
            LOGD("Kika-ENGINE, UserDBPath ==NULL\r\n");
#endif
        }
        else
        {
            sUserDBPath = env->GetStringUTFChars(jUserDBPath, JNI_FALSE);
        }

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, PredicDBPath =%s\r\n", sPredicDBPath);
        LOGD("Kika-ENGINE, UserDBPath   =%s\r\n", sUserDBPath);
#endif

        int
        retVal = IQQI_Initial(_IME, (char *) sPredicDBPath, (char *) sUserDBPath, NULL,
                              IME_ErrorList);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, Initial RetVal =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

#if defined(__IQQI_PY)
//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1Initial_1Py
(JNIEnv *env, jclass, jobject jime, jstring jPyPredicDBPath, jstring jPyUserDBPath, jstring jPredicDBPath, jstring jUserDBPath, jstring jPhrsDBPath, jobject)
{
    try
    {
        char
        IME_ErrorList[32] = {0};

        jint
        _IME = getImeID(env, jime);

        const char *
        sNullPath = "";

        const char *
        sPyPredicDBPath = NULL;

        if (jPyPredicDBPath == NULL)
        {
            sPyPredicDBPath = sNullPath;

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
            LOGD("Kika-ENGINE, PyPredicDBPath ==NULL\r\n");
#endif
        }
        else
        {
            sPyPredicDBPath = env->GetStringUTFChars(jPyPredicDBPath, JNI_FALSE);
        }

        const char *
        sPyUserDBPath = NULL;

        if (jPyUserDBPath == NULL)
        {
            sPyUserDBPath = sNullPath;

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
            LOGD("Kika-ENGINE, PyUserDBPath ==NULL\r\n");
#endif
        }
        else
        {
            sPyUserDBPath = env->GetStringUTFChars(jPyUserDBPath, JNI_FALSE);
        }


#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, PyPredicDBPath =%s\r\n", sPyPredicDBPath);
        LOGD("Kika-ENGINE, PyUserDBPath   =%s\r\n", sPyUserDBPath);
        LOGD("Kika-ENGINE, PredicDBPath   =%s\r\n", sPredicDBPath);
        LOGD("Kika-ENGINE, UserDBPath     =%s\r\n", sUserDBPath);
#endif

        int retVal = IQQI_Initial_Py(_IME, (char *) sPyPredicDBPath, (char *) sPyUserDBPath,
                                     IME_ErrorList);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, Initial_Py RetVal =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}
#endif // #if defined(__IQQI_PY)

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1SetOption
(JNIEnv *env, jclass, jobject jopt, jobject jval)
{
    try
    {
        jclass
        objectClass = env->GetObjectClass(jopt);

        jmethodID
        getValueMethod = env->GetMethodID(objectClass, "getValue", "()I");

        jint
        option = env->CallIntMethod(jopt, getValueMethod);

        objectClass = env->GetObjectClass(jval);

        getValueMethod = env->GetMethodID(objectClass, "getValue", "()I");

        jint
        val = env->CallIntMethod(jval, getValueMethod);

        IQQI_SetOption(option, val);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
jstring toJString (JNIEnv *env, const wchar_t *str)
{
    size_t
    len = wcslen(str);

    jchar *
    str2 = (jchar *)malloc((len + 1) * sizeof(jchar));

    for ( int i = 0; i < len; i++ )
        str2[i] = str[i];

    str2[len] = 0;

    jstring
    js = env->NewString(str2, (jsize)len);

    free(str2);

    return js;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool IQQI_SetCandidateInfo(JNIEnv *env, int cIME, const IQWCHAR *pWstr, const IQWCHAR *composing
                           , jobjectArray Container, int ContainerCount)
{
    jclass objectClass = env->FindClass("kika/qwt9/inputmethod/Resource/CandidatesInfo");

    if ( env->ExceptionCheck() )
    {
#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, CandidatesInfo class not FOUND!!!");
#endif
        return false;
    }

    jobject _obj = env->AllocObject(objectClass);

    jfieldID Candidate = env->GetFieldID(objectClass, "mCandidate", "Ljava/lang/String;");
    jfieldID isUserData = env->GetFieldID(objectClass, "misUserData", "Z");

    jstring jstr = toJString(env, pWstr);
    env->SetObjectField(_obj, Candidate, jstr);
    env->SetBooleanField(_obj, isUserData, false);

#if defined(__IQQI_PY)
    if (composing != NULL && composing[0] != 0)
    {
        jfieldID Composing = env->GetFieldID(objectClass, "mComposing", "Ljava/lang/String;");
        jstring jcomposing = toJString(env, composing);
        env->SetObjectField(_obj, Composing, jcomposing);
    }
#endif
    env->SetObjectArrayElement(Container, ContainerCount, _obj);

    return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::wstring IQQI_JstrToWstr(JNIEnv *env, jstring string)
{
    std::wstring
    value;

    const jchar
    *raw = env->GetStringChars(string, 0);

    jsize
    len = env->GetStringLength(string);

    const
    jchar *temp = raw;

    while ( len > 0 )
    {
        value += (wchar_t) * (temp++);
        len--;
    }
    env->ReleaseStringChars(string, raw);

    return value;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1GetCandidateCount
(JNIEnv *env, jclass, jobject jime, jstring KeyCode, jboolean FirstCandidateIsUserTyping, jint Type)
{
    try
    {
        jint _IME = getImeID(env, jime);
        wstring wsKeyCode = IQQI_JstrToWstr(env, KeyCode);

#if defined(__IQQI_PY)
        if (_IME == IQQI_IME_Chinese_CN)
        {
            return IQQI_GetCandidateCount(_IME, (wchar_t *) wsKeyCode.c_str(),
                                          FirstCandidateIsUserTyping, Type);
        }
#endif

#if defined(__SIMPLE)
        return IQQI_GetCandidateCount(_IME, (wchar_t *)wsKeyCode.c_str(), FirstCandidateIsUserTyping, Type);
#else

        int
        Number = CANDIDATE_MAX_ROW_ZH; // 120

        CandidateCH
        CandVoca;
        CandVoca.alloc(Number, CANDIDATE_INPUT_LIMIT);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetCandidates IME =%d, Keyin =%ls (vLen =%d), Begin =%d, Number =%d\r\n", _IME, (wchar_t *)wsKeyCode.c_str(), (int)wsKeyCode.size(), 0, Number);
#endif

        int
        Total = IQQI_GetCandidates(_IME, (wchar_t *) wsKeyCode.c_str(),
                                   FirstCandidateIsUserTyping, Type, 0, Number,
                                   CandVoca.pointer(), NULL);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetCandidatesCount =%d\r\n", Total);
#endif

        return Total;

#endif
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1GetCandidates
(JNIEnv *env, jclass, jobject jime, jstring KeyCode, jboolean FirstCandidateIsUserTyping, jint Type, jint Begin, jint Number, jobjectArray Candidates)
{
    try
    {
        jint _IME = getImeID(env, jime);

        wstring
        wsKeyCode = IQQI_JstrToWstr(env, KeyCode);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetCandidates IME =%d, Keyin =%ls (vLen =%d), Begin =%d, Number =%d\r\n", _IME, (wchar_t *)wsKeyCode.c_str(), (int)wsKeyCode.size(), Begin, Number);
#endif

        int Total;

#if defined(__IQQI_PY)
        if (_IME == IQQI_IME_Chinese_CN)
        {
            CandidateCH CandVoca;
            CandVoca.alloc(Number, CANDIDATE_MAX_ROW_ZH);
            CandidateCH Composing;
            Composing.alloc(Number, CANDIDATE_MAX_ROW_ZH);

            Total = IQQI_GetCandidates(_IME, (wchar_t *) wsKeyCode.c_str(),
                                       FirstCandidateIsUserTyping, Type, Begin, Number,
                                       CandVoca.pointer(), Composing.pointer());

            for (int k = 0; k < Total; k++)
            {
                if (!IQQI_SetCandidateInfo(env, _IME, CandVoca.record(k), Composing.record(k),
                                           Candidates, k))
                {
                    return 0;
                }
            }
        }
        else
#endif
        {
            CandidateCH CandVoca;
            CandVoca.alloc(Number, CANDIDATE_INPUT_LIMIT);
            Total = IQQI_GetCandidates(_IME, (wchar_t *) wsKeyCode.c_str(),
                                       FirstCandidateIsUserTyping, Type, Begin, Number,
                                       CandVoca.pointer(), NULL);

            if (Total > 0)
            {
                for (int k = 0; k < Total; k++)
                {
                    if (IQQI_SetCandidateInfo(env, _IME, CandVoca.record(k), NULL, Candidates,
                                              k) == false)
                    {
                        return 0;
                    }
                }
            }
        }


#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetCandidates =%d\r\n", Total);
#endif

        return Total;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1GetGrouping
(JNIEnv *env, jclass, jobject jime, jstring KeyCode, int Begin, jint Number, jobjectArray Candidates)
{
    try
    {
        CandidateCH
        CandVoca;
        CandVoca.alloc(Number, CANDIDATE_INPUT_LIMIT);

        jint
        _IME = getImeID(env, jime);

        wstring
        wsKeyCode = IQQI_JstrToWstr(env, KeyCode);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetGrouping IME =%d, Keyin =%ls (vLen =%d), Begin =%d, Number =%d\r\n", _IME, (wchar_t *)wsKeyCode.c_str(), (int)wsKeyCode.size(), Begin, Number);
#endif

        int
        Total = IQQI_GetGrouping(_IME, (wchar_t *) wsKeyCode.c_str(), Begin, Number,
                                 CandVoca.pointer());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetGrouping Total =%d\r\n", Total);
#endif

        if (Total > 0)
        {
            for (int k = 0; k < Total; k++)
            {
                if (IQQI_SetCandidateInfo(env, _IME, CandVoca.record(k), NULL, Candidates, k) ==
                        false)
                {
                    return 0;
                }
            }
        }
        return Total;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static
void replaceAll(std::wstring &str, const std::wstring &from, const std::wstring &to)
{
    if ( from.empty() )
        return;

    size_t start_pos = 0;

    while ( (start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
//std::string wchar_to_UTF8(const wchar_t * in);

JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1MultiTap_1Input
(JNIEnv *env, jclass, jobject jime, jint KeyCode, int PrevKeyCode, jint Count)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        sAll = IQQI_MultiTap_Input(_IME, KeyCode, PrevKeyCode, Count);

#if defined(__SPLT)

        std::vector<std::wstring>
        output;

        std::string::size_type
        prev_pos = 0,
        pos = 0;

        while ( (pos = sAll.find('|', pos)) != std::string::npos )
        {
            std::wstring
            substring( sAll.substr(prev_pos, pos - prev_pos) );

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
            LOGD("Kika-ENGINE, MultiTap_Input =%d\r\n", substring);
#endif

            output.push_back(substring);

            prev_pos = ++pos;
        }

        output.push_back(sAll.substr(prev_pos, pos - prev_pos)); // Last word

#else

        replaceAll(sAll, L">", L"");

        jstring
        jstr = toJString(env, sAll.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, MultiTap_Input, KeyCode =%d, PrevKeyCode =%d, out =%ls (vLen =%d)\r\n", KeyCode, PrevKeyCode, sAll.c_str(), (int)sAll.size());
#endif

        return jstr;

#endif
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return toJString(env, L"");
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1GetNextWordCandidates
(JNIEnv *env, jclass, jobject jime, jstring KeyCode, jint Begin, jint Number, jobjectArray Candidates)
{
    try
    {
        CandidateCH
        CandVoca;
        CandVoca.alloc(Number, CANDIDATE_INPUT_LIMIT);

        jint
        _IME = getImeID(env, jime);

        wstring
        wsKeyCode = IQQI_JstrToWstr(env, KeyCode);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetNextWordCandidates IME =%d, Keyin =%ls (vLen =%d), Begin =%d, Number =%d\r\n", _IME, (wchar_t *)wsKeyCode.c_str(), (int)wsKeyCode.size(), Begin, Number);
#endif

        int
        Total = IQQI_GetNextWordCandidates(_IME, (wchar_t *) wsKeyCode.c_str(), Begin,
                                           Number, CandVoca.pointer());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetNextWordCandidates =%d\r\n", Total);
#endif

        if (Total > 0)
        {
            for (int k = 0; k < Total; k++)
            {
                if (IQQI_SetCandidateInfo(env, _IME, CandVoca.record(k), NULL, Candidates, k) ==
                        false)
                {
                    return 0;
                }
            }
        }
        return Total;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1LearnWord
(JNIEnv *env, jclass, jobject jime, jstring jPinyinCode, jstring jContent)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wKeys = IQQI_JstrToWstr(env, jPinyinCode);

        wstring
        wContent = IQQI_JstrToWstr(env, jContent);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, LearnWord IME =%d, Keys =%ls, Content =%ls\r\n", _IME, wKeys.c_str(), wContent.c_str());
#endif

        int
        retVal = IQQI_LearnWord(_IME, (wchar_t *) wKeys.c_str(),
                                (wchar_t *) wContent.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, LearnWord ret =%d\r\n", retVal);
#endif
        return retVal;

    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1LearnWordEx
(JNIEnv *env, jclass, jobject jime, jstring jPinyinCode, jstring jContent, jboolean ForImport, jboolean IsMultiTap)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wKeys = IQQI_JstrToWstr(env, jPinyinCode);

        wstring
        wContent = IQQI_JstrToWstr(env, jContent);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, LearnWordEx IME =%d, Keys =%ls, Content =%ls, ForImport =%d\r\n", _IME, wKeys.c_str(), wContent.c_str(), ForImport);
#endif

        int
        retVal = IQQI_LearnWordEx(_IME, (wchar_t *) wKeys.c_str(),
                                  (wchar_t *) wContent.c_str(), ForImport, IsMultiTap);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, LearnWordEx ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1LearnWordSaveAs
(JNIEnv *env, jclass, jobject jime, jstring jUserDBName, jstring jPinyinCode, jstring jContent, jboolean ForImport, jboolean IsMultiTap)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        string
        sUserDBName = env->GetStringUTFChars(jUserDBName, JNI_FALSE);

        wstring
        wKeys = IQQI_JstrToWstr(env, jPinyinCode);

        wstring
        wContent = IQQI_JstrToWstr(env, jContent);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, LearnWordSaveAs IME =%d, sUserDBName =%ls, Keys =%ls, Content =%ls, ForImport =%d\r\n", _IME, (wchar_t *)sUserDBName.c_str(), (wchar_t *)wKeys.c_str(), (wchar_t *)wContent.c_str(), ForImport);
#endif

        int
        retVal = IQQI_LearnWordSaveAs(_IME, sUserDBName.c_str(),
                                      (wchar_t *) wKeys.c_str(),
                                      (wchar_t *) wContent.c_str(), ForImport,
                                      IsMultiTap);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, LearnWordEx ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1DeleLearnedWord
(JNIEnv *env, jclass, jobject jime, jstring jPinyinCode, jstring jContent)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wKeys = IQQI_JstrToWstr(env, jPinyinCode);

        wstring
        wContent = IQQI_JstrToWstr(env, jContent);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, DeleLearnedWord IME =%d, Keys =%ls, Content =%ls\r\n", _IME, wKeys.c_str(), wContent.c_str());
#endif

        int
        retVal = IQQI_DeleLearnedWord(_IME, (wchar_t *) wKeys.c_str(),
                                      (wchar_t *) wContent.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, DeleLearnedWord ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1DeleLearnedWordFrom
(JNIEnv *env, jclass, jobject jime, jstring jUserDBName, jstring jPinyinCode, jstring jContent)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        string
        sUserDBName = env->GetStringUTFChars(jUserDBName, JNI_FALSE);

        wstring
        wKeys = IQQI_JstrToWstr(env, jPinyinCode);

        wstring
        wContent = IQQI_JstrToWstr(env, jContent);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, DeleLearnedWordFrom IME =%d, Keys =%ls, Content =%ls\r\n", _IME, wKeys.c_str(), wContent.c_str());
#endif

        int
        retVal = IQQI_DeleLearnedWordFrom(_IME, sUserDBName.c_str(),
                                          (wchar_t *) wKeys.c_str(),
                                          (wchar_t *) wContent.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, DeleLearnedWord ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1ResetLearnedWordFromFile
(JNIEnv *env, jclass, jobject jime, jstring jUserDBName)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        string
        sUserDBName = env->GetStringUTFChars(jUserDBName, JNI_FALSE);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, ResetLearnedWordFromFile IME =%d, File =%ls\r\n", _IME, (wchar_t *)sUserDBName.c_str());
#endif

        int
        retVal = IQQI_ResetLearnedWordFromFile(_IME, sUserDBName.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, ResetLearnedWordFromFile ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1InitialDictionary
(JNIEnv *env, jclass, jobject jime, jstring jDictName)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        const char *
        sDictName = env->GetStringUTFChars(jDictName, JNI_FALSE);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, InitialDictionary IME =%d, FileName =%s\r\n", _IME, sDictName);
#endif

        int
        retVal = IQQI_InitialDictionary(0, sDictName);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        if ( retVal == -1 )
            LOGD("Kika-ENGINE, InitialDictionary ret =%d, file not exist!\r\n", retVal);
        else
            LOGD("Kika-ENGINE, InitialDictionary ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1IsVocabularyExist
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wVoca = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, IsVocabularyExist IME =%d, Voca =%ls\r\n", _IME, wVoca.c_str());
#endif

        int
        retVal = IQQI_IsVocabularyExist(_IME, wVoca.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, IsVocabularyExist ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1AppendVocabulary
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wVoca = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, AppendVocabulary IME =%d, Voca =%ls\r\n", _IME, wVoca.c_str());
#endif

        int
        retVal = IQQI_AppendVocabulary(_IME, wVoca.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, AppendVocabulary =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1DeleteVocabulary
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wVoca = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, DeleteVocabulary IME =%d, Voca =%ls\r\n", _IME, wVoca.c_str());
#endif

        int
        retVal = IQQI_DeleteVocabulary(_IME, wVoca.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, DeleteVocabulary ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1WordExist
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wVoca = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, WordExist IME =%d, Voca =%ls\r\n", _IME, wVoca.c_str());
#endif

        int
        retVal = IQQI_WordExist(_IME, wVoca.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, WordExist ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1IsLearnWordExist
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wVoca = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, IsLearnWordExist IME =%d, Voca =%ls\r\n", _IME, wVoca.c_str());
#endif

        int
        retVal = IQQI_IsLearnWordExist(_IME, L"", wVoca.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, IsLearnWordExist ret =%d\r\n", retVal);
#endif

        return retVal;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1GetComposingText
(JNIEnv *env, jclass, jobject jime, jstring jsKeyCode, jint type, jint aIndicator)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wKeyCode = IQQI_JstrToWstr(env, jsKeyCode);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetComposingText IME =%d, KeyCode =%ls\r\n", _IME, wKeyCode.c_str());
#endif

        const wchar_t *
        wsVoca = IQQI_GetComposingText(_IME, (wchar_t *) wKeyCode.c_str(), type,
                                       aIndicator);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetComposingText ret =%ls\r\n", wsVoca);
#endif

        jstring
        jstr = toJString(env, wsVoca);

        return jstr;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return toJString(env, L"");
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1NMapKoreanCode
(JNIEnv *env, jclass, jobject jime, jstring jsVoca, jboolean toComposing)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        wKeyin = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, toComposing =%d, NMapKoreanCode IME =%d, Voca =%ls\r\n", toComposing, _IME, wKeyin.c_str());
#endif

        wstring
        wKrKey;

        if (mKeyin.size() == 0)
        {
            long
            total = MAX_key_korean_data_ID;

            key_korean_data_ID
            KeyID;

            for (long i = 0; i < total; i++)
            {
                wchar_t *
                pWD = KeyID.GetWDataSz(i);

                pWD[1] = 0;

                wchar_t *
                pKey = pWD + 2;

                mKeyin[pKey] = pWD;
            }
        }

        wstring
        wsKeyTmp = wKeyin,
        wsKeyOrg = wKeyin;

        while (wKeyin.size())
        {
            map<wstring, wstring>::iterator
            p = mKeyin.find(wKeyin);

            if (p != mKeyin.end())
            {
                wstring
                wKey = mKeyin[wKeyin];

                wKrKey += wKey;
                wsKeyTmp.erase(0, wKeyin.size());
                wKeyin = wsKeyTmp;

                continue;
            }
            wKeyin.erase(wKeyin.size() - 1, 1);
        }

        if (toComposing == true)
        {
            trans_key_korean
            korean;

            wstring
            wVoca = korean.getComposingText(wKrKey, false);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
            LOGD("Kika-ENGINE, toComposing true, NMapKoreanCode ret =%ls\r\n", wVoca.c_str());
#endif

            jstring
            jstr = toJString(env, wVoca.c_str());

            return jstr;
        }
        else
        {
#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
            LOGD("Kika-ENGINE, toComposing false, NMapKoreanCode ret =%ls\r\n", wKrKey.c_str());
#endif

            jstring
            jstr = toJString(env, wKrKey.c_str());

            return jstr;
        }
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return toJString(env, L"");
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1NMapKeyCode
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        sAll = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, NMapKeyCode IME =%d, Voca =%ls\r\n", _IME, sAll.c_str());
#endif

        const wchar_t *
        wsVoca = IQQI_NMapKeyCode(_IME, sAll.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, NMapKeyCode ret =%ls\r\n", wsVoca);
#endif

        jstring
        jstr = toJString(env, wsVoca);

        return jstr;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return toJString(env, L"");
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1KeyCodeNMap
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        sAll = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, IQQI_KeyCodeNMap IME =%d, Voca =%ls\r\n", _IME, sAll.c_str());
#endif

        const wchar_t *
        wsVoca = IQQI_KeyCodeNMap(_IME, sAll.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, IQQI_KeyCodeNMap ret =%ls\r\n", wsVoca);
#endif

        jstring
        jstr = toJString(env, wsVoca);

        return jstr;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return toJString(env, L"");
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1ShiftKeyCode
(JNIEnv *env, jclass, jobject jime, jstring jsVoca)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

        wstring
        sAll = IQQI_JstrToWstr(env, jsVoca);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, ShiftKeyCode IME =%d, Voca =%ls\r\n", _IME, sAll.c_str());
#endif

        const wchar_t *
        wsVoca = IQQI_ShiftKeyCode(_IME, sAll.c_str());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, ShiftKeyCode ret =%ls\r\n", wsVoca);
#endif

        jstring
        jstr = toJString(env, wsVoca);

        return jstr;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return toJString(env, L"");
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1GetLearnedWordFromFile
(JNIEnv *env, jclass, jobject jime, jstring jUserDBName, jint Number, jobjectArray Candidates)
{
    try
    {
        CandidateCH
        CandVoca;
        CandVoca.alloc(Number, CANDIDATE_INPUT_LIMIT);

        jint
        _IME = getImeID(env, jime);

        string
        sUserDBName = env->GetStringUTFChars(jUserDBName, JNI_FALSE);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        ////LOGD("Kika-ENGINE, GetLearnedWords IME =%d, UserDBName =%ls, _IME, (wchar_t*)wUserDBName);
#endif

        int
        Total = IQQI_GetLearnedWordFromFile(_IME, sUserDBName.c_str(), Number,
                                            CandVoca.pointer());

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetLearnedWords =%d\r\n", Total);
#endif

        if (Total > 0)
        {
            for (int k = 0; k < Total; k++)
            {
                if (IQQI_SetCandidateInfo(env, _IME, CandVoca.record(k), NULL, Candidates, k) ==
                        false)
                {
                    return 0;
                }
            }
        }
        return Total;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static jobjectArray make_row(JNIEnv *env, jsize count, key key)
{
    try
    {
        jclass
        intClass = env->FindClass("java/lang/Integer");

        jobjectArray
        row = env->NewObjectArray(count, intClass, 0);

        jmethodID
        midInit = env->GetMethodID(intClass, "<init>", "(I)V");

        env->SetObjectArrayElement(row, 0, env->NewObject(intClass, midInit, key.x));
        env->SetObjectArrayElement(row, 1, env->NewObject(intClass, midInit, key.y));
        env->SetObjectArrayElement(row, 2, env->NewObject(intClass, midInit, key.width));
        env->SetObjectArrayElement(row, 3, env->NewObject(intClass, midInit, key.height));
        env->SetObjectArrayElement(row, 4, env->NewObject(intClass, midInit, key.code));

        return row;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return env->NewObjectArray(0, env->FindClass("java/lang/Integer"), 0);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jobjectArray JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_IQQI_1GetVirtualKeybdLayout
(JNIEnv *env, jclass, jobject jime, jboolean isQwerty)
{
    try
    {
        jint
        _IME = getImeID(env, jime);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetVirtualKeybdLayout IME =%d, isQwerty =%d\r\n", _IME, isQwerty);
#endif

        int
        total = 0;//IQQI_GetVirtualKeybdLayout(_IME, isQwerty, bKey);

        key *
        bKey = IQQI_GetVirtualKeybdLayout(_IME, isQwerty, total);

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
        LOGD("Kika-ENGINE, GetVirtualKeybdLayout IME =%d, isQwerty =%d, total =%d\r\n", _IME, isQwerty, total);
#endif

        jobjectArray
        rows = env->NewObjectArray(total, env->FindClass("java/lang/Integer"), 0);

        if (total > 0)
        {
            jobjectArray
            jitem = make_row(env, 5, bKey[0]);

            rows = env->NewObjectArray(total, env->GetObjectClass(jitem), 0);

            for (int i = 0; i < total; i++)
            {
                jitem = make_row(env, 5, bKey[i]);

                env->SetObjectArrayElement(rows, i, jitem);
            }
        }
        return rows;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return env->NewObjectArray(0, env->FindClass("java/lang/Integer"), 0);
}








//-----------------------------------------------------------------------------
// AutoCorrection
//-----------------------------------------------------------------------------
//#include "com_android_inputmethod_latin_BinaryDictionary.h"

#include <cstring> // for memset()
#include <vector>

#include "defines.h"
#include "dictionary/property/unigram_property.h"
#include "dictionary/property/ngram_context.h"
#include "dictionary/property/word_property.h"
#include "dictionary/structure/dictionary_structure_with_buffer_policy_factory.h"
//#include "jni.h"
//#include "jni_common.h"
#include "suggest/core/dictionary/dictionary.h"
#include "suggest/core/result/suggestion_results.h"
#include "suggest/core/suggest_options.h"
#include "suggest/core/session/dic_traverse_session.h"
#include "utils/char_utils.h"
#include "utils/int_array_view.h"
#include "jni_data_utils.h"
#include "utils/profiler.h"
#include "utils/time_keeper.h"

using namespace latinime;

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_openKikaNative
(JNIEnv *env, jclass clazz, jstring sourceDir, jlong dictOffset, jlong dictSize, jboolean isUpdatable)
{
    PROF_INIT;
    PROF_TIMER_START(66);
    try
    {

        const jsize
        sourceDirUtf8Length = env->GetStringUTFLength(sourceDir);

        if (sourceDirUtf8Length <= 0)
        {
            AKLOGE("DICT: Can't get sourceDir string");
            return NOT_A_PROBABILITY;
        }

        char sourceDirChars[sourceDirUtf8Length + 1];
        env->GetStringUTFRegion(sourceDir, 0, env->GetStringLength(sourceDir), sourceDirChars);
        sourceDirChars[sourceDirUtf8Length] = '\0';

        DictionaryStructureWithBufferPolicy::StructurePolicyPtr dictionaryStructureWithBufferPolicy(
            DictionaryStructureWithBufferPolicyFactory::newPolicyForExistingDictFile(
                sourceDirChars,
                static_cast<int>(dictOffset),
                static_cast<int>(dictSize),
                isUpdatable == JNI_TRUE));

        if (!dictionaryStructureWithBufferPolicy)
        {
            return 0;
        }

        Dictionary *const dictionary = new Dictionary(
            std::move(dictionaryStructureWithBufferPolicy));

        //LogUtils::logToJava(env, " %s", dictionary->getVersionInfo().c_str());

        PROF_TIMER_END(66);

        return reinterpret_cast<jlong>(dictionary);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return NOT_A_PROBABILITY;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_createOnMemoryKikaNative
(JNIEnv *env, jclass clazz, jlong formatVersion, jstring locale, jobjectArray attributeKeyStringArray,
 jobjectArray attributeValueStringArray)
{
    try
    {
        const jsize
        localeUtf8Length = env->GetStringUTFLength(locale);

        char
        localeChars[localeUtf8Length + 1];

        env->GetStringUTFRegion(locale, 0, env->GetStringLength(locale), localeChars);
        localeChars[localeUtf8Length] = '\0';

        std::vector<int> localeCodePoints;
        HeaderReadWriteUtils::insertCharactersIntoVector(localeChars, &localeCodePoints);

        const int keyCount = env->GetArrayLength(attributeKeyStringArray);
        const int valueCount = env->GetArrayLength(attributeValueStringArray);

        if (keyCount != valueCount)
        {
            return NOT_A_PROBABILITY;
        }

        DictionaryHeaderStructurePolicy::AttributeMap
        attributeMap = JniDataUtils::constructAttributeMap(
                           env, attributeKeyStringArray,
                           attributeValueStringArray);

        DictionaryStructureWithBufferPolicy::StructurePolicyPtr
        dictionaryStructureWithBufferPolicy = DictionaryStructureWithBufferPolicyFactory::newPolicyForOnMemoryDict(
                formatVersion,
                localeCodePoints,
                &attributeMap);

        if (!dictionaryStructureWithBufferPolicy)
        {
            return 0;
        }

        Dictionary *const dictionary = new Dictionary(
            std::move(dictionaryStructureWithBufferPolicy));
        //LogUtils::logToJava(env, " %s", dictionary->getVersionInfo().c_str());

        return reinterpret_cast<jlong>(dictionary);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return NOT_A_PROBABILITY;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getHeaderInfoKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jintArray outHeaderSize, jintArray outFormatVersion,
 jobject outAttributeKeys, jobject outAttributeValues)
{
    try
    {
        Dictionary *dictionary = reinterpret_cast<Dictionary *>(dict);
        if (!dictionary)
            return;

        const DictionaryHeaderStructurePolicy *const
        headerPolicy = dictionary->getDictionaryStructurePolicy()->getHeaderStructurePolicy();

        JniDataUtils::putIntToArray(env, outHeaderSize, 0 /* index */, headerPolicy->getSize());
        JniDataUtils::putIntToArray(env, outFormatVersion, 0 /* index */,

                                    headerPolicy->getFormatVersionNumber());

        // Output attribute map
        jclass arrayListClass = env->FindClass("java/util/ArrayList");
        jmethodID addMethodId = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

        const DictionaryHeaderStructurePolicy::AttributeMap *const
        attributeMap = headerPolicy->getAttributeMap();

        for (DictionaryHeaderStructurePolicy::AttributeMap::const_iterator it = attributeMap->begin();
                it != attributeMap->end(); ++it)
        {
            // Output key
            jintArray keyCodePointArray = env->NewIntArray(it->first.size());
            JniDataUtils::outputCodePoints(env,
                                           keyCodePointArray,
                                           0                  /* start */,
                                           it->first.size(),
                                           it->first.data(),
                                           it->first.size(),
                                           false              /* needsNullTermination */);

            env->CallBooleanMethod(outAttributeKeys, addMethodId, keyCodePointArray);
            env->DeleteLocalRef(keyCodePointArray);

            // Output value
            jintArray valueCodePointArray = env->NewIntArray(it->second.size());
            JniDataUtils::outputCodePoints(env,
                                           valueCodePointArray,
                                           0                   /* start */,
                                           it->second.size(),
                                           it->second.data(),
                                           it->second.size(),
                                           false               /* needsNullTermination */);

            env->CallBooleanMethod(outAttributeValues, addMethodId, valueCodePointArray);
            env->DeleteLocalRef(valueCodePointArray);
        }
        env->DeleteLocalRef(arrayListClass);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_flushKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jstring filePath)
{
    try
    {
        Dictionary *dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return JNI_FALSE;

        const jsize filePathUtf8Length = env->GetStringUTFLength(filePath);
        char filePathChars[filePathUtf8Length + 1];

        env->GetStringUTFRegion(filePath, 0, env->GetStringLength(filePath), filePathChars);
        filePathChars[filePathUtf8Length] = '\0';

        return dictionary->flush(filePathChars);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_needsToRunGCKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jboolean mindsBlockByGC)
{
    try
    {
        Dictionary *dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return JNI_FALSE;

        return dictionary->needsToRunGC(mindsBlockByGC == JNI_TRUE);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_flushWithGCKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jstring filePath)
{
    try
    {
        Dictionary *dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return JNI_FALSE;

        const jsize
        filePathUtf8Length = env->GetStringUTFLength(filePath);

        char filePathChars[filePathUtf8Length + 1];

        env->GetStringUTFRegion(filePath, 0, env->GetStringLength(filePath), filePathChars);

        filePathChars[filePathUtf8Length] = '\0';

        return dictionary->flushWithGC(filePathChars);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_closeKikaNative
(JNIEnv *env, jclass clazz, jlong dict)
{
    try
    {
        Dictionary *dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return;

        delete dictionary;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getFormatVersionKikaNative
(JNIEnv *env, jclass clazz, jlong dict)
{
    try
    {
        Dictionary *dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return NOT_A_PROBABILITY;

        const DictionaryHeaderStructurePolicy *const
        headerPolicy = dictionary->getDictionaryStructurePolicy()->getHeaderStructurePolicy();

        return headerPolicy->getFormatVersionNumber();
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return NOT_A_PROBABILITY;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getProbabilityKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jintArray word)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return NOT_A_PROBABILITY;

        const jsize
        codePointCount = env->GetArrayLength(word);

        int
        codePoints[codePointCount];

        env->GetIntArrayRegion(word, 0, codePointCount, codePoints);

        return dictionary->getProbability(CodePointArrayView(codePoints, codePointCount));
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return NOT_A_PROBABILITY;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getMaxProbabilityOfExactMatchesKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jintArray word)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return NOT_A_PROBABILITY;

        const jsize
        codePointCount = env->GetArrayLength(word);

        int
        codePoints[codePointCount];

        env->GetIntArrayRegion(word, 0, codePointCount, codePoints);

        return dictionary->getMaxProbabilityOfExactMatches(
                   CodePointArrayView(codePoints, codePointCount));
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return NOT_A_PROBABILITY;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getNgramProbabilityKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jobjectArray prevWordCodePointArrays,
 jbooleanArray isBeginningOfSentenceArray, jintArray word)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return NOT_A_PROBABILITY; // WTF: bool to int?!!!
        }

        const jsize wordLength = env->GetArrayLength(word);
        int wordCodePoints[wordLength];

        env->GetIntArrayRegion(word, 0, wordLength, wordCodePoints);

        const NgramContext
        ngramContext = JniDataUtils::constructNgramContext(env,
                       prevWordCodePointArrays,
                       isBeginningOfSentenceArray,
                       env->GetArrayLength(
                           prevWordCodePointArrays));

        return dictionary->getNgramProbability(&ngramContext,
                                               CodePointArrayView(wordCodePoints, wordLength));
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return NOT_A_PROBABILITY;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getWordPropertyKikaNative
(JNIEnv *env, jclass clazz,
 jlong dict, jintArray word, jboolean isBeginningOfSentence, jintArray outCodePoints,
 jbooleanArray outFlags, jintArray outProbabilityInfo, jobject outNgramPrevWordsArray,
 jobject outNgramPrevWordIsBeginningOfSentenceArray, jobject outNgramTargets,
 jobject outNgramProbabilityInfo, jobject outShortcutTargets,
 jobject outShortcutProbabilities)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return;

        const jsize wordLength = env->GetArrayLength(word);

        if (wordLength > MAX_WORD_LENGTH)
        {
            AKLOGE("Invalid wordLength: %d", wordLength);
            return;
        }
        int wordCodePoints[MAX_WORD_LENGTH];
        env->GetIntArrayRegion(word, 0, wordLength, wordCodePoints);

        int codePointCount = wordLength;
        if (isBeginningOfSentence)
        {
            codePointCount = CharUtils::attachBeginningOfSentenceMarker(
                                 wordCodePoints, wordLength, MAX_WORD_LENGTH);

            if (codePointCount < 0)
            {
                AKLOGE("Cannot attach Beginning-of-Sentence marker.");
                return;
            }
        }
        const WordProperty
        wordProperty = dictionary->getWordProperty(
                           CodePointArrayView(wordCodePoints, codePointCount));

        JniDataUtils::outputWordProperty(env,
                                         wordProperty,
                                         outCodePoints,
                                         outFlags,
                                         outProbabilityInfo,
                                         outNgramPrevWordsArray,
                                         outNgramPrevWordIsBeginningOfSentenceArray,
                                         outNgramTargets,
                                         outNgramProbabilityInfo,
                                         outShortcutTargets,
                                         outShortcutProbabilities);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getNextWordKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jint token, jintArray outCodePoints, jbooleanArray outIsBeginningOfSentence)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
            return 0;

        const jsize
        codePointBufSize = env->GetArrayLength(outCodePoints);

        if (codePointBufSize != MAX_WORD_LENGTH)
        {
            AKLOGE("Invalid outCodePointsLength: %d", codePointBufSize);
            ASSERT(false);
            return 0;
        }

        int wordCodePoints[codePointBufSize];
        int wordCodePointCount = 0;

        const int
        nextToken = dictionary->getNextWordAndNextToken(token, wordCodePoints,
                    &wordCodePointCount);

        JniDataUtils::outputCodePoints(env,
                                       outCodePoints, 0 /* start */,
                                       MAX_WORD_LENGTH  /* maxLength */,
                                       wordCodePoints,
                                       wordCodePointCount,
                                       false            /* needsNullTermination */);

        bool
        isBeginningOfSentence = false;

        if (wordCodePointCount > 0 && wordCodePoints[0] == CODE_POINT_BEGINNING_OF_SENTENCE)
        {
            isBeginningOfSentence = true;
        }

        JniDataUtils::putBooleanToArray(env,
                                        outIsBeginningOfSentence,
                                        0 /* index */,
                                        isBeginningOfSentence);

        return nextToken;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_addUnigramEntryKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jintArray word, jint probability,
 jintArray shortcutTarget, jint shortcutProbability, jboolean isBeginningOfSentence,
 jboolean isNotAWord, jboolean isPossiblyOffensive, jint timestamp)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return JNI_FALSE;
        }
        jsize
        codePointCount = env->GetArrayLength(word);

        int codePoints[codePointCount];
        env->GetIntArrayRegion(word, 0, codePointCount, codePoints);

        std::vector<UnigramProperty::ShortcutProperty> shortcuts;
        {
            std::vector<int>
            shortcutTargetCodePoints;

            JniDataUtils::jintarrayToVector(env, shortcutTarget, &shortcutTargetCodePoints);

            if (!shortcutTargetCodePoints.empty())
            {
                shortcuts.emplace_back(std::move(shortcutTargetCodePoints), shortcutProbability);
            }
        }

        // Use 1 for count to indicate the word has inputted.
        const UnigramProperty
        unigramProperty(isBeginningOfSentence,
                        isNotAWord,
                        isPossiblyOffensive,
                        probability,
                        HistoricalInfo(timestamp,
                                       0 /* level */,
                                       1 /* count */), std::move(shortcuts));

        return dictionary->addUnigramEntry(CodePointArrayView(codePoints, codePointCount),
                                           &unigramProperty);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_removeUnigramEntryKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jintArray word)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return JNI_FALSE;
        }
        jsize
        codePointCount = env->GetArrayLength(word);

        int codePoints[codePointCount];
        env->GetIntArrayRegion(word, 0, codePointCount, codePoints);

        return dictionary->removeUnigramEntry(CodePointArrayView(codePoints, codePointCount));
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_addNgramEntryKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jobjectArray prevWordCodePointArrays,
 jbooleanArray isBeginningOfSentenceArray, jintArray word, jint probability, jint timestamp)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return JNI_FALSE;
        }

        const NgramContext
        ngramContext = JniDataUtils::constructNgramContext(env,
                       prevWordCodePointArrays,
                       isBeginningOfSentenceArray,
                       env->GetArrayLength(
                           prevWordCodePointArrays));

        jsize
        wordLength = env->GetArrayLength(word);

        int wordCodePoints[wordLength];

        env->GetIntArrayRegion(word, 0, wordLength, wordCodePoints);

        // Use 1 for count to indicate the ngram has inputted.
        const NgramProperty
        ngramProperty(ngramContext,
                      CodePointArrayView(wordCodePoints, wordLength).toVector(),
                      probability, HistoricalInfo(timestamp, 0 /* level */, 1 /* count */));

        return dictionary->addNgramEntry(&ngramProperty);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_removeNgramEntryKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jobjectArray prevWordCodePointArrays,
 jbooleanArray isBeginningOfSentenceArray, jintArray word)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return JNI_FALSE;
        }

        const NgramContext
        ngramContext = JniDataUtils::constructNgramContext(env,
                       prevWordCodePointArrays,
                       isBeginningOfSentenceArray,
                       env->GetArrayLength(
                           prevWordCodePointArrays));

        jsize
        codePointCount = env->GetArrayLength(word);

        int wordCodePoints[codePointCount];
        env->GetIntArrayRegion(word, 0, codePointCount, wordCodePoints);

        return dictionary->removeNgramEntry(&ngramContext,
                                            CodePointArrayView(wordCodePoints, codePointCount));
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_updateEntriesForWordWithNgramContextKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jobjectArray prevWordCodePointArrays,
 jbooleanArray isBeginningOfSentenceArray, jintArray word, jboolean isValidWord, jint count, jint timestamp)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return JNI_FALSE;
        }

        const NgramContext
        ngramContext = JniDataUtils::constructNgramContext(env,
                       prevWordCodePointArrays,
                       isBeginningOfSentenceArray,
                       env->GetArrayLength(
                           prevWordCodePointArrays));

        jsize
        codePointCount = env->GetArrayLength(word);

        int wordCodePoints[codePointCount];
        env->GetIntArrayRegion(word, 0, codePointCount, wordCodePoints);

        const HistoricalInfo historicalInfo(timestamp, 0 /* level */, count);

        return dictionary->updateEntriesForWordWithNgramContext(&ngramContext,
                CodePointArrayView(wordCodePoints,
                                   codePointCount),
                isValidWord == JNI_TRUE,
                historicalInfo);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getPropertyKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jstring query)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return env->NewStringUTF("");
        }

        const jsize
        queryUtf8Length = env->GetStringUTFLength(query);

        char queryChars[queryUtf8Length + 1];
        env->GetStringUTFRegion(query, 0, env->GetStringLength(query), queryChars);

        queryChars[queryUtf8Length] = '\0';

        static const int GET_PROPERTY_RESULT_LENGTH = 100;
        char resultChars[GET_PROPERTY_RESULT_LENGTH];
        resultChars[0] = '\0';

        dictionary->getProperty(queryChars, queryUtf8Length, resultChars,
                                GET_PROPERTY_RESULT_LENGTH);

        return env->NewStringUTF(resultChars);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return env->NewStringUTF("");
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_isCorruptedKikaNative
(JNIEnv *env, jclass clazz, jlong dict)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return JNI_FALSE;
        }
        return dictionary->getDictionaryStructurePolicy()->isCorrupted();
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

static
DictionaryStructureWithBufferPolicy::StructurePolicyPtr runGCAndGetNewStructurePolicy(
    DictionaryStructureWithBufferPolicy::StructurePolicyPtr structurePolicy,
    const char *const dictFilePath)
{
    structurePolicy->flushWithGC(dictFilePath);
    structurePolicy.release();

    return DictionaryStructureWithBufferPolicyFactory::newPolicyForExistingDictFile(
               dictFilePath,
               0 /* offset */,
               0 /* size */,
               true /* isUpdatable */);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_migrateKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jstring dictFilePath, jlong newFormatVersion)
{
    try
    {
        Dictionary *
        dictionary = reinterpret_cast<Dictionary *>(dict);

        if (!dictionary)
        {
            return JNI_FALSE;
        }

        const jsize
        filePathUtf8Length = env->GetStringUTFLength(dictFilePath);

        char dictFilePathChars[filePathUtf8Length + 1];
        env->GetStringUTFRegion(dictFilePath, 0, env->GetStringLength(dictFilePath),
                                dictFilePathChars);
        dictFilePathChars[filePathUtf8Length] = '\0';

        const DictionaryHeaderStructurePolicy *const
        headerPolicy = dictionary->getDictionaryStructurePolicy()->getHeaderStructurePolicy();

        DictionaryStructureWithBufferPolicy::StructurePolicyPtr
        dictionaryStructureWithBufferPolicy = DictionaryStructureWithBufferPolicyFactory::newPolicyForOnMemoryDict(
                newFormatVersion, *headerPolicy->getLocale(), headerPolicy->getAttributeMap());

        if (!dictionaryStructureWithBufferPolicy)
        {
            //LogUtils::logToJava(env, "Cannot migrate header.");
            return JNI_FALSE;
        }

        int wordCodePoints[MAX_WORD_LENGTH];
        int wordCodePointCount = 0;
        int token = 0;

        // Add unigrams.
        do
        {
            token = dictionary->getNextWordAndNextToken(token, wordCodePoints, &wordCodePointCount);
            const WordProperty wordProperty = dictionary->getWordProperty(
                                                  CodePointArrayView(wordCodePoints, wordCodePointCount));

            if (wordCodePoints[0] == CODE_POINT_BEGINNING_OF_SENTENCE)
            {
                // Skip beginning-of-sentence unigram.
                continue;
            }

            if (dictionaryStructureWithBufferPolicy->needsToRunGC(true /* mindsBlockByGC */))
            {
                dictionaryStructureWithBufferPolicy = runGCAndGetNewStructurePolicy(
                        std::move(dictionaryStructureWithBufferPolicy), dictFilePathChars);

                if (!dictionaryStructureWithBufferPolicy)
                {
                    //LogUtils::logToJava(env, "Cannot open dict after GC.");
                    return JNI_FALSE;
                }
            }

            if (!dictionaryStructureWithBufferPolicy->addUnigramEntry(
                        CodePointArrayView(wordCodePoints, wordCodePointCount),
                        &wordProperty.getUnigramProperty()))
            {
                //LogUtils::logToJava(env, "Cannot add unigram to the new dict.");
                return JNI_FALSE;
            }
        }
        while (token != 0);

        // Add ngrams.
        do
        {
            token = dictionary->getNextWordAndNextToken(token, wordCodePoints, &wordCodePointCount);

            const WordProperty
            wordProperty = dictionary->getWordProperty(
                               CodePointArrayView(wordCodePoints, wordCodePointCount));

            if (dictionaryStructureWithBufferPolicy->needsToRunGC(true /* mindsBlockByGC */))
            {
                dictionaryStructureWithBufferPolicy = runGCAndGetNewStructurePolicy(
                        std::move(dictionaryStructureWithBufferPolicy), dictFilePathChars);

                if (!dictionaryStructureWithBufferPolicy)
                {
                    //LogUtils::logToJava(env, "Cannot open dict after GC.");
                    return JNI_FALSE;
                }
            }

            for (const NgramProperty &ngramProperty : wordProperty.getNgramProperties())
            {
                if (!dictionaryStructureWithBufferPolicy->addNgramEntry(&ngramProperty))
                {
                    //LogUtils::logToJava(env, "Cannot add ngram to the new dict.");
                    return JNI_FALSE;
                }
            }
        }
        while (token != 0);

        // Save to File.
        dictionaryStructureWithBufferPolicy->flushWithGC(dictFilePathChars);

        return true;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}




//-----------------------------------------------------------------------------
// for Session
//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: (Ljava/lang/String;J)J
//
//-----------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_setDicTraverseSessionKikaNative
(JNIEnv *env, jclass clazz, jstring localeJStr, jlong dictSize)
{
    try
    {
        void *traverseSession = DicTraverseSession::getSessionInstance((int) dictSize);

        return reinterpret_cast<jlong>(traverseSession);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: (JJ[II)V
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_initDicTraverseSessionKikaNative
(JNIEnv *env, jclass clazz, jlong traverseSession, jlong dictionary, jintArray previousWord, jint previousWordLength)
{
    try
    {
        DicTraverseSession *
        ts = reinterpret_cast<DicTraverseSession *>(traverseSession);

        if (!ts)
        {
            return;
        }
        Dictionary *
        dict = reinterpret_cast<Dictionary *>(dictionary);

        if (!previousWord)
        {
            NgramContext emptyNgramContext;
            ts->init(dict, &emptyNgramContext, 0 /* suggestOptions */);
            return;
        }
        int prevWord[previousWordLength];
        env->GetIntArrayRegion(previousWord, 0, previousWordLength, prevWord);

        NgramContext
        ngramContext(prevWord,
                     previousWordLength,
                     false /* isStartOfSentence */);

        ts->init(dict, &ngramContext, 0 /* suggestOptions */);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }

}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: (J)V
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_releaseDicTraverseSessionKikaNative
(JNIEnv *env, jclass clazz, jlong traverseSession)
{
    try
    {
        DicTraverseSession *
        ts = reinterpret_cast<DicTraverseSession *>(traverseSession);

        DicTraverseSession::releaseSessionInstance(ts);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: (JJJ[I[I[I[I[II[IZ[[I[I[ZI[I[I[I[I[I[I[F)V
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_getSuggestionsKikaNative
(JNIEnv *env, jclass clazz, jlong dict, jlong proximityInfo, jlong dicTraverseSession,
 jintArray xCoordinatesArray, jintArray yCoordinatesArray, jintArray timesArray, jintArray pointerIdsArray,
 jintArray inputCodePointsArray, jint inputSize, jintArray suggestOptions,
 jboolean useRNN, jobjectArray lmWordCodePointArrays, jintArray rnnWordScoreArray,
 jbooleanArray isBeginningOfSentenceArray, jint lmWordCount,
 jintArray outSuggestionCount, jintArray outCodePointsArray,
 jintArray outScoresArray, jintArray outSpaceIndicesArray, jintArray outTypesArray,
 jintArray outAutoCommitFirstWordConfidenceArray,
 jfloatArray inOutWeightOfLangModelVsSpatialModel)
{
    try
    {
        Dictionary *dictionary = reinterpret_cast<Dictionary *>(dict);

        // Assign 0 to outSuggestionCount here in case of returning earlier in this method.
        JniDataUtils::putIntToArray(env, outSuggestionCount, 0 /* index */, 0);
        if (!dictionary)
        {
            return;
        }

        ProximityInfo *
        pInfo = reinterpret_cast<ProximityInfo *>(proximityInfo);

        DicTraverseSession *
        traverseSession = reinterpret_cast<DicTraverseSession *>(dicTraverseSession);

        if (!traverseSession)
        {
            return;
        }

        // Input values
        int xCoordinates[inputSize];
        int yCoordinates[inputSize];
        int times[inputSize];
        int pointerIds[inputSize];
        const jsize inputCodePointsLength = env->GetArrayLength(inputCodePointsArray);
        int inputCodePoints[inputCodePointsLength];
        env->GetIntArrayRegion(xCoordinatesArray, 0, inputSize, xCoordinates);
        env->GetIntArrayRegion(yCoordinatesArray, 0, inputSize, yCoordinates);
        env->GetIntArrayRegion(timesArray, 0, inputSize, times);
        env->GetIntArrayRegion(pointerIdsArray, 0, inputSize, pointerIds);
        env->GetIntArrayRegion(inputCodePointsArray, 0, inputCodePointsLength, inputCodePoints);

        const jsize numberOfOptions = env->GetArrayLength(suggestOptions);
        int options[numberOfOptions];
        env->GetIntArrayRegion(suggestOptions, 0, numberOfOptions, options);
        SuggestOptions givenSuggestOptions(options, numberOfOptions);

        // Output values
        /* By the way, let's check the output array length here to make sure */
        const jsize outputCodePointsLength = env->GetArrayLength(outCodePointsArray);

        if (outputCodePointsLength != (MAX_WORD_LENGTH * MAX_RESULTS))
        {
            AKLOGE("Invalid outputCodePointsLength: %d", outputCodePointsLength);
            ASSERT(false);
            return;
        }

        const jsize scoresLength = env->GetArrayLength(outScoresArray);
        if (scoresLength != MAX_RESULTS)
        {
            AKLOGE("Invalid scoresLength: %d", scoresLength);
            ASSERT(false);
            return;
        }

        const jsize
        outputAutoCommitFirstWordConfidenceLength = env->GetArrayLength(
                    outAutoCommitFirstWordConfidenceArray);

        ASSERT(outputAutoCommitFirstWordConfidenceLength == 1);

        if (outputAutoCommitFirstWordConfidenceLength != 1)
        {
            // We only use the first result, as obviously we will only ever autocommit the first one
            AKLOGE("Invalid outputAutoCommitFirstWordConfidenceLength: %d",
                   outputAutoCommitFirstWordConfidenceLength);
            ASSERT(false);
            return;
        }

        // I doubt this parameter is really used.
        // It looks like that it is only copied to output...
        float weightOfLangModelVsSpatialModel;

        env->GetFloatArrayRegion(inOutWeightOfLangModelVsSpatialModel, 0, 1 /* len */,
                                 &weightOfLangModelVsSpatialModel);

        SuggestionResults suggestionResults(MAX_RESULTS);

        NgramContext ngramContext = useRNN ? NgramContext() :
                                    JniDataUtils::constructNgramContext(env,
                                            lmWordCodePointArrays,
                                            isBeginningOfSentenceArray,
                                            lmWordCount);

        if (givenSuggestOptions.isGesture() || inputSize > 0)
        {
            // TODO: Use SuggestionResults to return suggestions.
            traverseSession->getSuggestions(pInfo, dictionary, xCoordinates, yCoordinates,
                                            times, pointerIds, inputCodePoints, inputSize,
                                            &ngramContext,
                                            &givenSuggestOptions, weightOfLangModelVsSpatialModel,
                                            &suggestionResults);
        }
        else
        {
            dictionary->getPredictions(&ngramContext, &suggestionResults);
        }

        if (DEBUG_DICT)
        {
            suggestionResults.dumpSuggestions();
        }

        // Allocate space for answers.
        int N = suggestionResults.getSuggestionCount();
        float W = suggestionResults.getWeightOfLangModelVsSpatialModel();

        int outCodePoints[MAX_WORD_LENGTH * MAX_RESULTS];
        int outScores[MAX_RESULTS];
        int outSpaceIndices[MAX_RESULTS];
        int outTypes[MAX_RESULTS];
        int outAutoCommitFirstWordConfidence = 0;

        suggestionResults.outputSuggestions(
            outCodePoints, outScores,
            outSpaceIndices, outTypes,
            &outAutoCommitFirstWordConfidence);

        // Copy to Java.
        env->SetIntArrayRegion(outCodePointsArray, 0, N * MAX_WORD_LENGTH, outCodePoints);
        env->SetIntArrayRegion(outScoresArray, 0, N, outScores);
        env->SetIntArrayRegion(outSpaceIndicesArray, 0, N, outSpaceIndices);
        env->SetIntArrayRegion(outTypesArray, 0, N, outTypes);
        env->SetIntArrayRegion(outAutoCommitFirstWordConfidenceArray, 0, 1,
                               &outAutoCommitFirstWordConfidence);

        env->SetIntArrayRegion(outSuggestionCount, 0, 1, &N);
        env->SetFloatArrayRegion(inOutWeightOfLangModelVsSpatialModel, 0, 1, &W);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}


//-----------------------------------------------------------------------------
// for BinaryDictionaryUtils
//-----------------------------------------------------------------------------
#include <cstdio>

#include "defines.h"
#include "dictionary/utils/dict_file_writing_utils.h"
#include "utils/autocorrection_threshold_utils.h"
#include "utils/char_utils.h"
#include "jni_data_utils.h"
#include "utils/time_keeper.h"

using namespace latinime;


//
// Return Value :
// Parameters   :
// Remarks      :Signature: (Ljava/lang/String;JLjava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)Z
//
//-----------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_createEmptyDictFileKikaNative
(JNIEnv *env, jclass clazz, jstring filePath, jlong dictVersion,
 jstring locale, jobjectArray attributeKeyStringArray, jobjectArray attributeValueStringArray)
{
    try
    {
        const jsize
        filePathUtf8Length = env->GetStringUTFLength(filePath);

        char filePathChars[filePathUtf8Length + 1];

        env->GetStringUTFRegion(filePath, 0, env->GetStringLength(filePath), filePathChars);
        filePathChars[filePathUtf8Length] = '\0';

        const jsize
        localeUtf8Length = env->GetStringUTFLength(locale);

        char localeChars[localeUtf8Length + 1];
        env->GetStringUTFRegion(locale, 0, env->GetStringLength(locale), localeChars);

        localeChars[localeUtf8Length] = '\0';
        std::vector<int> localeCodePoints;

        HeaderReadWriteUtils::insertCharactersIntoVector(localeChars, &localeCodePoints);

        const int keyCount = env->GetArrayLength(attributeKeyStringArray);
        const int valueCount = env->GetArrayLength(attributeValueStringArray);

        if (keyCount != valueCount)
        {
            return JNI_FALSE;
        }

        DictionaryHeaderStructurePolicy::AttributeMap
        attributeMap = JniDataUtils::constructAttributeMap(env, attributeKeyStringArray,
                       attributeValueStringArray);

        return DictFileWritingUtils::createEmptyDictFile(filePathChars,
                static_cast<int>(dictVersion),
                localeCodePoints,
                &attributeMap);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: ([I[II)F
//
//-----------------------------------------------------------------------------
JNIEXPORT jfloat JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_calcNormalizedScoreKikaNative
(JNIEnv *env, jclass clazz, jintArray before, jintArray after, jint score)
{
    try
    {
        jsize
        beforeLength = env->GetArrayLength(before);

        jsize
        afterLength = env->GetArrayLength(after);

        int beforeCodePoints[beforeLength];
        int afterCodePoints[afterLength];

        env->GetIntArrayRegion(before, 0, beforeLength, beforeCodePoints);
        env->GetIntArrayRegion(after, 0, afterLength, afterCodePoints);

        return AutocorrectionThresholdUtils::calcNormalizedScore(
                   beforeCodePoints,
                   beforeLength,
                   afterCodePoints,
                   afterLength,
                   score);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0.0f;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: (I)I
//
//-----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_setCurrentTimeForTestKikaNative
(JNIEnv *env, jclass clazz, jint currentTime)
{
    try
    {
        if (currentTime >= 0)
        {
            TimeKeeper::startTestModeWithForceCurrentTime(currentTime);
        }
        else
        {
            TimeKeeper::stopTestMode();
        }
        TimeKeeper::setCurrentTime();

        return TimeKeeper::peekCurrentTime();
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}


// -------------------------------------------------------------------------
// for KikaProximityInfo
// -------------------------------------------------------------------------
#include "defines.h"
#include "suggest/core/layout/proximity_info.h"

template <typename _ElemT>
struct _JNIArrayDispatch;

template <>
struct _JNIArrayDispatch<int>
{
    typedef jintArray JNIArrayType;

    static int *capture(JNIEnv *env, JNIArrayType arr)
    {
        return env->GetIntArrayElements(arr, nullptr);
    }

    static void release(JNIEnv *env, JNIArrayType arr, int *captured)
    {
        env->ReleaseIntArrayElements(arr, captured, JNI_ABORT);
    }
};

template <>
struct _JNIArrayDispatch<float>
{
    typedef jfloatArray JNIArrayType;

    static float *capture(JNIEnv *env, JNIArrayType arr)
    {
        return env->GetFloatArrayElements(arr, nullptr);
    }

    static void release(JNIEnv *env, JNIArrayType arr, float *captured)
    {
        env->ReleaseFloatArrayElements(arr, captured, JNI_ABORT);
    }
};

template <typename _ElemT>
class ConstJNIArrayRef
{
private:
    typedef _JNIArrayDispatch<_ElemT> DispatchType;
    typedef typename DispatchType::JNIArrayType JNIArrayType;

    JNIEnv *jniEnv;
    JNIArrayType jniArray;
    mutable _ElemT *captured = nullptr;

    _ElemT *captureArray() const
    {
        return DispatchType::capture(jniEnv, jniArray);
    }

    void releaseArray()
    {
        if (captured)
            DispatchType::release(jniEnv, jniArray, captured);
    }

public:
    typedef _ElemT ElementType;

    ConstJNIArrayRef(JNIEnv *env, JNIArrayType arr)
        : jniEnv(env), jniArray(arr) {}

    ~ConstJNIArrayRef()
    {
        releaseArray();
    }

    const ElementType *get() const
    {
        if (jniArray && !captured)
            captured = captureArray();

        return captured;
    }
};


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: (IIIIII[II[I[I[I[I[I[F[F[F)J
//
//-----------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_setProximityInfoKikaNative
(JNIEnv *env, jclass clazz, jint displayWidth, jint displayHeight, jint gridWidth, jint gridHeight,
 jint mostCommonkeyWidth, jint mostCommonkeyHeight, jintArray proximityChars, jint keyCount,
 jintArray keyXCoordinates, jintArray keyYCoordinates, jintArray keyWidths,
 jintArray keyHeights, jintArray keyCharCodes, jfloatArray sweetSpotCenterXs,
 jfloatArray sweetSpotCenterYs, jfloatArray sweetSpotRadii)
{
    try
    {
#define ASSURE_JARRAY_LENGTH(ja, len) do { \
    jsize actual_len = ja ? env->GetArrayLength(ja) : 0; \
    if (len != actual_len) { \
        AKLOGE("unexpected length of %s, actual len=%d", #ja, actual_len); \
        return (jlong)nullptr; \
    } \
    } while (0)

        // proximityChars has magic size.
        ASSURE_JARRAY_LENGTH(proximityChars, gridWidth * gridHeight * MAX_PROXIMITY_CHARS_SIZE);

        // Other arrays should have size equals to keyCount.
        ASSURE_JARRAY_LENGTH(keyXCoordinates, keyCount);
        ASSURE_JARRAY_LENGTH(keyYCoordinates, keyCount);
        ASSURE_JARRAY_LENGTH(keyWidths, keyCount);
        ASSURE_JARRAY_LENGTH(keyHeights, keyCount);
        ASSURE_JARRAY_LENGTH(keyCharCodes, keyCount);

        if (sweetSpotCenterXs)
            ASSURE_JARRAY_LENGTH(sweetSpotCenterXs, keyCount);
        if (sweetSpotCenterYs)
            ASSURE_JARRAY_LENGTH(sweetSpotCenterYs, keyCount);
        if (sweetSpotRadii)
            ASSURE_JARRAY_LENGTH(sweetSpotRadii, keyCount);

#undef ASSURE_JARRAY_LENGTH

#define JARRAY_MAKE_REF(type, ja) \
    ConstJNIArrayRef<type> ja ##_(env, ja)

        JARRAY_MAKE_REF(int, proximityChars);
        JARRAY_MAKE_REF(int, keyXCoordinates);
        JARRAY_MAKE_REF(int, keyYCoordinates);
        JARRAY_MAKE_REF(int, keyWidths);
        JARRAY_MAKE_REF(int, keyHeights);
        JARRAY_MAKE_REF(int, keyCharCodes);
        JARRAY_MAKE_REF(float, sweetSpotCenterXs);
        JARRAY_MAKE_REF(float, sweetSpotCenterYs);
        JARRAY_MAKE_REF(float, sweetSpotRadii);

#undef JARRAY_MAKE_REF

        ProximityInfo *
        proximityInfo = new ProximityInfo(
            displayWidth,
            displayHeight,
            gridWidth,
            gridHeight,
            mostCommonkeyWidth,
            mostCommonkeyHeight,
            proximityChars_.get(),
            keyCount,
            keyXCoordinates_.get(),
            keyYCoordinates_.get(),
            keyWidths_.get(),
            keyHeights_.get(),
            keyCharCodes_.get(),
            sweetSpotCenterXs_.get(),
            sweetSpotCenterYs_.get(),
            sweetSpotRadii_.get());

        return reinterpret_cast<jlong>(proximityInfo);
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :Signature: (J)V
//
//-----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_kika_qwt9_inputmethod_Resource_qwt9ini_releaseProximityInfoKikaNative
(JNIEnv *env, jclass clazz, jlong proximityInfo)
{
    try
    {
        ProximityInfo *
        pi = reinterpret_cast<ProximityInfo *>(proximityInfo);

        delete pi;
    }
    catch (exception &e)
    {
        KIKA_LOGE("Error:%s", e.what());
    }
}
