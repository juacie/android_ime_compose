/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include <jni.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <exception>

#include "pinyinime.h"
#include "sync.h"
#include "userdict.h"
#include "utf16char.h"

#include <android/log.h>
#define LOG_KIKA_TAG "KIKA_PY"
#define KIKA_LOGE(args...)  __android_log_print(ANDROID_LOG_ERROR, LOG_KIKA_TAG , ## args)



#ifdef __cplusplus
extern "C" {
#endif

using namespace ime_pinyin;

#define RET_BUF_LEN 256

static char16 retbuf[RET_BUF_LEN];
static char16 (*predict_buf)[kMaxPredictSize + 1] = NULL;
static size_t predict_len;

static Sync sync_worker;
    
JNIEXPORT jboolean JNICALL nativeImOpenDecoder(JNIEnv* env, jclass jclazz,
                                               jbyteArray fn_dict,
                                               jbyteArray fn_usr_dict) {
    try {
        jbyte *fud = (*env).GetByteArrayElements(fn_dict, 0);
        size_t fud_len = (size_t) env->GetArrayLength(fn_dict);

        jbyte *usr_fud = (*env).GetByteArrayElements(fn_usr_dict, 0);
        size_t usr_fud_len = (size_t) env->GetArrayLength(fn_usr_dict);

        jboolean jret = JNI_FALSE;

        if (fud_len >= RET_BUF_LEN || usr_fud_len >= RET_BUF_LEN) {
            (*env).ReleaseByteArrayElements(fn_dict, fud, 0);
            (*env).ReleaseByteArrayElements(fn_usr_dict, usr_fud, 0);
            return jret;
        }

        char _fud[RET_BUF_LEN], _usr_fud[RET_BUF_LEN];

        memcpy(_fud, fud, (size_t) fud_len);
        _fud[fud_len] = '\0';

        memcpy(_usr_fud, usr_fud, (size_t) usr_fud_len);
        _usr_fud[usr_fud_len] = '\0';

        if (im_open_decoder(_fud, _usr_fud))
            jret = JNI_TRUE;

        (*env).ReleaseByteArrayElements(fn_dict, fud, 0);
        (*env).ReleaseByteArrayElements(fn_usr_dict, usr_fud, 0);

        return jret;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

JNIEXPORT void JNICALL nativeImSetMaxLens(JNIEnv* env, jclass jclazz,
                                          jint max_sps_len,
                                          jint max_hzs_len) {
    try {
        im_set_max_lens(static_cast<size_t>(max_sps_len),
                        static_cast<size_t>(max_hzs_len));
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
}

JNIEXPORT jboolean JNICALL nativeImCloseDecoder(JNIEnv* env, jclass jclazz) {
    try {
        im_close_decoder();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
  return JNI_TRUE;
}

JNIEXPORT jint JNICALL nativeImSearch(JNIEnv* env, jclass jclazz,
                                      jbyteArray pybuf, jint pylen) {
    try {
        jbyte *array_body = (*env).GetByteArrayElements(pybuf, 0);

        jint jret = 0;
        if (NULL != array_body) {
            jret = im_search((const char *) array_body, pylen);
        }

        (*env).ReleaseByteArrayElements(pybuf, array_body, 0);

        return jret;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jint JNICALL nativeImDelSearch(JNIEnv* env, jclass jclazz, jint pos,
                                         jboolean is_pos_in_splid,
                                         jboolean clear_fixed_this_step) {
    try {
        return im_delsearch(pos, is_pos_in_splid, clear_fixed_this_step);
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT void JNICALL nativeImResetSearch(JNIEnv* env, jclass jclazz) {
    try {
        im_reset_search();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
}

JNIEXPORT jint JNICALL nativeImAddLetter(JNIEnv *env, jclass clazz, jbyte ch) {
    try {
        return im_add_letter(ch);
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jstring JNICALL nativeImGetPyStr(JNIEnv* env, jclass jclazz,
                                           jboolean decoded) {
    try {
        size_t py_len;
        const char *py = im_get_sps_str(&py_len);  // py_len gets decoded length
        assert(NULL != py);
        if (!decoded)
            py_len = strlen(py);

        const unsigned short *spl_start;
        size_t len;
        len = im_get_spl_start_pos(spl_start);

        size_t i;
        for (i = 0; i < py_len; i++)
            retbuf[i] = py[i];
        retbuf[i] = (char16) '\0';

        jstring retstr = (*env).NewString((unsigned short *) retbuf, i);
        return retstr;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return (*env).NewString((unsigned short *) "", 1);
}

JNIEXPORT jint JNICALL nativeImGetPyStrLen(JNIEnv* env, jclass jclazz,
                                           jboolean decoded) {
    try {
        size_t py_len;
        const char *py = im_get_sps_str(&py_len);  // py_len gets decoded length
        assert(NULL != py);
        if (!decoded)
            py_len = strlen(py);
        return py_len;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jintArray JNICALL nativeImGetSplStart(JNIEnv* env, jclass jclazz) {
    try {
        const unsigned short *spl_start;
        size_t len;

        // There will be len + 1 elements in the buffer when len > 0.
        len = im_get_spl_start_pos(spl_start);

        jintArray arr = (*env).NewIntArray(len + 2);
        jint *arr_body = (*env).GetIntArrayElements(arr, 0);
        assert(NULL != arr_body);
        arr_body[0] = len; // element 0 is used to store the length of buffer.
        for (size_t i = 0; i <= len; i++)
            arr_body[i + 1] = spl_start[i];

        (*env).ReleaseIntArrayElements(arr, arr_body, 0);

        return arr;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return (*env).NewIntArray(0);
}

JNIEXPORT jstring JNICALL nativeImGetChoice(JNIEnv *env, jclass clazz,
                                            jint candidateId) {
    try {
        jstring retstr;
        if (im_get_candidate(candidateId, retbuf, RET_BUF_LEN)) {
            retstr = (*env).NewString(retbuf, utf16_strlen(retbuf));
            return retstr;
        } else {
            retstr = (*env).NewString((unsigned short *) retbuf, 0);
            return retstr;
        }
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return (*env).NewString((unsigned short *) "", 0);
}

JNIEXPORT jint JNICALL nativeImChoose(JNIEnv *env, jclass clazz,
                                      jint choice_id) {
    try {
        return im_choose(choice_id);
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jint JNICALL nativeImCancelLastChoice(JNIEnv *env, jclass clazz) {
    try {
        return im_cancel_last_choice();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jint JNICALL nativeImGetFixedLen(JNIEnv *env, jclass clazz) {
    try {
        return im_get_fixed_len();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jboolean JNICALL nativeImCancelInput(JNIEnv *env, jclass clazz) {
    try {
        if (im_cancel_input())
            return JNI_TRUE;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
  return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL nativeImFlushCache(JNIEnv *env, jclass clazz) {
    try {
        im_flush_cache();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
  return JNI_TRUE;
}

JNIEXPORT jint JNICALL nativeImGetPredictsNum(JNIEnv *env, jclass clazz,
                                              jstring fixed_str) {
    try {
        jboolean isCopy = JNI_FALSE;
        char16 *fixed_ptr = (char16 *) (*env).GetStringChars(fixed_str, &isCopy);
        size_t fixed_len = (size_t) (*env).GetStringLength(fixed_str);

        char16 fixed_buf[kMaxPredictSize + 1];

        if (fixed_len > kMaxPredictSize) {
            fixed_ptr += fixed_len - kMaxPredictSize;
            fixed_len = kMaxPredictSize;
        }
        utf16_strncpy(fixed_buf, fixed_ptr, fixed_len);
        fixed_buf[fixed_len] = (char16) '\0';

        predict_len = im_get_predicts(fixed_buf, predict_buf);

        (*env).ReleaseStringChars(fixed_str, fixed_ptr);

        return (jint)predict_len;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jstring JNICALL nativeImGetPredictItem(JNIEnv *env, jclass clazz,
                                                 jint predict_no) {
    try {
        jstring retstr;

        if (predict_no < 0 || (size_t) predict_no >= predict_len) {
            retstr = (*env).NewString((unsigned short *) predict_buf[0], 0);
        } else {
            retstr = (*env).NewString((unsigned short *) predict_buf[predict_no],
                                      utf16_strlen(predict_buf[predict_no]));
        }
        return retstr;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return (*env).NewString((unsigned short *) L"", 0);
}

JNIEXPORT jboolean JNICALL nativeSyncBegin(JNIEnv *env, jclass clazz,
                                           jbyteArray dict_file) {
    try {
        jbyte *file_name = (*env).GetByteArrayElements(dict_file, 0);

        jboolean jret = JNI_FALSE;
        if (true == sync_worker.begin((const char *) file_name))
            jret = JNI_TRUE;

        (*env).ReleaseByteArrayElements(dict_file, file_name, 0);

        return jret;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL nativeSyncFinish(JNIEnv *env, jclass clazz) {
    try {
        sync_worker.finish();
        return JNI_TRUE;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

JNIEXPORT jint JNICALL nativeSyncGetCapacity(JNIEnv *env, jclass clazz) {
    try {
        return sync_worker.get_capacity();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jint JNICALL nativeSyncPutLemmas(JNIEnv *env, jclass clazz,
                                           jstring tomerge) {

    try {
        char16 *ptr = (char16 *) (*env).GetStringChars(tomerge, NULL);
        int len = (size_t) (*env).GetStringLength(tomerge);

        int added = sync_worker.put_lemmas(ptr, len);

        (*env).ReleaseStringChars(tomerge, ptr);

        return added;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

JNIEXPORT jstring JNICALL nativeSyncGetLemmas(JNIEnv *env, jclass clazz) {
    try {
        int len = sync_worker.get_lemmas(retbuf, RET_BUF_LEN);
        if (len == 0)
            return NULL;
        jstring retstr;
        retstr = (*env).NewString((unsigned short *) retbuf, len);
        return retstr;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return (*env).NewString((unsigned short *) L"", 0);

}

JNIEXPORT jint JNICALL nativeSyncGetLastCount(JNIEnv *env, jclass clazz) {
    try {
        return sync_worker.get_last_got_count();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return 0;
}

JNIEXPORT jint JNICALL nativeSyncGetTotalCount(JNIEnv *env, jclass clazz) {
    try {
        return sync_worker.get_total_count();
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return -1;
}

JNIEXPORT jboolean JNICALL nativeSyncClearLastGot(JNIEnv *env, jclass clazz) {
    try {
        sync_worker.clear_last_got();
        return JNI_TRUE;
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
    return JNI_FALSE;
}

JNIEXPORT void JNICALL nativeImSetDictComp(JNIEnv *env, jclass clazz, jboolean isComp) {
    try {
        im_set_spl_homo(!isComp);
    } catch (std::exception &e) {
        KIKA_LOGE("Error:%s", e.what());
    }
}

/**
 * Table of methods associated with a single class.
 */
static JNINativeMethod gMethods[] = {
    /* name, signature, funcPtr */
    /* ------Functions for Pinyin-to-hanzi decoding begin--------->> */
    { "nativeImOpenDecoder", "([B[B)Z",
        (void*) nativeImOpenDecoder },
    { "nativeImSetMaxLens", "(II)V",
            (void*) nativeImSetMaxLens },
    { "nativeImCloseDecoder", "()Z",
            (void*) nativeImCloseDecoder },
    { "nativeImSearch",  "([BI)I",
            (void*) nativeImSearch },
    { "nativeImDelSearch",  "(IZZ)I",
            (void*) nativeImDelSearch },
    { "nativeImResetSearch",  "()V",
            (void*) nativeImResetSearch },
    { "nativeImAddLetter", "(B)I",
            (void*) nativeImAddLetter },
    { "nativeImGetPyStr", "(Z)Ljava/lang/String;",
            (void*) nativeImGetPyStr },
    { "nativeImGetPyStrLen", "(Z)I",
            (void*) nativeImGetPyStrLen },
    { "nativeImGetSplStart", "()[I",
            (void*) nativeImGetSplStart },
    { "nativeImGetChoice", "(I)Ljava/lang/String;",
            (void*) nativeImGetChoice },
    { "nativeImChoose", "(I)I",
            (void*) nativeImChoose },
    { "nativeImCancelLastChoice", "()I",
            (void*) nativeImCancelLastChoice },
    { "nativeImGetFixedLen", "()I",
            (void*) nativeImGetFixedLen },
    { "nativeImGetPredictsNum", "(Ljava/lang/String;)I",
            (void*) nativeImGetPredictsNum },
    { "nativeImGetPredictItem", "(I)Ljava/lang/String;",
            (void*) nativeImGetPredictItem },
    { "nativeImCancelInput", "()Z",
            (void*) nativeImCancelInput },
    { "nativeImFlushCache", "()Z",
            (void*) nativeImFlushCache },
    /* <<----Functions for Pinyin-to-hanzi decoding end------------- */

    /* ------Functions for sync begin----------------------------->> */
    { "nativeSyncBegin", "([B)Z",
            (void*) nativeSyncBegin },
    { "nativeSyncFinish", "()Z",
            (void*) nativeSyncFinish },
    { "nativeSyncPutLemmas", "(Ljava/lang/String;)I",
            (void*) nativeSyncPutLemmas },
    { "nativeSyncGetLemmas", "()Ljava/lang/String;",
            (void*) nativeSyncGetLemmas },
    { "nativeSyncGetLastCount", "()I",
            (void*) nativeSyncGetLastCount },
    { "nativeSyncGetTotalCount", "()I",
            (void*) nativeSyncGetTotalCount },
    { "nativeSyncClearLastGot", "()Z",
            (void*) nativeSyncClearLastGot },
    { "nativeSyncGetCapacity", "()I",
            (void*) nativeSyncGetCapacity },
    /* <<----Functions for sync end--------------------------------- */

    { "nativeImSetDictComp", "(Z)V",
            (void*) nativeImSetDictComp },
};


/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = (*env).FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env).RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 */
static int registerNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env,
           "com/android/inputmethod/pinyin/PinyinDecoder",
            gMethods, sizeof(gMethods) / sizeof(gMethods[0])))
        return JNI_FALSE;

    return JNI_TRUE;
}

/*
 * Set some test stuff up.
 *
 * Returns the JNI version on success, -1 on failure.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if ((*vm).GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        goto bail;
    }
    assert(env != NULL);

    if (!registerNatives(env)) {
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}

#ifdef __cplusplus
}
#endif
