// debug_util.h: interface for the debug_util class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUG_UTIL_H__C8208F1D_FD87_4B97_B173_6FBD22238671__INCLUDED_)
#define AFX_DEBUG_UTIL_H__C8208F1D_FD87_4B97_B173_6FBD22238671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LOG_KIKA_TAG          "Kika-ENGINE"

#if defined(__FOR_ANDROID)

 #include <android/log.h> 
 #define KIKA_LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_KIKA_TAG, __VA_ARGS__)
 #define KIKA_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , LOG_KIKA_TAG, __VA_ARGS__)
 #define KIKA_LOGI(...) __android_log_print(ANDROID_LOG_INFO   , LOG_KIKA_TAG, __VA_ARGS__)
 #define KIKA_LOGW(...) __android_log_print(ANDROID_LOG_WARN   , LOG_KIKA_TAG, __VA_ARGS__)
 #define KIKA_LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_KIKA_TAG, __VA_ARGS__) 

#elif !defined(__TEST) && defined(__DEBUG_LOG)

 #include <android/log.h>
 #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)

#endif

void d_pintf(const char * msg);
void d_pintf_ex(const char * msg);

void dw_pintf(const wchar_t * msg);
void dw_pintf_ex(const wchar_t * msg);


#endif // !defined(AFX_DEBUG_UTIL_H__C8208F1D_FD87_4B97_B173_6FBD22238671__INCLUDED_)
