#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <cstdlib>
#include <clocale>
#include <sstream>
#include <iostream>
#include <regex>

#if defined(__FOR_ANDROID)
 #if !defined(__TEST)
  #include <sys/system_properties.h>
  #include <android/log.h>
 #endif
#endif

#if !defined(__NO_KOREAN)
#include "trans_key_korean.h"
#endif

#if !defined(__NO_KIKA_IME)
 #include "kikaime.h"
#endif


#include "maptable.h"
#include "imebase.h"
#include "phraseimp.h"
#include "cachedata.h"
#include "vcandidate.h"
#include "debug_util.h"
#include "autotest_kr.h"

#if defined(__TEST) || defined(__FOR_ANDROID) || defined(__ANDROID_APP)
 #include "vmap_imp.h"
#endif

#pragma warning(disable: 4786)
#include <vector>
#include <algorithm>
using namespace std;


#if defined(__FOR_ANDROID) || defined(__ANDROID_APP) || defined(__TEST)
 #define IQQI_VERSION_NO              "2022.0804.1"
 #define IQQI_VERSION_NO_W            L"2022.0804.1"
#else
 #define IQQI_VERSION_NO              "2022.0804.1"
 #define IQQI_VERSION_NO_W            L"2022.0804.1"
#endif

#define IQQI_VERSION_ID_KEY           L"34775295376258377466"

//#define __LOCK_MODEL_NAME

#define LOCK_MODEL_NAME               L"B35"
#define MSG_INVALID_MODEL             L"ErrMDL|"
#define MSG_INVALID_MODEL_EX          (wchar_t*)L"ErrMDL|"

//#define __DEMO_EXPIRED

#define MSG_EXPIRED_EX            (wchar_t*)L"Expired|"
#define MSG_EXPIRED               L"Expired!"
#define EXPIRED_YEAR              2019
#define EXPIRED_MONTH             9
#define EXPIRED_DATE              30


// #define __FOR_TRACE
// #define __DEBUG_MSG
// #define __DEBUG_LOG
// #define __DEBUG_MSG2
// #define __ANDROID_APP
// #define __ANDROID_MSG
// #define __DEBUG_UNMATCH
// #define __DEBUG_CANDIDATES_LIST
// #define __DEBUG_LEARN_WORD_EX
// #define __DEBUG_LEARN_WORD


//#define __DEMO_AP
//#define __TEST_USER_ONLY
//#define __TEST_USER_ADD_VOCA
//#define __TEST_USER_DEL_VOCA

//#define __CONTINUOUS_KEYIN
#define __ENABLE_WILDCARD
#define __APPLY_RULE
#define __USER_DIC
//#define __USER_DIC_SAVE
//#define __USER_DIC_SAVE_CMD
#define __PHRS_DIC
//#define __USER_DIC_CLEAR_EVERY_TIMES
//#define __PHRS_DIC_SAVE
//#define __REPORT_ACTIVATE
#define __CHECK_INVALID_LEARN_WORD
#define __AUTO_APPEND_LAST
#define __0208

#include <android/log.h>
#if defined(__DEBUG_LOG)
//#include <android/log.h>
// #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)
#endif

#if defined(__FOR_TRACE)
  #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)
#endif

#if defined(__DEBUG_LOG)
 #if !defined(KIKA_LOGD)
  #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)
 #endif
#endif


#define KIKA_LOGE(fmt, ...)  __android_log_print(ANDROID_LOG_ERROR, LOG_KIKA_TAG , "%s(L%d): " fmt, __FUNCTION__, __LINE__,  ## __VA_ARGS__)


#if defined(__REPORT_ACTIVATE)
 #include "ClientMeta.h"
 #include "HTTP_Imp_Linux.h"
 #include "Data_Agent_Linux.h"
#endif

#define EXTRA_I                   1
#define MAX_AA                    2048
#define MAX_1024                  1024

#define MAX_GROUP_COUNT           60
#define MAX_GROUP_LENGTH          6

#if defined(__IQQI_PY)
#include "iqqilib_v1.h"
#define MAX_CHI_WORD_GET_NUMBER   120
#endif // defined

#define MAX_ENG_WORD_GET_NUMBER   21

#if defined(__FOR_HUAWEI)
 #define MAX_ENG_WORD_LENGTH      45
 #define MAX_CHI_WORD_LENGTH      256
#else
 #define MAX_ENG_WORD_LENGTH      45
 #define MAX_CHI_WORD_LENGTH      1
#endif // defined

#define MAX_WORD_SYMB_LENGTH      80

#define MAX_INPUT_KEY             256

#define MAX_LEARN_WORD_LEN        MAX_ENG_WORD_LENGTH //256

#define RECORD_KEY_SZ             L"%3d"

#define KEY_STAR_1                0x2a   //'*'
#define KEY_STAR_2                0x40   //'@"
#define KEY_TOKEN                 0x7c   // |

#define KEY_EXTRA_ALLOW_CH_QUOTATION       '\''
//#define KEY_EXTRA_ALLOW_CH_MINUS           '-'
#define KEY_EXTRA_ALLOW_CH_0               '0'
#define KEY_EXTRA_ALLOW_CH_2               '2'

#if defined(__SONIM)
 #define LIMIT_USER_DB_COUNT      300
 #define LIMIT_PRIORITY_COUNT     300
#else
 #define LIMIT_USER_DB_COUNT      1000
 #define LIMIT_PRIORITY_COUNT     1000
#endif

#if !defined(IQQILIBQTSHARED_EXPORT)
#define IQQILIBQTSHARED_EXPORT
#endif

#if defined(__TEST)
CandidateCH                  gCandVoca;

wchar_t                      gDebugSz[40960] ={0};
bool                         gOutVer    =false;
bool                         gAutoTest  =false;
bool                         gKaiOS_fmt =false;
bool                         gBrackets  =true;
bool                         gDebug     =false;

bool                         gDiagnostic =false;
bool                         gDiagnostiPressureTest =false;
int                          gDiagnosticTestN =0;
int                          gDiagnosticError =0;
wstring                      gDiagnosticDebug =L"";

int                          gTmpOff    =0;
int                          gKaiOS_idx =1;
#elif defined(__DEBUG_MSG)
wchar_t                      gDebugSz[40960] ={0};
#else
wchar_t                      gDebugSz[4096] ={0};
#endif

#if defined(__DEMO_EXPIRED)
struct tm                    gTm;
#endif

#define ERR_CREATE_KEYBD_LAYOUT        -1
#define ERR_INVALID_KEYBD_LAYOUT       -2
#define ERR_LOAD_DICTIONARY            -3
#define ERR_CREATE_SESSION             -4
#define ERR_INVLID_MODEL_NAME          -5

#define ERR_KERNEL_UN_INITIAL          -101
#define ERR_NULL_STR                   -102
#define ERR_INVALID_KEYCODE            -103
#define ERR_NO_SUGGESTION              -104

#define ERR_DICT_NEW_FAIL              -201
#define ERR_DICT_ADD_FAIL              -202
#define ERR_DICT_REMOVE_FAIL           -203
#define ERR_DICT_WRITE_FAIL            -204
#define ERR_DICT_NOT_SUPPORT_1WORD     -205
#define ERR_DICT_INVALID_LAST_CH       -206
#define ERR_DICT_OUTOF_WORD_LEN        -207
#define ERR_DICT_KEY_WORD_LEN_NOT_SAME -208
#define ERR_DICT_INVALID_KEY_LEN_0     -219
#define ERR_DICT_INVALID_PARAM         -210

#define ERR_NEXT_WORD_FAIL             -308

#define ERR_PHRS_LOAD_FAIL             -401
#define ERR_PHRS_ADD_FAIL              -402
#define ERR_PHRS_REMOVE_FAIL           -403
#define ERR_PHRS_WRITE_FAIL            -404


#if defined(__TEST)

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void showErr(int retVal)
{
     if ( gOutVer ==false )
          return;

     switch ( retVal )
     {
              case ERR_CREATE_KEYBD_LAYOUT://     -1
                   printf("ERR_CREATE_KEYBD_LAYOUT" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_INVALID_KEYBD_LAYOUT://    -2
                   printf("ERR_INVALID_KEYBD_LAYOUT" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_LOAD_DICTIONARY://         -3
                   printf("ERR_LOAD_DICTIONARY" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_CREATE_SESSION://          -4
                   printf("ERR_CREATE_SESSION" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_KERNEL_UN_INITIAL://       -101
                   printf("ERR_KERNEL_UN_INITIAL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_NULL_STR:
                   printf("ERR_NULL_STR" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_INVALID_KEYCODE:
                   printf("ERR_INVALID_KEYCODE" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_NO_SUGGESTION: //          -102
                   printf("ERR_NO_SUGGESTION RED" "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_DICT_ADD_FAIL://           -103
                   printf("ERR_DICT_ADD_FAIL RED" "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_DICT_REMOVE_FAIL://        -104
                   printf("ERR_DICT_REMOVE_FAIL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_DICT_WRITE_FAIL://         -105
                   printf("ERR_DICT_WRITE_FAIL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_NEXT_WORD_FAIL://          -106
                   printf("ERR_NEXT_WORD_FAIL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_PHRS_LOAD_FAIL://          -109
                   printf("ERR_PHRS_LOAD_FAIL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_PHRS_ADD_FAIL://           -110
                   printf("ERR_PHRS_ADD_FAIL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_PHRS_REMOVE_FAIL://        -111
                   printf("ERR_PHRS_REMOVE_FAIL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_PHRS_WRITE_FAIL://         -112
                   printf("ERR_PHRS_WRITE_FAIL" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_DICT_OUTOF_WORD_LEN://  -208
                   printf("ERR_DICT_OUTOF_WORD_LEN" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_DICT_KEY_WORD_LEN_NOT_SAME://  -208
                   printf("ERR_DICT_KEY_WORD_LEN_NOT_SAME" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
              case ERR_DICT_INVALID_KEY_LEN_0://      -208
                   printf("ERR_DICT_INVALID_KEY_LEN_0" RED "(%d)" RESET "\r\n\r\n", retVal);
                   break;
     }
}

void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
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

#endif

class keyLenMap :public phraseimp
{
public :
      keyLenMap()
      {
           append(IQQI_IME_Chinese_CN,     L"6",  L"8", 0);
           //append(IQQI_IME_Bulgarian,      L"12", L"12", 0);
           ///append(IQQI_IME_Macedonian,     L"20", L"20", 0); /// peijei
           //append(IQQI_IME_Slovak,         L"12", L"12", 0);
           //append(IQQI_IME_Slovenian,      L"12", L"12", 0);
           //append(IQQI_IME_Ukrainian,      L"12", L"12", 0);
           //append(IQQI_IME_Xhosa,          L"20", L"20", 0);
           ///append(IQQI_IME_Albanian,       L"12", L"12", 0); /// peijei
           //append(IQQI_IME_Armenian,       L"12", L"12", 0);
           //append(IQQI_IME_Azerbaijani,    L"12", L"12", 0);
           ///append(IQQI_IME_Belarusian,     L"20", L"20", 0); /// peijei
           //append(IQQI_IME_Bosnian,        L"12", L"12", 0);
           append(IQQI_IME_Changjie,       L"4", L"6", 0);
           append(IQQI_IME_Chinese_HK,     L"52", L"1", 0);
           append(IQQI_IME_Chinese_TW,     L"4",  L"6", 0);
           //append(IQQI_IME_Estonian,       L"12", L"12", 0);
           //append(IQQI_IME_Georgian,       L"12", L"12", 0);
           //append(IQQI_IME_Icelandic,      L"12", L"12", 0);
           //append(IQQI_IME_Latvian,        L"15", L"15", 0);
           //append(IQQI_IME_Uzbek_Cyrillic, L"18", L"18", 0);
           #if !defined(__FOR_HUAWEI)
           append(IQQI_IME_Thai,             L"4",  L"4",  4);
           append(IQQI_IME_Vietnamese,       L"4",  L"4",  4);
           #endif
           #if !defined(__NO_KOREAN)
           append(IQQI_IME_Korean,           L"4",  L"4",  4);
           #endif
      }
};

#if !defined(__NO_KIKA_IME)

class KikaBase
{
private:
      int                         mCandidateCount;
      proximity_info_config       mKeyboard;
      ProximityInfo*              mProx;
      Dictionary*                 mDict;
      DicTraverseSession*         mSession;
      Suggestions*                mSuggestions;
public:
      vector<VCandidate>          mVc;

public:
      KikaBase()
      {
          mProx        =0;
          mDict        =0;
          mSession     =0;
          mSuggestions =0;

          mCandidateCount =0;
      }
      ~KikaBase()
      {
           Free();
      }

      int  GetCandidateCount()
      {
           return mCandidateCount;
      }

      void Free();

      bool Initial(int _IME, proximity_info_config* config);
      int  Initial(int _IME, const char* DictPath, char* IME_ErrorList, bool isUser =false);

      bool WordExist( int _IME, const wchar_t* Content);

      int  LearnWord_utf8( int _IME, const char* KeyCode, const char* Content);
      int  LearnWord( int _IME, const wchar_t* KeyCode, const wchar_t* Content);
      int  DeleLearnedWord( int _IME, wchar_t* KeyCode, wchar_t* Content);

      int  GetCandidates(int _IME, wchar_t* KeyCode, vector<VCandidate> &v, int vtype);
      int  GetNextWordCandidates(int _IME, wchar_t* KeyCode, vector<VCandidate> &v);
};

#endif

struct PACKAGES_UI
{
     int           _IME;
     MEM_INT32     orderID;
     wstring       wsKeys;
     wstring       wsPhrs;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#if !defined(__NO_KIKA_IME)
KikaBase                     gMain;
KikaBase                     gUser;
#endif

imebase                      gPhrs;
#if !defined(__NO_KOREAN)
trans_key_korean             gKoreanX;
#endif

///vector<string>               gUserData;
imebase                      gImeX;
imebase                      gImeX_Prio;
imebase                      gImeX_User;
vector<PACKAGES_UI>          gImeX_Packages;
keyLenMap                    gLenX;
cachedata                    gCache;
string                       gMainDBPath;
string                       gPrioDBPath;
string                       gUserDBPath;
string                       gPhrsDBPath;
string                       gUserDBFolder;
wstring                      gKeyLast;          // last input key
wstring                      gLastSelectedBuf;  // last candidate send to editor
wstring                      gKeyComp;          // last composing key
wstring                      gKeyCompBuf;       // last composing buffer
int                          gKeyLastLen;

KEYBD_MODE                   gKeybdMode =for_T9;
int                          gCandidateCount =0;
int                          gReportActivate =1;
int                          g4LenPriority   =0;
#if defined(__FOR_PUNKT) || defined(__FOR_HUAWEI)
int                          gFullMatchKey   =1;
#else
int                          gFullMatchKey   =0;
#endif
int                          gWeight =1000;

wstring                      gWsModel;

#if defined(__REPORT_ACTIVATE)
CClientMeta                  gPack;
#endif

maptable                     gMap;



class userData_imp :public phraseimp
{
private:
      string                 sUserWordFname;
      imebase                imeX;
      int                    imeID;
      map<wstring, wstring>  KeyForImeContentX;

public:
      userData_imp()
      {
          imeID =-1;
      }

      void clearKeys()
      {
           KeyForImeContentX.clear();
           KeyForImeContentX.erase(KeyForImeContentX.begin(), KeyForImeContentX.end());
      }

      bool delKeys(int _IME, wstring wContent);
      bool findKeys(int _IME, wstring wContent, wchar_t* wsRetKey, int size);
      bool append_keys(int sID, const wchar_t *wsTag, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 iNext =-1);

      void resetAndSave();
      void updateAndSave(bool save_u);

      int  getImeID() { return imeID; }
      void setImeID(int _imeID)
      {
           imeID =_imeID;
           #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
           KIKA_LOGD("Kika-ENGINE, -----------------Set IME =%d---------------------------\r\n", imeID);
           #endif
      }

      string get_fname() { return sUserWordFname; }
      void set_fname(string fname) { sUserWordFname =fname; }

      bool isExist_Pri(int imeID, const wchar_t *wsKey, const wchar_t *wsPhrs);

      void getAll_userData(int imeID, vector<PHRS_UI> &All);

      bool isExist_UserData(int imeID, const wchar_t *wsContent);

      void reset_UserData(int _IME, imebase ImeX, const char *sFile);

      void initial_UserData(imebase ImeX, const char *sFile, int imeID, int limitN);
      void initial_UserData_load(imebase ImeX, const char *sFile, int imeID, int limitN);

      #if !defined(__NO_KIKA_IME)
      int  initial_KikaData(Dictionary* mDict);
      #endif

      #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
      bool remove_ex(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs);
      bool append_ex(int sID, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 iNext =-1);
      #endif

      int  check_UserDB_Limit();

      int  check_UserDB_Limit(int LimitCount);
};

userData_imp                 gUserFile;
userData_imp                 gPrioFile;

#if defined(__TEST) || defined(__FOR_ANDROID) || defined(__ANDROID_APP)
vmap_imp                     gVoca_InnerX;
#endif

#if defined(__TEST)
vector<VCandidate>           vdict;
vector<VCandidate>           vdict_type;
#endif

//-----------------------------------------------------------------------------
enum OPTION_ID
{
     opt_KeybdMode      =0x1000,
     opt_ReportActivate =0x1001,
     opt_FullMatchKey   =0x1002,
     opt_4LenPriority   =0x1003
};


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
#if defined(__TEST)
void KIKA_LOGD(const char * msg)
{
}
#endif

void d_pintf(const char * msg)
{
     #if defined(__DEBUG_MSG)
     std::string ss=msg;
     std::wstring ws(ss.begin(), ss.end());
     wcscat(gDebugSz, ws.c_str());
     printf("%s", msg);
     #endif

     #if defined(__DEBUG_LOG)
      #if defined(__FOR_ANDROID) || defined(__ANDROID_OS)
     KIKA_LOGD("Kika-ENGINE, %s", msg);
      #else
     KIKA_LOGD(msg);
      #endif
     #endif
}

void d_pintf_ex(const char * msg)
{
     cout <<msg;

     #if defined(__DEBUG_LOG)
      #if defined(__FOR_ANDROID) || defined(__ANDROID_OS)
     KIKA_LOGD("Kika-ENGINE, %s", msg);
      #else
     KIKA_LOGD(msg);
      #endif
     #endif
}

void dw_pintf(const wchar_t * msg)
{
     #if defined(__DEBUG_MSG)
     wcscat(gDebugSz, msg);
     printf("%ls", msg);
     #endif

     #if defined(__DEBUG_LOG)
     string
     svoca =wchar_to_UTF8(msg);

      #if defined(__FOR_ANDROID) || defined(__ANDROID_OS)
     KIKA_LOGD("Kika-ENGINE, %s", svoca.c_str());
      #else
     KIKA_LOGD(svoca.c_str());
      #endif
     #endif
}

void dw_pintf_ex(const wchar_t * msg)
{
     wcout <<msg;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void report(bool always =false)
{
     #if defined(__REPORT_ACTIVATE) && !defined(__TEST_REPORT)

     if ( gReportActivate ==0 )
     {
          return;
     }

     CData_Agent_Linux
     agent;
     agent.init();

     if ( gPack.data_pack(&agent.dui) ==0 )
     {
          gPack.http.onPostThread(agent.dui.d_duid.getMem(), gPack.mta.getByte(), gPack.mta.getLen(), always);

     }

     #endif
}



//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
#if !defined(__NO_KIKA_IME)

#if defined(__HINDI_BASE)

#include "hindi_lang01_imp.h"

extern hindi_lang01_imp      m_Hindi_01;

#endif

bool KikaBase::Initial(int _IME, proximity_info_config* config)
{
     config->keyboard_width  =1600;
     config->keyboard_height =1030;
     config->grid_width      =32;
     config->grid_height     =16;

     gMap.setKbMode(gKeybdMode);

     key*
     bKeys =0;

     int
     total =0;

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, KikaBase::Initial, IME =%d", _IME);
     #endif

     #if defined(__HINDI_BASE)
     if ( _IME ==IQQI_IME_Hindi_01 )
     {
          total =m_Hindi_01.getLayoutForKika(0, 0, 100, 20, 16, 8, bKeys);

          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          for ( int i =0; i <total; i++ )
          {
                KIKA_LOGD("Kika-ENGINE, KikaBase::Initial, getLayoutForKika (%d) =%x", i, bKeys[i].code);
          }

          KIKA_LOGD("Kika-ENGINE, KikaBase::Initial, getLayoutForKika total =%d", total);
          #endif
     }
     else
     #endif
     {
          bKeys =gMap.GetLayout(_IME);
          total =gMap.GetLayoutTotalBtn();
     }
     config->keys  =bKeys;
     config->nkeys =total;

     if ( total ==0 )
     {
          #if defined(__DEBUG_MSG) || defined(__DEBUG_LOG)
          char aa[MAX_AA] ={0};
          sprintf(aa, "IME =%d, bKeys total ==0\r\n", _IME);
          d_pintf(aa);
          #endif
          return false;
     }

     #if defined(__DEBUG_MSG)
     wchar_t aa[2000] ={0};
     //swprintf(aa, 2000, L"Kika-ENGINE, KikaBase::Initial, _IME =%d, bkeys =%x, total =%d, 0=%d,25=%d\r\n", _IME, bKeys, total, bKeys[0].x, bKeys[25].x);
     swprintf(aa, 2000, L"_IME =%d, bkeys =%x, total =%d\r\n", _IME, bKeys, total);
     dw_pintf(aa);
     #endif

     #if defined(__DEMO_EXPIRED)
     time_t
     t = time(NULL);

     gTm =*localtime(&t);
     #endif

     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void KikaBase::Free()
{
    if ( mDict !=0 )
    {
         dictionary_close(mDict);
         mDict =0;
    }
    if ( mSession !=0 )
    {
         session_close(mSession);
         mSession =0;
    }
    if ( mProx !=0 )
    {
         proximity_info_destroy(mProx);
         mProx =0;
    }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  KikaBase::Initial(int _IME, const char* DictPath, char* IME_ErrorList, bool isUser)
{
     setlocale( LC_ALL, "");

     Free();

     if ( Initial(_IME, &mKeyboard) ==false )
     {
          return ERR_CREATE_KEYBD_LAYOUT;   //-1
     }

     if ( DictPath ==0 || strlen(DictPath) ==0 )
     {
          #if defined(__USER_DIC_SAVE)
          return ERR_NULL_STR;
          #else

          if ( isUser ==false )
          {
               return ERR_NULL_STR;
          }
          #endif
     }

     mProx =proximity_info_create(&mKeyboard);
     if ( mProx ==0 )
     {
          return ERR_INVALID_KEYBD_LAYOUT;  //-2
     }

     if ( isUser ==true )
     {
          #if defined(__USER_DIC_SAVE)

          struct stat
          myStat;

          bool
          exist =(stat(gUserDBFolder.c_str(), &myStat) == 0) && (((myStat.st_mode) & S_IFMT) == S_IFDIR);

          if ( exist ==false )
          {
               char aa[MAX_AA] ={0};
               sprintf(aa, "%s not exist\r\n", gUserDBFolder.c_str());
               d_pintf(aa);

               mDict =0;
               return ERR_DICT_WRITE_FAIL;
          }

          exist =(stat(DictPath, &myStat) == 0) && (((myStat.st_mode) & S_IFMT) == S_IFDIR);

          if ( exist ==true )
          {
               mDict =dictionary_create(DictPath);
          }
          else
          {
               mDict =dictionary_new();

               try
               {
                    if ( dictionary_write(mDict, DictPath) ==false )
                    {
                         return ERR_DICT_WRITE_FAIL;
                    }
               }
               catch (std::exception const& e)
               {
                    return ERR_DICT_WRITE_FAIL;
               }
               catch (...)
               {
                    return ERR_DICT_WRITE_FAIL;
               }
          }

          if ( mDict ==0 )
          {
               return ERR_DICT_NEW_FAIL; //-3
          }

          #else

          mDict =dictionary_new();

          if ( mDict ==0 )
          {
               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
               KIKA_LOGD("Kika-ENGINE, KikaBase::Initial, user dict fail %s", DictPath);
               #endif
               return ERR_DICT_NEW_FAIL; //-3
          }

          gUserFile.initial_KikaData(mDict);

          #endif
     }
     else
     {
          mDict =dictionary_create(DictPath);
          if ( mDict ==0 )
          {
               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
               KIKA_LOGD("Kika-ENGINE, KikaBase::Initial, main dict fail, %s", DictPath);
               #endif
               return ERR_LOAD_DICTIONARY; //-3
          }
     }

     mSession =session_create(mDict);
     if ( mSession ==0 )
     {
          return ERR_CREATE_SESSION; // -4
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
bool KikaBase::WordExist( int _IME, const wchar_t* wsContent)
{
     if ( mDict ==0 )
          return false;

     string
     sUTF8 =wchar_to_UTF8(wsContent);

     bool exist =dictionary_exist(mDict, sUTF8.c_str(), 0);

     return exist;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  KikaBase::LearnWord( int _IME, const wchar_t* KeyCode, const wchar_t* Content)
{
     if ( mProx ==0 || mDict ==0 || mSession ==0 )
     {
          return ERR_KERNEL_UN_INITIAL;
     }
     if ( Content ==0 || wcslen(Content) ==0 )
     {
          return ERR_NULL_STR;
     }
     if ( wcslen(Content) ==1 )
     {
          return ERR_DICT_NOT_SUPPORT_1WORD;
     }

     if ( wcslen(KeyCode) >MAX_LEARN_WORD_LEN )
          return ERR_DICT_OUTOF_WORD_LEN;

     #if defined(__USER_DIC_SAVE)

     struct stat
     myStat;

     bool
     exist =(stat(gUserDBFolder.c_str(), &myStat) == 0) && (((myStat.st_mode) & S_IFMT) == S_IFDIR);


     if ( exist ==false )
     {
          mDict =0;
          return ERR_DICT_WRITE_FAIL;
     }

     try
     {
          if ( dictionary_write(mDict, gUserDBPath.c_str()) ==false )
          {
               return ERR_DICT_WRITE_FAIL;
          }
     }
     catch (std::exception const& e)
     {
          return ERR_DICT_WRITE_FAIL;
     }
     catch (...)
     {
          return ERR_DICT_WRITE_FAIL;
     }

     #else

     // nothing have to do

     #endif

     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  KikaBase::DeleLearnedWord( int _IME, wchar_t* KeyCode, wchar_t* Content)
{
     if ( mProx ==0 || mDict ==0 || mSession ==0 )
     {
          return ERR_KERNEL_UN_INITIAL;
     }

     string
     svoca =wchar_to_UTF8(Content);

     if ( dictionary_remove(mDict, svoca.c_str()) ==false )
     {
          return ERR_DICT_REMOVE_FAIL;
     }

     #if defined(__USER_DIC_SAVE)

     struct stat
     myStat;

     bool
     exist =(stat(gUserDBFolder.c_str(), &myStat) == 0) && (((myStat.st_mode) & S_IFMT) == S_IFDIR);

     if ( exist ==false )
     {
          mDict =0;
          return ERR_DICT_WRITE_FAIL;
     }

     try
     {
          if ( dictionary_write(mDict, gUserDBPath.c_str()) ==false )
          {
               return ERR_DICT_WRITE_FAIL;
          }
     }
     catch (std::exception const& e)
     {
          return ERR_DICT_WRITE_FAIL;
     }
     catch (...)
     {
          return ERR_DICT_WRITE_FAIL;
     }
     #else

     // Somthing wrong
     ///std::vector<string>::iterator
     ///itr =std::find( gUserData.begin(), gUserData.end(), svoca );

     ///if ( itr !=gUserData.end() )
     ///{
     ///     gUserData.erase(itr);
     ///}

     #endif

     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  KikaBase::GetCandidates(int _IME, wchar_t* KeyCode, vector<VCandidate> &v, int vtype = VTYPE_MAIN)
{
     #if defined(__FOR_TRACE)
     KIKA_LOGD("Kika-ENGINE, GetCandidates, _IME =%d, KeyCode =%ls, vtype =%d", _IME, KeyCode, vtype);
     #endif

     try
     {
          gDebugSz[0] =0;

          int
          wsLen =wcslen(KeyCode);

          mCandidateCount =0;

          if ( wsLen >MAX_ENG_WORD_LENGTH )
          {
               return mCandidateCount =0;
          }

          //------------------------------------------------------------------------
          // 前1個字自行枚舉所有可能的candidate
          //------------------------------------------------------------------------
          #if defined(__HINDI_BASE)
          if ( gKeybdMode ==for_T9 && wsLen ==1 && _IME !=IQQI_IME_Hindi_01 )
          #else
          if ( gKeybdMode ==for_T9 && wsLen ==1 )
          #endif
          {
               wchar_t
               keyBuffer[1024] ={0};

               int
               Ch =(KeyCode[0] ==KEY_STAR_1) ? 10 :KeyCode[0] -0x30;

               vector<wstring>
               vSz;
               gMap.MultiType_ToBufferV(_IME, Ch, vSz);

               for ( mCandidateCount =0; mCandidateCount <vSz.size(); mCandidateCount++ )
               {
                     v.push_back(VCandidate(vSz[mCandidateCount], mCandidateCount +EXTRA_I, vtype));
               }

//               gMap.MultiType_ToBuffer(_IME, Ch, keyBuffer);
//
//               wsLen =wcslen(keyBuffer);
//
//               for ( mCandidateCount =0; mCandidateCount <wsLen; mCandidateCount++ )
//               {
//                     wchar_t ws[2] ={0, 0};
//                     ws[0] =keyBuffer[mCandidateCount];
//
//                     //wcscpy(Candidates[mCandidateCount], ws);
//                     v.push_back(VCandidate(ws, mCandidateCount +EXTRA_I, vtype));
//               }

               d_pintf("\r\n");

               return mCandidateCount;
          }

          if ( mProx ==0 || mDict ==0 || mSession ==0 )
          {
               return ERR_KERNEL_UN_INITIAL;
          }
          if ( KeyCode ==0 || wcslen(KeyCode) ==0 )
          {
               return ERR_NULL_STR;
          }
          //------------------------------------------------------------------------
          point*
          pt =new point[wsLen];

          #if defined(__DEBUG_MSG)
           #if defined(__TEST)
          if ( gOutVer ==true )
           #endif
          {
               if ( gKeybdMode ==for_T9 )
               {
                    dw_pintf((wchar_t*)L"is T9:\r\n");
               }
               else
               {
                    dw_pintf((wchar_t*)L"is Qwerty:\r\n");
               }
          }
          #endif

          //------------------------------------------------------------------------
          int jlen =0;

          if ( pt != nullptr )
          {
               for ( int chx =0; chx <wsLen; chx++ )
               {
                     int
                     cCode =KeyCode[chx];

                     bool ok =false;

                     #if defined(__HINDI_BASE)
                     if ( _IME ==IQQI_IME_Hindi_01 )
                     {
                          cCode =KeyCode[chx];

                          #if defined(__DEBUG_MSG)
                          KIKA_LOGD("Kika-ENGINE, Layout GetXY: KeyCode[0] =%x\r\n", KeyCode[chx]);
                          KIKA_LOGD("Kika-ENGINE, Layout GetXY: KeyCode[1] =%x\r\n", KeyCode[chx +1]);
                          #endif

                          if ( KeyCode[chx +1] !=0 )
                          {
                               cCode =KeyCode[chx +1] | KeyCode[chx]<<16;

                               ok =m_Hindi_01.keyCode2HitXY(cCode, pt[jlenj].x, pt[jlen].y);

                               if ( ok ==true )
                               {
                                    chx++;
                                    pt[jlen].t =50 *(jlen);
                                    jlen++;
                                    continue;
                               }
                          }

                          #if defined(__DEBUG_MSG)
                          KIKA_LOGD("Kika-ENGINE, Layout GetXY: cCode =%x\r\n", cCode);
                          #endif

                          ok =m_Hindi_01.keyCode2HitXY(cCode, pt[jlen].x, pt[jlen].y);
                          if ( ok ==true )
                          {
                               pt[jlen].t =50 *(jlen);
                               jlen++;
                          }
                     }
                     else
                     #endif
                     {
                          ok =gMap.KeyCode2HitXY(_IME, cCode, pt[jlen].x, pt[jlen].y);
                     }

                     #if defined(__DEBUG_MSG)
                      #if defined(__FOR_ANDROID)
                      KIKA_LOGD("Kika-ENGINE, Layout Get: ok = %d, pt[j].x=%d, pt[jlen].y=%d", ok, pt[jlen].x, pt[jlen].y);
                      #else
                      {
                      char aa[MAX_AA] ={0};
                      sprintf(aa, "Kika-ENGINE, Layout Get: ok = %d, pt[jlen].x=%d, pt[jlen].y=%d", ok, pt[jlen].x, pt[jlen].y);
                      d_pintf(aa);
                      }
                      #endif
                     #endif

                     if ( ok ==true )
                     {
                          #if defined(__DEBUG_MSG)
                          wchar_t aa[MAX_AA];
                          swprintf(aa, 120, L"      final hit =0x%x, x =%d, y =%d\r\n", cCode, pt[jlen].x, pt[jlen].y);
                          dw_pintf(aa);
                          #endif

                          pt[jlen].t =50 *(jlen);
                          jlen++;
                     }
                     else
                     {
                          #if defined(__DEBUG_MSG)
                          wchar_t aa[MAX_AA];
                          swprintf(aa, 1024, L"      final hit =0x%x, no match\r\n", cCode);
                          dw_pintf(aa);
                          #endif
                          return ERR_INVALID_KEYCODE;
                     }

                     #if defined(__DEBUG_MSG2)
                     //-------------------------------------------------------------
                     if ( chx ==0 )
                     {
                          for ( int i =0; i <26; i++ )
                          {
                                wchar_t aa[MAX_AA];
                                swprintf(aa, 2048, L"%2d, {%4d, %3d, %3d, %3d, 0x%x},\r\n", i,
                                         gMap.GetLayout()[i].x,     gMap.GetLayout()[i].y,
                                         gMap.GetLayout()[i].width, gMap.GetLayout()[i].height,
                                         gMap.GetLayout()[i].code);
                                dw_pintf(aa);
                          }
                     }
                     //-------------------------------------------------------------
                     #endif
               }
          }

          #if defined(__DEBUG_MSG)
          {
                for ( int chx =0; chx <wsLen; chx++ )
                {
                      wchar_t aa[MAX_AA] ={0};
                      swprintf(aa, 120, L"final, x =%4d, y =%4d, t =%d\r\n", pt[chx].x, pt[chx].y, pt[chx].t);
                      dw_pintf(aa);
                }
          }
          #endif

          //mSuggestions =(j ==0? 0: dictionary_suggest(mSession, mDict, mProx, nullptr, pt, j));
          mSuggestions =(jlen ==0? 0: dictionary_suggest(mSession, mDict, mProx, nullptr, pt, jlen, gWeight));

          delete[] pt;

          if ( mSuggestions == nullptr )
          {
               #if defined(__DEBUG_MSG)
               wchar_t aa[MAX_AA];
               swprintf(aa, 1024, L"gSuggestions =0\r\n");
               //dw_pintf(aa);
               #endif
               return ERR_NO_SUGGESTION;
          }

          mCandidateCount =suggestion_results_get_count(mSuggestions);

          #if defined(__DEBUG_MSG)
          wchar_t aa[MAX_AA];
          swprintf(aa, 1024, L"mCandidateCount =%d\r\n", mCandidateCount);
          dw_pintf(aa);
          #endif

          //------------------------------------------------------------------------
          for ( int i =0; i <mCandidateCount; i++ )
          {
                const char*
                pUTF8 =suggestion_results_get(mSuggestions, i);

                wstring
                wss =utf8_to_utf16(pUTF8);

                #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                KIKA_LOGD("Kika-ENGINE, kika_GetCandidate =%ls\r\n", wss.c_str());
                #endif

                #if defined(__DEBUG_MSG)
                unsigned int
                *xx =(unsigned int*)wss.c_str();

                #if defined(__FOR_ANDROID)
                 KIKA_LOGD("Kika-ENGINE, Layout GetCandidates: (%d) =%x,%x,%x,%x (%x,%x,%x,%x)\r\n", i, pUTF8[0], pUTF8[1], pUTF8[2], pUTF8[3], xx[0], xx[1], xx[2], xx[3]);
                #else
                 {
                      char aa[MAX_AA] ={0};
                      sprintf(aa, "Kika-ENGINE, Layout GetCandidates: (%d) =%x,%x,%x,%x (%x,%x,%x,%x)\r\n", i, pUTF8[0], pUTF8[1], pUTF8[2], pUTF8[3], xx[0], xx[1], xx[2], xx[3]);
                      d_pintf(aa);
                 }
                #endif
                #endif

                VCandidate
                vx(wss.c_str(), v.size() +i);

                if ( std::find(v.begin(), v.end(), vx) == v.end() )
                {
                     v.push_back(VCandidate(wss.c_str(), v.size() +i +EXTRA_I, vtype));
                }
          }

          // free candidate mem
          suggestion_results_free(mSuggestions);

          #if defined(__APPLY_RULE)
          vcandidate_applyRule(v);
          #endif

          return mCandidateCount;
     }
     catch ( ... )
     {
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
int  KikaBase::GetNextWordCandidates( int _IME, wchar_t* KeyCode, vector<VCandidate> &v)
{
     if ( mProx ==0 || mDict ==0 || mSession ==0 )
     {
          return ERR_KERNEL_UN_INITIAL;
     }

     if ( KeyCode ==0 || wcslen(KeyCode) ==0 )
     {
          return ERR_NULL_STR;
     }
     gDebugSz[0] =0;

     mCandidateCount =0;

     string
     svoca =wchar_to_UTF8(KeyCode);

     const char* prev_words[1024] ={0};
     const char *ps =svoca.c_str();

     std::wstring temp;
     std::vector<std::wstring> parts;
     std::vector<std::string>  pss;
     std::wstringstream wss(KeyCode);

     while ( std::getline(wss, temp, L' ') )
     {
             parts.push_back(temp);
             wchar_t *wx =(wchar_t *)parts[parts.size() -1].c_str();

             pss.push_back(wchar_to_UTF8(wx));
     }

     if ( pss.size() ==0 )
     {
          pss.push_back(svoca);
     }

     vector<string>::iterator
     ii;

     int word_i =0;
     for ( ii = pss.end() -1; ii >=pss.begin(); ii--, word_i++ )
     {
           prev_words[word_i] =ii->c_str();

           #if defined(__DEBUG_MSG)
           {
           char aa[MAX_AA] ={0};
           sprintf(aa, "next word =%s, l=%d\r\n", prev_words[word_i], (int)strlen(prev_words[word_i]));
           d_pintf(aa);
           }
           #endif
     }

     context
     test;
     test.prev_words  =prev_words;
     test.begin_marks =nullptr;
     test.nwords      =word_i;

     #if defined(__DEBUG_MSG)
     {
     char aa[MAX_AA] ={0};
     sprintf(aa, "next word, word_i=%d\r\n", word_i);
     d_pintf(aa);
     }
     #endif

     //------------------------------------------------------------------------
     try
     {
          mSuggestions = dictionary_predict(mDict, &test);
     }
     catch ( ... )
     {

     }

     if ( mSuggestions ==0 )
     {
          return ERR_NEXT_WORD_FAIL;
     }

     mCandidateCount =suggestion_results_get_count(mSuggestions);

     #if defined(__DEBUG_MSG)
     {
     char aa[MAX_AA] ={0};
     sprintf(aa, "\r\npredict (Next word) count =%d\r\n\r\n", mCandidateCount);
     d_pintf(aa);
     }
     #endif

     if ( mCandidateCount >0 )
     {
          //setlocale( LC_ALL, "");

          //-------------------------------------------------------------------
          for ( int i =0; i <mCandidateCount; i++ )
          {
                const char*
                pVocaUTF8 =suggestion_results_get(mSuggestions, i);

                wstring
                wss =utf8_to_utf16(pVocaUTF8);

                v.push_back(VCandidate(wss.c_str(), i));
          }
     }

     // free candidate mem
     suggestion_results_free(mSuggestions);

     return mCandidateCount;
}

#endif








//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_WordExist( int _IME, const wchar_t* wsContent)
{
     return gMain.WordExist(_IME, wsContent);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_Free()
{
    #if !defined(__NO_KIKA_IME)

    gMain.Free();

    gImeX.clear();
    gImeX->clear();
    gImeX->clear_u();

    gUserFile.clearKeys();
    gUserFile.clear();
    gImeX_User.clear_u();
    gImeX_User->clear_u();

    gPrioFile.clear();
    gPrioFile.clear_u();
    gImeX_Prio->clear_u();

    #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
    KIKA_LOGD("Kika-ENGINE, IQQI_Free");
    #endif

    #if defined(__IQQI_PY)
    IQQI_Free_Py();
    #endif

    #endif

    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_FreeKika()
{
    #if !defined(__NO_KIKA_IME)

    gMain.Free();

    #if defined(__USER_DIC)
    gUser.Free();
    #endif

    #endif

    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
inline int wtoi(const wchar_t *str)
{
     return (int)wcstol(str, 0, 10);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
bool setT9(int _IME)
{
     if ( gKeybdMode ==for_T9 || _IME ==IQQI_IME_Korean )
     {
          MEM_INT32
          i3;

          wstring
          v1, v2;

          if ( gLenX.getData(_IME, v1, v2, i3) ==_IME )
          {
               if ( v1.empty() || v2.empty() )
               {
                    return false;
               }
               gImeX->setMaxKeyLen(wtoi(v1.c_str()));
               gImeX->setMaxPhrLen(wtoi(v2.c_str()));
               gImeX->setMaxNwdLen(sizeof(MEM_INT32));

               return true;
          }
     }
     //2021/01/05
     switch ( _IME )
     {
              //中文都是
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_TW:
              case IQQI_IME_Chinese_HK:
                   #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                   KIKA_LOGD("Kika-ENGINE, Chinese IME Always set To T9");
                   #endif
                   return true;
              default:
                   break;
     }
     return false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_Initial(int _IME, char* PredicDBPath, char* UserDBPath, char* PhrsDBPath, char* IME_ErrorList)
{
     #if defined(__DEMO_EXPIRED)
     time_t
     t = time(NULL);

     gTm =*localtime(&t);
     #endif

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, initial_UserData_load oImeID =%d, new IME =%d", gUserFile.getImeID(), _IME);
     #endif

     #if !defined(__USER_DIC_SAVE)
     int
     oImeID =gPrioFile.getImeID();

     if ( oImeID !=_IME )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, reset & clear IME =[%d]", _IME);
          #endif

          ////gUserData.clear();
          gUserFile.clearKeys();
          gUserFile.clear();
          gImeX_User.clear_u();
          gImeX_User->clear_u();

          gPrioFile.clear();
          gPrioFile.clear_u();
          gImeX_Prio->clear_u();

          //gUserFile.setImeID(_IME);
     }
     #endif

     #if !defined(__TEST) && defined(__LOCK_MODEL_NAME)

     //------------------------------------------------------------------------
     // Get Model Name
     //------------------------------------------------------------------------
     char
     gModelName[PROP_VALUE_MAX +1];

     int
     osVersionLength =__system_property_get("ro.product.model", gModelName);

     gWsModel =UTF8_to_wchar(gModelName);

     if ( osVersionLength >0 && wcscmp(gWsModel.c_str(), LOCK_MODEL_NAME) !=0 )
     {
          if ( _IME !=IQQI_IME_Chinese_TW && _IME !=IQQI_IME_Chinese_CN )
          {
               return ERR_INVLID_MODEL_NAME;
          }
     }
     #endif
     //------------------------------------------------------------------------

     IQQI_FreeKika();

     gDebugSz[0] =0;
     gKeyLast    =L"";
     gKeyLastLen =0;

     if ( PhrsDBPath !=NULL )
     {
          gPhrsDBPath =PhrsDBPath;
     }
     if ( PredicDBPath !=NULL )
     {
          gMainDBPath =PredicDBPath;
     }

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_Initial PredicDBPath =%s, UserDBPath =%s", PredicDBPath, UserDBPath);
     #endif

     if ( UserDBPath !=NULL )
     {
          gUserDBPath =UserDBPath;

          gPrioDBPath =gUserDBPath.c_str();
          gPrioDBPath +=".pri";

          #if !defined(__USER_DIC_SAVE)
          //int
          //oImeID =gPrioFile.getImeID();

          if ( oImeID !=_IME )
          {
               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
               KIKA_LOGD("Kika-ENGINE, set new IME =[%d]", _IME);
               #endif

               gPrioFile.initial_UserData_load(gImeX_Prio, gPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);

               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
               KIKA_LOGD("Kika-ENGINE, IQQI_Initial, load Prio Path =[lmt %04d]%s", LIMIT_PRIORITY_COUNT, gPrioDBPath.c_str());
               KIKA_LOGD("Kika-ENGINE, IQQI_Initial, load User Path =[lmt %04d]%s", LIMIT_USER_DB_COUNT,  gUserDBPath.c_str());
               #endif

               // Inital UserDB
               gUserFile.initial_UserData_load(gImeX_User, gUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);

               gUserFile.setImeID(_IME);
          }
          #else

          int
          idx =gUserDBPath.find_last_of("/\\");

          if ( idx >0 )
          {
               gUserDBFolder =gUserDBPath.substr(0, idx);
          }
          #endif
     }

     int
     retVal =0;

     bool
     isT9 =setT9(_IME);

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, KikaBase::Initial, IME =%d, IsT9 =%d\r\n", _IME, isT9);
     #endif
     if ( isT9 ==true  )
     {
          switch ( _IME )
          {
                   case IQQI_IME_Changjie:
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_TW:
                        gImeX->setSortID(SORT_BY_CH);
                        gImeX->setPreLoad(true);
                        gImeX->attachWildKLen(false);
                        break;

                   case IQQI_IME_Chinese_HK:
                        gImeX->attachWildKLen(true);
                        break;

                   #if !defined(__FOR_HUAWEI)
                   case IQQI_IME_Thai:
                   case IQQI_IME_Vietnamese:
                        //gImeX->setSortID(SORT_BY_EN);
                        //gImeX->setPreLoad(true);
                        //gImeX->attachWildKLen(false);
                        //break;
                   #endif

                   default:
                        gImeX->setSortID(SORT_BY_EN);
                        gImeX->setPreLoad(true);
                        gImeX->attachWildKLen(false);
                        break;
          }

          gImeX->clear();

          retVal =gImeX.loadDict(gMainDBPath.c_str()) ==true? 0 :ERR_LOAD_DICTIONARY;
          gImeX_User.attach(phrs_v1);

          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, KikaBase::loadDict, IME =%d, %s, retVal=%d\r\n", _IME, gMainDBPath.c_str(), retVal);
          #endif

          #if defined(__TEST)
          if ( gImeX.isV3() )
          {
               char aa[256] ={0};
               sprintf(aa, "T9 Dict Version 3\r\n");
               d_pintf_ex(aa);
          }
          else if ( gImeX.isV2_1() )
          {
               char aa[256] ={0};
               sprintf(aa, "T9 Dict Version 2_1\r\n");
               d_pintf_ex(aa);
          }
          else if ( gImeX.isV2() )
          {
               char aa[256] ={0};
               sprintf(aa, "T9 Dict Version 2\r\n");
               d_pintf_ex(aa);
          }
          else
          {
               char aa[256] ={0};
               sprintf(aa, "T9 Dict Version 1\r\n");
               d_pintf_ex(aa);
          }
          #endif

          #if defined(__DEBUG_MSG)
          {
          char aa[256] ={0};
          sprintf(aa, "T9 main initial retVal =%d\r\n", retVal);
          d_pintf_ex(aa);
          }
          #endif

          #if defined(__TEST)
          if ( retVal !=0 )
          {
               //if ( gMainDBPath.find(".dict") !=-1 )
               if ( gOutVer ==true )
               {
                    char aa[256] ={0};
                    sprintf(aa, "Load (%s) retVal =%d\r\n", gMainDBPath.c_str(), retVal);
                    d_pintf_ex(aa);

                    showErr(retVal);
               }
          }
          #endif

          #if defined(__DEBUG_MSG) || defined(__TEST) || defined(__DEBUG_LOG)
          {
          char aa[256] ={0};
          sprintf(aa, "T9 IME count() =%ld\r\n", gImeX->count());
          d_pintf(aa);
          }
          #endif

          return 0;
     }
     //------------------------------------------------------------------------
     #if !defined(__NO_KIKA_IME)

     #if defined(__USER_DIC)
     retVal =gUser.Initial(_IME, (char *)gUserDBPath.c_str(), IME_ErrorList, true);
     gUser.Free();
     #endif

     #if defined(__DEBUG_MSG)
     {
     char aa[256] ={0};
     sprintf(aa, "user initial retVal =%d\r\n", retVal);
     d_pintf(aa);
     }
     #endif

     //if ( retVal ==0 )
     {
          retVal =gMain.Initial(_IME, (char *)gMainDBPath.c_str(), IME_ErrorList);

          #if defined(__DEBUG_MSG)
          {
          char aa[256] ={0};
          sprintf(aa, "main initial retVal =%d\r\n", retVal);
          d_pintf(aa);
          }
          #endif

          #if defined(__TEST)
          if ( retVal !=0 )
          {
               //if ( gMainDBPath.find(".dict") !=-1 )
               {
                    char aa[256] ={0};
                    sprintf(aa, "%s retVal =%d\r\n", gMainDBPath.c_str(), retVal);
                    d_pintf_ex(aa);

                    showErr(retVal);
               }
          }
          #endif

          if ( retVal ==0 )
          {
               report();
          }
     }
     return retVal;

     #else

     return 0;

     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_GetCandidateCount(int _IME, wchar_t* KeyCode, bool FirstCandidateIsUserTyping, int Type)
{
#if defined(__IQQI_PY)
    if ( _IME ==IQQI_IME_Chinese_CN ) {
        if (gKeybdMode == for_T9)
            return IQQI_GetCandidateCount_KT9(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
        else
            return IQQI_GetCandidateCount_Py(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
    }
#endif
     return gCandidateCount;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_GetGrouping(int _IME, wchar_t* KeyCode, int Begin, int Number, wchar_t** Candidates)
{
     #if defined(__DEMO_EXPIRED)

     #if defined(__DEBUG_LOG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
     char sss[1024]={0};
     sprintf(sss, "Grp-now: %d-%d-%d\r\n", gTm.tm_year + 1900, gTm.tm_mon + 1, gTm.tm_mday);
     KIKA_LOGD(sss);
     #endif

     bool
     y0 =gTm.tm_year + 1900 >EXPIRED_YEAR,
     y1 =gTm.tm_year + 1900 ==EXPIRED_YEAR,
     m1 =gTm.tm_mon + 1 >EXPIRED_MONTH,
     m2 =gTm.tm_mon + 1 ==EXPIRED_MONTH,
     d1 =gTm.tm_mday >EXPIRED_DATE;

     #if defined(__DEBUG_LOG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
     sprintf(sss, "Grp-now: y0 =%d, y1=%d, m1=%d, m2=%d, d1=%d\r\n", y0, y1, m1, m2, d1);
     KIKA_LOGD(sss);
     #endif

     if ( y0 ==true )
     {
          gCandidateCount =0;
          wcscpy(Candidates[gCandidateCount++], MSG_EXPIRED);
          return 1;
     }
     else if ( y1 ==true )
     {
          if ( m1 ==true )
          {
               gCandidateCount =0;
               wcscpy(Candidates[gCandidateCount++], MSG_EXPIRED);
               return 1;
          }
          else if ( m2 ==true && d1 ==true )
          {
               gCandidateCount =0;
               wcscpy(Candidates[gCandidateCount++], MSG_EXPIRED);
               return 1;
          }
     }
     #endif

     #if !defined(__TEST) && defined(__LOCK_MODEL_NAME)

     if ( wcscmp(gWsModel.c_str(), LOCK_MODEL_NAME) !=0 )
     {
          gCandidateCount =0;
          wcscpy(Candidates[gCandidateCount++], MSG_INVALID_MODEL_EX);

          #if defined(__DEBUG_LOG)
          {
              char sss[1024]={0};
              sprintf(sss, "ModelName =%ls, Count =%d\r\n", gWsModel.c_str(), gCandidateCount);
              KIKA_LOGD(sss);
          }
          #endif
          return 1;
     }
     #endif

     #if defined(__DEBUG_MSG)
     time_t now = time(0);
     clock_t t1,t2;
     t1=clock();
     #endif

     vector<wstring>
     vcGrp;

     int
     mGrpCount =0;

     //if ( gKeybdMode ==for_T9  )
     {
          if ( gImeX->count() >0 )
          {
               int
               TEST_GET_TOTAL =1,
               TEST_GET_GROUP =40;

               switch ( _IME )
               {
                        case IQQI_IME_Chinese_CN:
                        case IQQI_IME_Chinese_TW:
                             mGrpCount =0;
                             {
                                  if ( wcslen(KeyCode) ==1 )
                                  {
                                       wchar_t
                                       wsKey[2048] ={0};

                                       gMap.Numb2keyCodeAll(_IME, KeyCode, wsKey, 256);

                                       for ( int i =0; i <wcslen(wsKey); i++ )
                                       {
                                             wchar_t
                                             wsGrp[2] ={0, 0};
                                             wsGrp[0] =wsKey[i];

                                             if ( _IME ==IQQI_IME_Chinese_CN )
                                             {
                                                  if ( wsKey[i] =='i' || wsKey[i] =='u' || wsKey[i] =='v' )
                                                       continue;
                                             }
                                             wcscpy(Candidates[mGrpCount++], wsGrp);
                                       }
                                  }
                                  else
                                  {
                                       #if defined(__DEBUG_MSG) //xxx
                                       {
                                       wchar_t aa[MAX_AA];
                                       swprintf(aa, 120, L"GetGrouping (%ls)...\r\n", KeyCode);
                                       dw_pintf(aa);
                                       }
                                       #endif

                                       #if defined(__DEBUG_DATA)
                                       vcGrp.push_back(L"ai");
                                       vcGrp.push_back(L"bi");
                                       vcGrp.push_back(L"ch");
                                       vcGrp.push_back(L"ci");
                                       #else
                                       gImeX->getGrouping(_IME, KeyCode, vcGrp, TEST_GET_GROUP);
                                       #endif

                                       vector<wstring>::iterator
                                       iivc =vcGrp.begin();

                                       for ( ; iivc != vcGrp.end(); iivc++ )
                                       {
                                             vector<wstring>
                                             vc;

                                             // no need to check candidate available again
                                             ///gImeX->getKeyFuzzy(_IME, KeyCode, vc, Candidates[mGrpCount], TEST_GET_TOTAL);

                                             #if defined(__DEBUG_MSG)
                                             {
                                             wchar_t aa[MAX_AA];
                                             swprintf(aa, 120, L"KeyCode =%ls, vc.size() =%d\r\n", KeyCode, (int)vc.size());
                                             dw_pintf(aa);
                                             }
                                             #endif

                                             //if ( vc.size() >0 )
                                             {
                                                  wcscpy(Candidates[mGrpCount++], iivc->c_str());
                                             }

                                             #if defined(__TEST)
                                             if ( gDiagnostic ==true )
                                             {
                                                  if ( gDiagnosticDebug.size() )
                                                       gDiagnosticDebug +=L" ";
                                                  gDiagnosticDebug +=iivc->c_str();
                                             }
                                             #endif
                                       }
                                  }
                             }
                             break;
               }
          }
     }

     #if defined(__DEBUG_MSG)
     {
     wchar_t aa[MAX_AA];
     swprintf(aa, 120, L"final Grouping Count =%d\r\n", mGrpCount);
     dw_pintf(aa);
     }
     #endif

     #if defined(__DEBUG_MSG)
     {
         t2=clock();
         float runningTime =((float)t2-(float)t1);///1000;

         wchar_t
         aa[1024] ={0};

         #if defined(_WIN32)
         swprintf(aa, L"total run ms =%f\r\n", runningTime /1000);
         #else
         swprintf(aa, 1024, L"total run ms =%f\r\n", runningTime/1000);
         #endif

         dw_pintf(aa);
     }
     #endif

     return mGrpCount;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  IQQI_GetCandidatesEx(int _IME, wchar_t* KeyCodeEx, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, bool FullMatchQwerty)
{
     #if defined(__TEST)
     vdict.clear();
     #else
     vector<VCandidate>
     vdict;
     #endif

     #if defined(__DEMO_EXPIRED)

     #if defined(__DEBUG_LOG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
     char sss[1024]={0};
     sprintf(sss, "Cdt-now: %d-%d-%d\r\n", gTm.tm_year + 1900, gTm.tm_mon + 1, gTm.tm_mday);
     KIKA_LOGD(sss);
     #endif

     bool
     y0 =gTm.tm_year + 1900 >EXPIRED_YEAR,
     y1 =gTm.tm_year + 1900 ==EXPIRED_YEAR,
     m1 =gTm.tm_mon + 1 >EXPIRED_MONTH,
     m2 =gTm.tm_mon + 1 ==EXPIRED_MONTH,
     d1 =gTm.tm_mday >EXPIRED_DATE;

     #if defined(__DEBUG_LOG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
     sprintf(sss, "Cdt-now: y0 =%d, y1=%d, m1=%d, m2=%d, d1=%d\r\n", y0, y1, m1, m2, d1);
     KIKA_LOGD(sss);
     #endif

     if ( y0 ==true )
     {
          gCandidateCount =0;
          wcscpy(Candidates[gCandidateCount++], MSG_EXPIRED);
          return 1;
     }
     else if ( y1 ==true )
     {
          if ( m1 ==true )
          {
               gCandidateCount =0;
               wcscpy(Candidates[gCandidateCount++], MSG_EXPIRED);
               return 1;
          }
          else if ( m2 ==true && d1 ==true )
          {
               gCandidateCount =0;
               wcscpy(Candidates[gCandidateCount++], MSG_EXPIRED);
               return 1;
          }
     }
     #endif

     #if !defined(__TEST) && defined(__LOCK_MODEL_NAME)

     if ( wcscmp(gWsModel.c_str(), LOCK_MODEL_NAME) !=0 )
     {
          gCandidateCount =0;
          wcscpy(Candidates[gCandidateCount++], MSG_INVALID_MODEL);

          #if defined(__DEBUG_LOG)
          {
              char sss[1024]={0};
              sprintf(sss, "ModelName =%ls, Count =%d\r\n", gWsModel.c_str(), gCandidateCount);
              KIKA_LOGD(sss);
          }
          #endif

          return 1;
     }
     #endif

     #if !defined(__FOR_ANDROID)
     //if ( gTmpOff ==0 )
     {
          switch ( _IME )
          {
                   case IQQI_IME_Changjie:
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_TW:
                        if ( !gImeX.isV1() )
                        {
                             gCandidateCount =gCache.check(_IME, KeyCodeEx, Candidates, Begin, Number);

                             if ( gCandidateCount !=0 )
                             {
                                  return gCandidateCount;
                             }
                        }
                        break;

                   default:
                        gCache.clear();
                        break;
          }
     }
     #endif

     wstring
     wsKeyCode =KeyCodeEx,
     wsKeyCodeMatch;

     //------------------------------------------------------------------------
     int
     posKey =wsKeyCode.find(IQQI_VERSION_ID_KEY);

     if ( posKey >=0 )
     {
          switch ( _IME )
          {
                   case IQQI_IME_Changjie:
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_HK:
                   case IQQI_IME_Chinese_TW:
                        // Chinese Category only have 1 character space, not suitable for this
                        return 0;
          }
          // display kernel version number
          gCandidateCount =0;
          wcscpy(Candidates[gCandidateCount++], IQQI_VERSION_NO_W);

          wchar_t wsDict_ID[256] ={0};
          if ( gImeX.isV3() )
          {
               swprintf(wsDict_ID, 256, L"T9 Dict Version 3\r\n");
          }
          else if ( gImeX.isV2_1() )
          {
               swprintf(wsDict_ID, 256, L"T9 Dict Version 2_1\r\n");
          }
          else if ( gImeX.isV2() )
          {
               swprintf(wsDict_ID, 256, L"T9 Dict Version 2\r\n");
          }
          else
          {
               swprintf(wsDict_ID, 256, L"T9 Dict Version 1\r\n");
          }
          wcscpy(Candidates[gCandidateCount++], wsDict_ID);

          #if !defined(__TEST) && defined(__LOCK_MODEL_NAME)
          wcscpy(Candidates[gCandidateCount++], gWsModel.c_str());
          #endif

          return gCandidateCount;
     }

     //------------------------------------------------------------------------
     posKey =wsKeyCode.find(L":");

     if ( posKey >0 )
     {
          // for chinese (TW,CN,STROKE)
          posKey++;
          wsKeyCodeMatch =wsKeyCode.substr(posKey, wsKeyCode.size() -posKey);

          wsKeyCode.resize(posKey -1);

          #if defined(__DEBUG_MSG) || defined(__DEBUG_LOG)
          char aa[256] ={0};
          sprintf(aa, "wsKeyCode      =%ls\r\n", wsKeyCode.c_str());
          d_pintf(aa);
          sprintf(aa, "wsKeyCodeMatch =%ls\r\n", wsKeyCodeMatch.c_str());
          d_pintf(aa);
          #endif
     }

     IQQI_FreeKika();

     wchar_t*
     KeyCode =(wchar_t *)wsKeyCode.c_str();

     int
     wsLen =wcslen(KeyCode);

     //------------------------------------------------------------------------
     int
     retVal =0;

     #if defined(__DEMO_AP)
     if ( wsLen >4 )
     {
          //return gCandidateCount =0;
     }
     #endif

     int
     NumberEx =Number;

     #if defined(__FOR_ANDROID)
     NumberEx =MAX_CHI_WORD_GET_NUMBER;
     #endif

     //------------------------------------------------------------------------
     // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
     // volatile word is first priority
     // volatile word = 使用者選過的候選字或學習過的字
     //------------------------------------------------------------------------
     #if !defined(__I_WITH) && defined(__FOR_PUNKT)
     {
          vector<wstring>
          vc;

          gImeX_Prio->getKeyFuzzyByTimesN(_IME, KeyCodeEx, vc, NumberEx);

          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          {
              string
              svoca =WstringToString(KeyCodeEx);
              KIKA_LOGD("Kika-ENGINE, get match pri-word ( %s ) =%d\r\n", svoca.c_str(), (int)vc.size());
          }
          #endif

          int
          vIdx1 =0;

          //-------------------------------------------------------------------
          // 先找4字詞以內的為第一優先
          //-------------------------------------------------------------------
          if ( g4LenPriority ==1 )
          {
               for ( vIdx1 =0; vIdx1 <vc.size(); vIdx1++ )
               {
                     wstring
                     wsPhrs =vc[vIdx1];

                     if ( wsPhrs.size() <=4 )
                          vdict.push_back(VCandidate(wsPhrs.c_str(), vIdx1 +EXTRA_I, VTYPE_USER));

                     #if defined(__FOR_ANDROID)
                     if ( vdict.size() <3 )
                     {
                          // list first priority candidates
                          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                          string
                          svoca =WstringToString(wsPhrs.c_str());
                          KIKA_LOGD("Kika-ENGINE, get match pri-word =%s\r\n", svoca.c_str());
                          #endif
                     }
                     #endif
               }
          }

          //-------------------------------------------------------------------
          // 再先找4字詞以外的為第二優先
          //-------------------------------------------------------------------
          for ( vIdx1 =0; vIdx1 <vc.size(); vIdx1++ )
          {
                 wstring
                 wsPhrs =vc[vIdx1];

                 if ( g4LenPriority ==0 || wsPhrs.size() >4 )
                      vdict.push_back(VCandidate(wsPhrs.c_str(), vIdx1 +EXTRA_I, VTYPE_USER));

                 #if defined(__FOR_ANDROID)
                 if ( g4LenPriority ==0 || vdict.size() <3 )
                 {
                      // list first priority candidates
                      #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                      string
                      svoca =WstringToString(wsPhrs.c_str());
                      KIKA_LOGD("Kika-ENGINE, get match pri-word =%s\r\n", svoca.c_str());
                      #endif
                 }
                 #endif
          }

          if ( vdict.size() >0 )
          {
               #if defined(__APPLY_RULE)
               //vcandidate_applyRule(vdict);
               #endif
          }
     }

     #else

     vector<VCandidate>
     vdictPrio;

     //------------------------------------------------------------------------
     // volatile (user) selected word is first priority
     //------------------------------------------------------------------------
     {
          vector<wstring>
          vc;

          gImeX_Prio->getKeyFuzzyByTimesN(_IME, KeyCodeEx, vc, NumberEx);

          #if (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          {
              string
              svoca =WstringToString(KeyCodeEx);
              KIKA_LOGD("Kika-ENGINE, get match pri-word ( %s ) =%d\r\n", svoca.c_str(), (int)vc.size());
          }
          #endif

          for ( int vIdx1 =0; vIdx1 <vc.size(); vIdx1++ )
          {
                wstring
                wsPhrs =vc[vIdx1];

                #if (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
                {
                     // list first priority candidates
                     string
                     svoca =WstringToString(wsPhrs.c_str());
                     KIKA_LOGD("Kika-ENGINE, get match pri-word =%s\r\n", svoca.c_str());
                }
                #endif

                vdictPrio.push_back(VCandidate(wsPhrs.c_str(), vIdx1 +EXTRA_I, VTYPE_USER));
          }

          if ( vdictPrio.size() >0 )
          {
               #if defined(__APPLY_RULE)
               //vcandidate_applyRule(vdictPrio);
               #endif
          }
     }
     #endif

     //------------------------------------------------------------------------
     // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
     //------------------------------------------------------------------------

     #if defined(__I_WITH)
     vector<VCandidate>
     vdictUser;
     #endif

     //2020/11/13
     if ( wsLen >1 || gKeybdMode ==for_Qwerty )
     {
          #if defined(__PHRS_DIC)

          vector<wstring>
          phv;

          //-------------------------------------------------------------------
          // get Phrase
          //-------------------------------------------------------------------
          gPhrs->get(_IME, KeyCode, phv, NumberEx);

          int
          vcIdx =vdict.size(); //0

          for ( int i =0; i <phv.size(); i++ )
          {
                wstring
                wsPhrs =phv[i];

                vdict.push_back(VCandidate(wsPhrs.c_str(), i +vcIdx +EXTRA_I,  VTYPE_PHRS));
          }

          #endif

          //-------------------------------------------------------------------
          // get User DB
          //-------------------------------------------------------------------
          #if !defined(__NO_KIKA_IME)

          #if defined(__USER_DIC)

          retVal =gUser.Initial(_IME, (char *)gUserDBPath.c_str(), NULL, true);
          if ( retVal !=0 )
          {
               #if defined(__DEBUG_MSG) || defined(__TEST) || defined(__DEBUG_LOG)
               char aa[256] ={0};
               sprintf(aa, "user initial retVal =%d\r\n%s\r\n", retVal, (char *)gUserDBPath.c_str());
               d_pintf(aa);
               #endif

               #if defined(__TEST)
               showErr(retVal);
               #endif
          }

          if ( retVal ==0 )
          {
               #if !defined(__I_WITH)
               retVal =gUser.GetCandidates(_IME, KeyCode, vdict, VTYPE_USER);
               #else
               retVal =gUser.GetCandidates(_IME, KeyCode, vdictUser, VTYPE_USER);
               #endif

               if ( retVal <0 )
               {
                    #if defined(__DEBUG_MSG) || defined(__DEBUG_LOG)
                    char aa[256] ={0};
                    sprintf(aa, "user GetCandidates retVal =%d\r\n", retVal);
                    d_pintf(aa);
                    #endif

                    #if defined(__TEST) && (defined(__DEBUG_MSG) || defined(__DEBUG_LOG))
                    if ( gOutVer ==true )
                    {
                         d_pintf_ex(aa);
                         showErr(retVal);
                    }
                    #endif
              }

              //---------------------------------------------------------------
              // 2020/11/30, 取得使用者詞典(_user)之候選字
              // (Kika 引擎詞典如果沒有已存之'號的詞, 會無法取得, 所以必須在這裡
              // 再取一次
              //---------------------------------------------------------------
              //#if !defined(__I_WITH)
              //if ( vdict.size() ==0 )
              //#else
              //if ( vdictUser.size() ==0 )
              //#endif
              {
                   vector<wstring>
                   vc;
                   gImeX_User->getKeyFuzzyByTimesN(_IME, KeyCodeEx, vc, NumberEx);

                   for ( int vIdx1 =0; vIdx1 <vc.size(); vIdx1++ )
                   {
                         wstring
                         wsPhrs =vc[vIdx1];

                         #if (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
                         {
                              // list first priority candidates
                              string
                              svoca =WstringToString(wsPhrs.c_str());
                              KIKA_LOGD("Kika-ENGINE, get match pri-word =%s\r\n", svoca.c_str());
                         }
                         #endif

                         #if !defined(__I_WITH)
                         vdict.push_back(VCandidate(wsPhrs.c_str(), vIdx1 +EXTRA_I, VTYPE_USER));
                         #else
                         vdictUser.push_back(VCandidate(wsPhrs.c_str(), vIdx1 +EXTRA_I, VTYPE_USER));
                         #endif
                    }
              }

              //---------------------------------------------------------------
              #if defined(__DEBUG_MSG) || defined(__TEST) || defined(__DEBUG_LOG)
              {
                   char aa[256] ={0};
                   #if !defined(__I_WITH)
                   sprintf(aa, "user get candidate total =%d\r\n", (int)vdict.size());
                   #else
                   sprintf(aa, "user get candidate total =%d\r\n", (int)vdictUser.size());
                   #endif
                   d_pintf_ex(aa);
              }
              #endif
          }
          gUser.Free();
          #endif

          #endif
     }

     //------------------------------------------------------------------------
     // Get Main
     //------------------------------------------------------------------------
     bool
     isT9 =false;

     switch ( _IME )
     {
              case IQQI_IME_Changjie:
                   // no matter key len, alway check isT9
                   if ( wsLen ==1 )
                   {
                        switch ( KeyCode[0] )
                        {
                                 case KEY_STAR_1:
                                      break;
                                 default:
                                      isT9 =setT9(_IME);
                                      break;
                        }
                   }
                   else
                   {
                        isT9 =setT9(_IME);
                   }
                   break;

              case IQQI_IME_Chinese_TW:
                   // no matter key len, alway check isT9
                   if ( wsLen ==1 )
                   {
                        switch ( KeyCode[0] )
                        {
                                 case KEY_STAR_1:
                                      break;
                                 default:
                                      isT9 =setT9(_IME);
                                      break;
                        }
                   }
                   else
                   {
                        isT9 =setT9(_IME);
                   }
                   break;

              case IQQI_IME_Chinese_CN:
                   if ( wsLen ==1 )
                   {
                        switch ( KeyCode[0] )
                        {
                                 case '1':
                                      break;
                                 case KEY_STAR_1:
                                      break;
                                 default:
                                      isT9 =setT9(_IME);
                                      break;
                        }
                   }
                   else
                   {
                        isT9 =setT9(_IME);
                   }
                   break;

              case IQQI_IME_Chinese_HK:
                   // no matter key len, alway check isT9
                   if ( wsLen ==1 )
                   {
                        switch ( KeyCode[0] )
                        {
                                 case KEY_STAR_1:
                                      break;
                                 #if !defined(__ENABLE_WILDCARD)
                                 case '6':
                                 #endif
                                 case '7':
                                 case '8':
                                 case '9':
                                 case '0':
                                      break;
                                 default:
                                      isT9 =setT9(_IME);
                                      break;
                        }
                   }
                   else
                   {
                        isT9 =setT9(_IME);
                   }
                   break;

              default:
                   // if  length ==1 always take MutilTap action, otherrwise check is T9 or not
                   isT9 =wsLen >1 && setT9(_IME);
                   break;

              case IQQI_IME_Korean:
                   isT9 =setT9(_IME);
                   break;
     }
     #if defined(__TEST)
     printf("IME =%d, IsT9 =%d\r\n", _IME, isT9);
     #endif

     //------------------------------------------------------------------------
     // 是否為艾維爾?
     //------------------------------------------------------------------------
     #if defined(__I_WITH)
     vector<VCandidate>
     vdictMain;
     #endif

     if ( isT9 ==true  )
     {
          #if defined(__DEBUG_MSG) && defined(__DEBUG_LOG)
          KIKA_LOGD("Kika-ENGINE, %d, get total =%ld", _IME, gImeX->count());
          #endif

          if ( gImeX->count() >0 )
          {
               vector<wstring>
               vc;

               int
               vcIdx =vdict.size();// 0;

               switch ( _IME )
               {
                        case IQQI_IME_Changjie:
                        case IQQI_IME_Chinese_CN:
                        case IQQI_IME_Chinese_TW:
                             {
                                  if ( gKeybdMode ==for_Qwerty )
                                  {
                                       #if defined(__DEBUG_MSG) && defined(__DEBUG_LOG)
                                       KIKA_LOGD("Kika-ENGINE, %d, get (%ls) Qwerty Candidates", _IME, KeyCode);
                                       #endif

                                       #if defined(__FOR_HUAWEI)

                                       if ( wsLen >1 )
                                       {
                                            //先取得首字候選字列
                                            #if defined(__CONTINUOUS_KEYIN)
                                             gImeX->getKeyFullMatchH(_IME, KeyCode, vc, wsKeyCodeMatch.c_str(), NumberEx);
                                            #else
                                             gImeX->getKeyFuzzyH(_IME, KeyCode, vc, wsKeyCodeMatch.c_str(), NumberEx);
                                            #endif
                                       }

                                       #endif

                                       if ( FullMatchQwerty ==true )
                                       {
                                            //再取得Qwetry之完全Match候選字列
                                            gImeX->getKeyFullMatchW(_IME, KeyCode, vc, wsKeyCodeMatch.c_str(), NumberEx);
                                       }
                                       else
                                       {
                                            //再取得Qwetry之只要前幾個Key Match之候選字列
                                            gImeX->getKeyFuzzyW(_IME, KeyCode, vc, wsKeyCodeMatch.c_str(), NumberEx);
                                       }
                                  }
                                  else
                                  {
                                       #if defined(__DEBUG_MSG) || defined(__TEST) || defined(__DEBUG_LOG)
                                       KIKA_LOGD("Kika-ENGINE, get T9 Chandidate");
                                       #endif

                                       gImeX->getKeyFuzzy(_IME, KeyCode, vc, wsKeyCodeMatch.c_str(), NumberEx);
                                  }
                             }
                             break;

                        case IQQI_IME_Chinese_HK:
                             {
                                  gImeX->getKeyWildc(_IME, KeyCode, vc, L"6", NumberEx);
                             }
                             break;

                        case IQQI_IME_Korean:
                             {
                                  wstring
                                  wNewKeyinKr =KeyCode;

                                  if ( gKeybdMode ==for_T9 )
                                  {
                                       autotest_kr
                                       krX;

                                       wstring
                                       wKrKey, wGetAns;

                                       krX.numberGetResult(KeyCode, wKrKey, wGetAns, false, false);

                                       #if defined(__DEBUG_MSG) && (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
                                       KIKA_LOGD("Kika-ENGINE, %ls, numberGet Korean =%ls\r\n", KeyCode, wGetAns.c_str());
                                       #endif

                                       wNewKeyinKr =wGetAns;
                                  }

                                  gImeX_User->getKeyFuzzy(_IME, wNewKeyinKr.c_str(), vc, NumberEx);

                                  #if defined(__DEBUG_MSG) && (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
                                  KIKA_LOGD("Kika-ENGINE, getKeyFuzzy User Korean =%ls, match total =%d\r\n", wNewKeyinKr.c_str(), (int)vc.size());
                                  #endif

                                  for ( vcIdx =0; vcIdx <vc.size(); vcIdx++ )
                                  {
                                        wstring
                                        wsPhrs =vc[vcIdx];

                                        //------------------------------------------
                                        // 艾維爾 先把結果放入 vdictMain
                                        //------------------------------------------
                                        #if defined(__I_WITH)
                                        vdictMain.push_back(VCandidate(wsPhrs.c_str(), vcIdx +EXTRA_I, VTYPE_USER));
                                        #else
                                        vdict.push_back(VCandidate(wsPhrs.c_str(), vcIdx +EXTRA_I, VTYPE_USER));
                                        #endif
                                  }

                                  vc.clear();
                                  gImeX->getKeyFuzzy(_IME, wNewKeyinKr.c_str(), vc, NumberEx);

                                  #if defined(__DEBUG_MSG) && (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
                                  KIKA_LOGD("Kika-ENGINE, getKeyFuzzy Main Korean =%ls, match total =%d\r\n", wNewKeyinKr.c_str(), (int)vc.size());
                                  #endif

                                  break;
                             }

                        default:
                             gImeX_User->getKeyFuzzy(_IME, KeyCode, vc, NumberEx);

                             for ( vcIdx =0; vcIdx <vc.size(); vcIdx++ )
                             {
                                   wstring
                                   wsPhrs =vc[vcIdx];

                                   //------------------------------------------
                                   // 艾維爾 先把結果放入 vdictMain
                                   //------------------------------------------
                                   #if defined(__I_WITH)
                                   vdictMain.push_back(VCandidate(wsPhrs.c_str(), vcIdx +EXTRA_I, VTYPE_USER));
                                   #else
                                   vdict.push_back(VCandidate(wsPhrs.c_str(), vcIdx +EXTRA_I, VTYPE_USER));
                                   #endif
                             }

                             vc.clear();
                             gImeX->getKeyFuzzy(_IME, KeyCode, vc, NumberEx);
                             break;
               }

               for ( int i =0; i <vc.size(); i++ )
               {
                     wstring
                     wsPhrs =vc[i];

                     //--------------------------------------------------------
                     // 艾維爾 先把結果放入 vdictMain
                     //--------------------------------------------------------
                     #if defined(__I_WITH)
                     vdictMain.push_back(VCandidate(wsPhrs.c_str(), vcIdx +i +EXTRA_I, VTYPE_MAIN));
                     #else
                     vdict.push_back(VCandidate(wsPhrs.c_str(), vcIdx +i +EXTRA_I, VTYPE_MAIN));
                     #endif
               }

               #if defined(__I_WITH)
               if ( vdictMain.size() >0 )
               #else
               if ( vdict.size() >0 )
               #endif
               {
                    #if defined(__APPLY_RULE)
                     #if defined(__I_WITH)
                    vcandidate_applyRule(vdictMain);
                     #else
                    vcandidate_applyRule(vdict);
                     #endif
                    #endif
               }
          }
     }

     #if !defined(__NO_KIKA_IME)

     else
     {
          //-------------------------------------------------------------------
          retVal =gMain.Initial(_IME, (char *)gMainDBPath.c_str(), NULL);

          if ( retVal !=0 && wsLen >1 )
          {
               #if defined(__DEBUG_MSG) || defined(__TEST) || defined(__DEBUG_LOG)
               char aa[256] ={0};
               sprintf(aa, "main initial retVal =%d\r\n%s\r\n", retVal, (char *)gMainDBPath.c_str());
               d_pintf(aa);
               #endif

               #if defined(__TEST)
               showErr(retVal);
               #endif

               //if ( vdict.size() ==0 )
               {
                    return gCandidateCount =0;
               }
          }

          #if !defined(__TEST_USER_ONLY)
           #if defined(__I_WITH)
          retVal =gMain.GetCandidates(_IME, KeyCode, vdictMain, VTYPE_MAIN);
           #else
          retVal =gMain.GetCandidates(_IME, KeyCode, vdict, VTYPE_MAIN);
           #endif
          #endif

          if ( retVal <0 )
          {
               #if defined(__TEST)
               showErr(retVal);
               #endif

               if ( vdict.size() ==0 )
               {
                    return gCandidateCount =0;
               }
          }
          else
          {
               #if defined(__DEBUG_MSG) || defined(__TEST) || defined(__DEBUG_LOG)
               char aa[256] ={0};
               sprintf(aa, "main GetCandidates retVal =%d\r\n", retVal);
               d_pintf(aa);
               #endif
          }
     }

     #endif

     if ( FirstCandidateIsUserTyping ==true )
     {
          #if defined(__DEBUG_MSG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          KIKA_LOGD("Kika-ENGINE, FirstCandidateIsUserTyping b4 vdict total =%d\r\n", (int)vdict.size());
          #endif

          int
          vcIdx =vdict.size();
          vdict.push_back(VCandidate(KeyCodeEx, EXTRA_I +(vcIdx++), VTYPE_PRIO));

          #if defined(__DEBUG_MSG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          KIKA_LOGD("Kika-ENGINE, FirstCandidateIsUserTyping af vdict total =%d\r\n", (int)vdict.size());
          #endif
     }

     //------------------------------------------------------------------------
     // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
     // 取得最終結果 ( Final Result )
     //------------------------------------------------------------------------
     vector<wstring>
     vPrio4,
     vPrioMisc;

     vector<wstring>
     vUser4,
     vUserMisc;

     vector<wstring>
     vMain4,
     vMainMisc;

     int
     vcIdx =vdict.size();

     //-------------------------------------------------------------------------
     //2020/11/13
     wchar_t*
     pCmpMatchKey =0;
     //-------------------------------------------------------------------------

     //-------------------------------------------------------------------------
     bool
     useFullMatchKey =gFullMatchKey;
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_TW:
                   //中文類不使用FullMatchKey規則, 否則連打會超慢
                   useFullMatchKey =0;
                   break;
              default:
                   break;
     }
     //-------------------------------------------------------------------------

     #if defined(__I_WITH)
     //if ( wsLen >1 )
     {
          #if defined(__DEBUG_MSG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          KIKA_LOGD("Kika-ENGINE, vdictPrio total =%d\r\n", (int)vdictPrio.size());
          #endif

          //-------------------------------------------------------------------
          // 先找[Volatile優先詞]
          //-------------------------------------------------------------------
          vector<VCandidate>::iterator
          bi =vdictPrio.begin();

          while ( bi !=vdictPrio.end() )
          {
                  //-----------------------------------------------------------
                  // 移除按键顺序与输入Key不对等的候选字, 2020/07/28
                  //-----------------------------------------------------------
                  int
                  c_cut =0;
                  if ( useFullMatchKey ==1 )
                  {
                       //wstring
                       //wsKeyMapEx = std::regex_replace(wsKeyMap, std::wregex(KeyCodeEx), L"");

                       //if ( wsKeyMapEx.size() >0 )
                       //{
                       //}

                       if ( _IME !=IQQI_IME_Korean && _IME !=IQQI_IME_Chinese_CN && _IME !=IQQI_IME_Chinese_TW && _IME !=IQQI_IME_Chinese_HK && _IME !=IQQI_IME_Changjie )
                       {
                            wstring
                            wContent =bi->ws.c_str(),
                            wContentEx;

                            for ( auto c: wContent )
                            {
                                  wContentEx +=static_cast<wchar_t>(std::towlower(c));
                            }

                            #if defined(__TEST)
                            const wchar_t*
                            pCandSz =wContentEx.c_str();
                            #endif

                            wchar_t
                            wsKeyMap[MAX_INPUT_KEY] ={0};

                            //2020/11/13
                            //-------------------------------------------------
                            if ( gKeybdMode !=for_Qwerty  )
                            {
                                 gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKeyMap, MAX_INPUT_KEY);
                                 pCmpMatchKey =wsKeyMap;
                            }
                            else
                            {
                                 #if defined(__SCAN_NEARBY_KEYCHAR)
                                 //如果頭碼不需要一致(週邊都要掃瞄到), 改成
                                 pCmpMatchKey =(wchar_t*)KeyCodeEx;
                                 #else
                                 //Huwawei, Qwerty頭碼都一致才允許
                                 pCmpMatchKey =(wchar_t*)wContentEx.c_str();
                                 #endif
                            }
                            //-------------------------------------------------

                            bool
                            unMatch =false;
                            for ( int kx =0; kx <wcslen(KeyCodeEx); kx++ )
                            {
                                  if ( pCmpMatchKey[kx] !=KeyCodeEx[kx] )
                                  {
                                       unMatch =true;
                                       break;
                                  }
                            }
                            if ( unMatch ==true )
                            {
                                 bi++;
                                 continue;
                            }
                            c_cut  =std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_QUOTATION );
                            //c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_MINUS );
                            #if !defined(__SONIM)
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_0 );
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_2 );
                            #endif
                       }
                  }
                  //-----------------------------------------------------------

                  switch ( _IME )
                  {
                           case IQQI_IME_Changjie:
                           case IQQI_IME_Chinese_CN:
                           case IQQI_IME_Chinese_HK:
                           case IQQI_IME_Chinese_TW:
                                if ( g4LenPriority ==1 )
                                {
                                     if ( (bi->ws.size() -c_cut) <=4 )
                                          vPrio4.push_back(bi->ws.c_str());
                                     else
                                          vPrioMisc.push_back(bi->ws.c_str());
                                }
                                else
                                {
                                     vPrio4.push_back(bi->ws.c_str());
                                }
                                break;

                           default:
                                //---------------------------------------------
                                // 一字不必套用學習規則
                                //---------------------------------------------
                                if ( wsLen >1 || gKeybdMode ==for_Qwerty )
                                {
                                     if ( g4LenPriority ==1 )
                                     {
                                          if ( (bi->ws.size() -c_cut) <=4 ) //&& wsLen >=bi->ws.size() )
                                               vPrio4.push_back(bi->ws.c_str());
                                          else
                                               vPrioMisc.push_back(bi->ws.c_str());
                                     }
                                     else
                                     {
                                          vPrio4.push_back(bi->ws.c_str());
                                     }
                                }
                                break;
                  }
                  bi++;
          }

          //-------------------------------------------------------------------
          // 然後是[使用者詞]
          //-------------------------------------------------------------------
          #if defined(__DEBUG_MSG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          KIKA_LOGD("Kika-ENGINE, vdictUser total =%d\r\n", (int)vdictUser.size());
          #endif

          bi =vdictUser.begin();
          while ( bi !=vdictUser.end() )
          {
                  //-----------------------------------------------------------
                  // 移除按键顺序与输入Key不对等的候选字
                  //-----------------------------------------------------------
                  int
                  c_cut =0;
                  if ( useFullMatchKey ==1 )
                  {
                       if ( _IME !=IQQI_IME_Korean && _IME !=IQQI_IME_Chinese_CN && _IME !=IQQI_IME_Chinese_TW && _IME !=IQQI_IME_Chinese_HK && _IME !=IQQI_IME_Changjie )
                       {
                            wstring
                            wContent =bi->ws.c_str(),
                            wContentEx;

                            for ( auto c: wContent )
                            {
                                  wContentEx +=static_cast<wchar_t>(std::towlower(c));
                            }

                            #if defined(__TEST)
                            const wchar_t*
                            pCandSz =wContentEx.c_str();
                            #endif

                            wchar_t
                            wsKeyMap[MAX_INPUT_KEY] ={0};

                            //2020/11/13
                            //-------------------------------------------------
                            if ( gKeybdMode !=for_Qwerty  )
                            {
                                 gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKeyMap, MAX_INPUT_KEY);
                                 pCmpMatchKey =wsKeyMap;
                            }
                            else
                            {
                                 #if defined(__SCAN_NEARBY_KEYCHAR)
                                 //如果頭碼不需要一致(週邊都要掃瞄到), 改成
                                 pCmpMatchKey =(wchar_t*)KeyCodeEx;
                                 #else
                                 //Huwawei, Qwerty頭碼都一致才允許
                                 pCmpMatchKey =(wchar_t*)wContentEx.c_str();
                                 #endif
                            }
                            //-------------------------------------------------

                            bool
                            unMatch =false;
                            for ( int kx =0; kx <wcslen(KeyCodeEx); kx++ )
                            {
                                  if ( pCmpMatchKey[kx] !=KeyCodeEx[kx] )
                                  {
                                       unMatch =true;
                                       break;
                                  }
                            }
                            if ( unMatch ==true )
                            {
                                 bi++;
                                 continue;
                            }
                            c_cut  =std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_QUOTATION );
                            //c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_MINUS );
                            #if !defined(__SONIM)
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_0 );
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_2 );
                            #endif
                       }
                  }
                  //-----------------------------------------------------------

                  //-----------------------------------------------------------
                  // 一字不必套用學習規則
                  //-----------------------------------------------------------
                  //2020/11/13
                  if ( wsLen >1 || gKeybdMode ==for_Qwerty )
                  {
                       if ( g4LenPriority ==1 )
                       {
                            if ( (bi->ws.size() -c_cut) <=4 ) //&& wsLen >=bi->ws.size() )
                                  vUser4.push_back(bi->ws.c_str());
                            else
                                  vUserMisc.push_back(bi->ws.c_str());
                       }
                       else
                       {
                            vUser4.push_back(bi->ws.c_str());
                       }
                  }
                  bi++;
          }

          //-------------------------------------------------------------------
          // 再然後是[Kika主詞]
          //-------------------------------------------------------------------
          #if defined(__DEBUG_MSG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          KIKA_LOGD("Kika-ENGINE, vdictMain total =%d\r\n", (int)vdictMain.size());
          #endif

          bi =vdictMain.begin();
          while ( bi !=vdictMain.end() )
          {
                  //-----------------------------------------------------------
                  // 移除按键顺序与输入Key不对等的候选字 (一字詞除外)
                  //-----------------------------------------------------------
                  int
                  c_cut =0;
                  if ( useFullMatchKey && wsLen >1 )
                  {
                       if ( _IME !=IQQI_IME_Korean && _IME !=IQQI_IME_Chinese_CN && _IME !=IQQI_IME_Chinese_TW && _IME !=IQQI_IME_Chinese_HK && _IME !=IQQI_IME_Changjie )
                       {
                            wstring
                            wContent =bi->ws.c_str(),
                            wContentEx;

                            for ( auto c: wContent )
                            {
                                  wContentEx +=static_cast<wchar_t>(std::towlower(c));
                            }

                            #if defined(__TEST)
                            const wchar_t*
                            pCandSz =wContentEx.c_str();
                            #endif

                            wchar_t
                            wsKeyMap[MAX_INPUT_KEY] ={0};

                            //2020/11/13
                            //-------------------------------------------------
                            if ( gKeybdMode !=for_Qwerty  )
                            {
                                 gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKeyMap, MAX_INPUT_KEY);
                                 pCmpMatchKey =wsKeyMap;
                            }
                            else
                            {
                                 #if defined(__SCAN_NEARBY_KEYCHAR)
                                 //如果頭碼不需要一致(週邊都要掃瞄到), 改成
                                 pCmpMatchKey =(wchar_t*)KeyCodeEx;
                                 #else
                                 //Huwawei, Qwerty頭碼都一致才允許
                                 pCmpMatchKey =(wchar_t*)wContentEx.c_str();
                                 #endif
                            }
                            //-------------------------------------------------

                            {
                                  #if (defined(__TEST) || defined(__DEBUG_UNMATCH)) && (defined(__DEBUG_MSG) || defined(__DEBUG_LOG))
                                  string
                                  svoca1 =WstringToString(wContentEx),
                                  svoca2 =WstringToString(wsKeyMap);

                                  char aa[256] ={0};
                                  sprintf(aa, "Kika-ENGINE, main check unMatch, wContentEx =%s, wsKeyMap =%s\r\n", svoca1.c_str(), svoca2.c_str());
                                  d_pintf(aa);
                                  #endif
                            }

                            bool
                            unMatch =false;
                            for ( int kx =0; kx <wcslen(KeyCodeEx); kx++ )
                            {
                                  if ( pCmpMatchKey[kx] !=KeyCodeEx[kx] )
                                  {
                                       #if (defined(__TEST) || defined(__DEBUG_UNMATCH)) && (defined(__DEBUG_MSG) || defined(__DEBUG_LOG))
                                       string
                                       svoca1 =WstringToString(wsKeyMap),
                                       svoca2 =WstringToString(KeyCodeEx);
                                       char aa[256] ={0};
                                       sprintf(aa, "kx =%d, Kika-ENGINE, main unMatch, wsKeyMap =%s, KeyCodeEx =%s\r\n", kx, svoca1.c_str(), svoca2.c_str());
                                       d_pintf(aa);
                                       #endif

                                       unMatch =true;
                                       break;
                                  }
                            }
                            if ( unMatch ==true )
                            {
                                 bi++;
                                 continue;
                            }
                            c_cut  =std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_QUOTATION );
                            //c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_MINUS );
                            #if !defined(__SONIM)
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_0 );
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_2 );
                            #endif
                       }
                  }
                  //-----------------------------------------------------------

                  if ( g4LenPriority ==1 )
                  {
                       if ( (bi->ws.size() -c_cut) <=4 )
                            vMain4.push_back(bi->ws.c_str());
                       else
                            vMainMisc.push_back(bi->ws.c_str());
                  }
                  else
                  {
                       vMain4.push_back(bi->ws.c_str());
                  }
                  bi++;
          }

          //-------------------------------------------------------------------
          // final 結果
          // 4字詞以內的為第一優先(1.[Volatile優先詞], 2.[使用者詞], 3.[Kika主詞])
          //-------------------------------------------------------------------
          #if defined(__DEBUG_MSG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          KIKA_LOGD("Kika-ENGINE, vPrio4 total =%d\r\n", (int)vPrio4.size());
          KIKA_LOGD("Kika-ENGINE, vUser4 total =%d\r\n", (int)vUser4.size());
          KIKA_LOGD("Kika-ENGINE, vMain4 total =%d\r\n", (int)vMain4.size());
          KIKA_LOGD("Kika-ENGINE, vPrioMisc total =%d\r\n", (int)vPrioMisc.size());
          KIKA_LOGD("Kika-ENGINE, vUserMisc total =%d\r\n", (int)vUserMisc.size());
          KIKA_LOGD("Kika-ENGINE, vMainMisc total =%d\r\n", (int)vMainMisc.size());
          #endif

          vector<wstring>::iterator
          bii =vPrio4.begin();
          while ( bii !=vPrio4.end() )
          {
                  #if defined(__TEST)
                  if ( gDebug ==true )
                  {
                       const wchar_t*
                       pCandSz =bii->c_str();
                       printf("vPrio4 =%ls\r\n", pCandSz);
                  }
                  #endif

                  #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
                  string
                  svoca =WstringToString(bii->c_str());
                  KIKA_LOGD("Kika-ENGINE, add Pri-Prio(1-4) candidate =%s\r\n", svoca.c_str());
                  #endif

                  vdict.push_back(VCandidate(bii->c_str(), EXTRA_I +(vcIdx++), VTYPE_PRIO));
                  bii++;
          }
          bii =vUser4.begin();
          while ( bii !=vUser4.end() )
          {
                  #if defined(__TEST)
                  if ( gDebug ==true )
                  {
                       const wchar_t*
                       pCandSz =bii->c_str();
                       printf("vUser4 =%ls\r\n", pCandSz);
                  }
                  #endif

                  #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
                  string
                  svoca =WstringToString(bii->c_str());
                  KIKA_LOGD("Kika-ENGINE, add Pri-User(1-4) candidate =%s\r\n", svoca.c_str());
                  #endif

                  vdict.push_back(VCandidate(bii->c_str(), EXTRA_I +(vcIdx++), VTYPE_PRIO));
                  bii++;
          }
          bii =vMain4.begin();
          while ( bii !=vMain4.end() )
          {
                  #if defined(__TEST)
                  if ( gDebug ==true )
                  {
                       const wchar_t*
                       pCandSz =bii->c_str();
                       printf("vMain4 =%ls\r\n", pCandSz);
                  }
                  #endif

                  #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
                  string
                  svoca =WstringToString(bii->c_str());
                  KIKA_LOGD("Kika-ENGINE, add Pri-Main(1-4) candidate =%s\r\n", svoca.c_str());
                  #endif

                  vdict.push_back(VCandidate(bii->c_str(), EXTRA_I +(vcIdx++), VTYPE_PRIO));
                  bii++;
          }

          //-------------------------------------------------------------------
          // 4字詞以外的為第二優先(1.[Volatile優先詞], 2.[使用者詞], 3.[Kika主詞])
          //-------------------------------------------------------------------
          if ( g4LenPriority ==1 )
          {
               bii =vPrioMisc.begin();
               while ( bii !=vPrioMisc.end() )
               {
                       #if defined(__TEST)
                       if ( gDebug ==true )
                       {
                            const wchar_t*
                            pCandSz =bii->c_str();
                            printf("vPrioMisc =%ls\r\n", pCandSz);
                       }
                       #endif

                       #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
                       string
                       svoca =WstringToString(bii->c_str());
                       KIKA_LOGD("Kika-ENGINE, add Prio(>4) candidate =%s\r\n", svoca.c_str());
                       #endif

                       vdict.push_back(VCandidate(bii->c_str(), EXTRA_I +(vcIdx++), VTYPE_USER));
                       bii++;
               }
               bii =vUserMisc.begin();
               while ( bii !=vUserMisc.end() )
               {
                       #if defined(__TEST)
                       if ( gDebug ==true )
                       {
                            const wchar_t*
                            pCandSz =bii->c_str();
                            printf("vUserMisc =%ls\r\n", pCandSz);
                       }
                       #endif

                       #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
                       string
                       svoca =WstringToString(bii->c_str());
                       KIKA_LOGD("Kika-ENGINE, add User(>4) candidate =%s\r\n", svoca.c_str());
                       #endif

                       vdict.push_back(VCandidate(bii->c_str(), EXTRA_I +(vcIdx++), VTYPE_USER));
                       bii++;
               }
               bii =vMainMisc.begin();
               while ( bii !=vMainMisc.end() )
               {
                       #if defined(__TEST)
                       if ( gDebug ==true )
                       {
                            const wchar_t*
                            pCandSz =bii->c_str();
                            printf("vMainMisc =%ls\r\n", pCandSz);
                       }
                       #endif

                       #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
                       string
                       svoca =WstringToString(bii->c_str());
                       KIKA_LOGD("Kika-ENGINE, add Main(>4) candidate =%s\r\n", svoca.c_str());
                       #endif

                       vdict.push_back(VCandidate(bii->c_str(), EXTRA_I +(vcIdx++), VTYPE_MAIN));
                       bii++;
               }
          }

          //-------------------------------------------------------------------
          // Debug Msg
          //-------------------------------------------------------------------
          #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
          bi =vdict.begin();
          while ( bi !=vdict.end() )
          {
                  string
                  svoca =WstringToString(bi->ws.c_str());

                  KIKA_LOGD("Kika-ENGINE, beta t =%d, candidate =%s\r\n", bi->vtype, svoca.c_str());

                  bi++;
          }
          #endif
     }

     #else

     //-------------------------------------------------------------------------
     //2020/11/13
     if ( useFullMatchKey ==1 )
     {
          #if defined(__TEST)
          printf("vMain4 total =%d\r\n", vMain4.size());
          #endif

          //-------------------------------------------------------------------
          // 再然後是[Kika主詞]
          //-------------------------------------------------------------------
          #if defined(__DEBUG_MSG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          KIKA_LOGD("Kika-ENGINE, vdict total =%d\r\n", (int)vdict.size());
          #endif

          vector<VCandidate>::iterator
          bix =vdict.begin();
          while ( bix !=vdict.end() )
          {
                  //-----------------------------------------------------------
                  // 移除按键顺序与输入Key不对等的候选字
                  //-----------------------------------------------------------
                  int
                  c_cut =0;
                  if ( gFullMatchKey ==1 )
                  {
                       if ( _IME !=IQQI_IME_Chinese_CN && _IME !=IQQI_IME_Chinese_TW && _IME !=IQQI_IME_Changjie )
                       {
                            wstring
                            wContent =bix->ws.c_str(),
                            wContentEx;

                            for ( auto c: wContent )
                            {
                                  wContentEx +=static_cast<wchar_t>(std::towlower(c));
                            }

                            #if defined(__TEST)
                            const wchar_t*
                            pCandSz =wContentEx.c_str();
                            #endif

                            wchar_t
                            wsKeyMap[MAX_INPUT_KEY] ={0};

                            if ( gKeybdMode !=for_Qwerty  )
                            {
                                 gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKeyMap, MAX_INPUT_KEY);
                                 {
                                    #if (defined(__TEST) || defined(__DEBUG_UNMATCH)) && (defined(__DEBUG_MSG) || defined(__DEBUG_LOG))
                                    string
                                    svoca1 =WstringToString(wContentEx),
                                    svoca2 =WstringToString(wsKeyMap);

                                    char aa[256] ={0};
                                    sprintf(aa, "Kika-ENGINE, main check unMatch, wContentEx =%s, wsKeyMap =%s\r\n", svoca1.c_str(), svoca2.c_str());
                                    d_pintf(aa);
                                    #endif
                                 }
                                 pCmpMatchKey =wsKeyMap;
                            }
                            else
                            {
                                 pCmpMatchKey =(wchar_t*)wContentEx.c_str();
                            }

                            #if defined(__TEST) && defined(__DEBUG_MSG)
                            printf("KeyCodeEx  =%ls\r\n", KeyCodeEx);
                            printf("pCmpMatchKey =%ls\r\n", pCmpMatchKey);
                            #endif

                            bool
                            unMatch =false;
                            for ( int kx =0; kx <wcslen(KeyCodeEx); kx++ )
                            {
                                  if ( pCmpMatchKey[kx] !=KeyCodeEx[kx] )
                                  {
                                       #if (defined(__TEST) || defined(__DEBUG_UNMATCH)) && (defined(__DEBUG_MSG) || defined(__DEBUG_LOG))
                                       string
                                       svoca1 =WstringToString(pCmpMatchKey),
                                       svoca2 =WstringToString(KeyCodeEx);
                                       char aa[256] ={0};
                                       sprintf(aa, "kx =%d, Kika-ENGINE, main unMatch, wsKeyMap =%s, KeyCodeEx =%s\r\n", kx, svoca1.c_str(), svoca2.c_str());
                                       d_pintf(aa);
                                       #endif

                                       unMatch =true;
                                       break;
                                  }
                            }
                            if ( unMatch ==true )
                            {
                                 bix++;
                                 continue;
                            }
                            c_cut  =std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_QUOTATION );
                            //c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_MINUS );
                            #if !defined(__SONIM)
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_0 );
                            c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_2 );
                            #endif
                       }
                  }
                  //-----------------------------------------------------------

                  vMain4.push_back(bix->ws.c_str());

                  #if defined(__TEST) && defined(__DEBUG_MSG)
                  printf("vMain4 add =%ls, total =%d\r\n", bix->ws.c_str(), vMain4.size());
                  #endif

                  bix++;
          }

          vdict.clear();

          #if defined(__TEST)
          printf("final vdict total b4 =%d\r\n", vdict.size());
          #endif

          vector<wstring>::iterator
          bii =vMain4.begin();
          while ( bii !=vMain4.end() )
          {
                  #if defined(__TEST)
                  if ( gDebug ==true )
                  {
                       const wchar_t*
                       pCandSz =bii->c_str();
                       printf("vMain4 =%ls\r\n", pCandSz);
                  }
                  #endif

                  #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
                  string
                  svoca =WstringToString(bii->c_str());
                  KIKA_LOGD("Kika-ENGINE, add Pri-Main(1-4) candidate =%s\r\n", svoca.c_str());
                  #endif

                  vdict.push_back(VCandidate(bii->c_str(), EXTRA_I +(vcIdx++), VTYPE_PRIO));
                  bii++;
          }
     }
     //-------------------------------------------------------------------------
     #endif

     //------------------------------------------------------------------------
     // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
     //------------------------------------------------------------------------
     #if defined(__TEST)
     printf("final vdict get total =%d\r\n", vdict.size());
     #endif

     #if defined(__APPLY_RULE)
      #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_HK:
              case IQQI_IME_Chinese_TW:
                   #if !defined(__FOR_HUAWEI)
                   vcandidate_applyRuleEx(vdict, wsLen);
                   break;
                   #endif
              default:
                   vcandidate_applyRule(vdict);
     }
      #else
     vcandidate_applyRule(vdict);
      #endif
     #endif

     //------------------------------------------------------------------------
     // Debug Msg
     //------------------------------------------------------------------------
     #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID)
     {
          vector<VCandidate>::iterator
          bi =vdict.begin();

          while ( bi !=vdict.end() )
          {
                  string
                  svoca =WstringToString(bi->ws.c_str());

                  KIKA_LOGD("Kika-ENGINE, final t =%d, candidate =%s\r\n", bi->vtype, svoca.c_str());

                  bi++;
          }
     }
     #endif

     //------------------------------------------------------------------------
     // re-assign counter & content
     gCandidateCount =0;

     #if defined(__TEST)
     int
     cIdx =0;

     if ( gDebug ==true )
     {
          printf("--------------------\r\n");
          printf("Key =%ls, engine get all candidates total =%d\r\n", KeyCode, (int)vdict.size());
          printf("--------------------\r\n");

          vector<VCandidate>::iterator
          iv =vdict.begin();

          for ( int ix =0; iv != vdict.end(); iv++ )
          {
                printf("candidate(%02d) =%ls\r\n", ++ix, iv->ws.c_str());
          }
          if ( vdict.size() >0 )
               printf("--------------------\r\n");
     }
     #endif

     vector<VCandidate>::iterator
     ii =vdict.begin();

     #if !defined(__I_WITH)

     // CUT-LEN RULE
     if ( ii !=vdict.end() )
     {
          switch ( _IME )
          {
                   case IQQI_IME_Changjie:
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_HK:
                   case IQQI_IME_Chinese_TW:
                        break;

                   default:
                        #if defined(__TEST)
                        if ( gDebug ==true )
                        {
                             printf("--------------------\r\n");
                             printf("find first one OK candidate\r\n");
                             printf("--------------------\r\n");
                        }
                        #endif

                        for ( ; ii != vdict.end(); ii++ )
                        {
                              #if defined(__TEST)
                              const wchar_t *
                              pCV =ii->ws.c_str();
                              int
                              pVLen =wcslen(pCV);
                              #endif

                              if ( ii->vtype == VTYPE_PHRS )
                              {
                                   #if defined(__TEST)
                                   if ( gDebug ==true )
                                   {
                                        printf("--------------------\r\n");
                                        printf(" Custom candidate!! => (%04d)=> %ls (len =%d)\r\n", ++cIdx, pCV, pVLen);
                                        printf("--------------------\r\n");
                                   }
                                   #endif
                                   //continue;
                              }

                              // find first one without space & lenght must equal or great then wsLen
                              if ( ii->ws.find(' ') ==-1 && wcslen(ii->ws.c_str()) >=wsLen )
                              {
                                   #if defined(__TEST)
                                   if ( gDebug ==true )
                                   {
                                        printf("--------------------\r\n");
                                        printf(" Key =%ls, OK found first one candidate!! => (%04d)=> %ls (len =%d)\r\n", KeyCode, ++cIdx, pCV, pVLen);
                                        printf("--------------------\r\n");
                                   }
                                   #endif

                                   break;
                              }

                              #if defined(__TEST)
                              if ( gDebug ==true )
                              {
                                   if ( ii->ws.find(' ') !=-1 )
                                        printf(" Invalid space, skip to next (%04d)=> %ls (len =%d)\r\n", ++cIdx, pCV, pVLen);
                                   else
                                        printf(" Invalid length, skip to next (%04d)=> %ls (len =%d)\r\n", ++cIdx, pCV, pVLen);
                              }
                              #endif
                        }

                        if ( ii !=vdict.end() )
                        {
                             // cut word rule
                             bool
                             NeedCut =false;

                             // 20-18-04-13
                             #if defined(NEW_R_20_18_0413)
                             if ( gTmpOff >0 )
                             {
                                  NeedCut =wcslen(ii->ws.c_str()) >gTmpOff;
                             }
                             else
                             {
                                  NeedCut =wcslen(ii->ws.c_str()) >wsLen;
                             }
                             #else
                             NeedCut =wcslen(ii->ws.c_str()) >wsLen;
                             #endif

                             if ( NeedCut ) //xx
                             //if ( wcslen(ii->ws.c_str()) >wsLen )
                             {
                                  wchar_t
                                  newV[256] = {0};

                                  #if defined(__TEST)
                                  if ( gBrackets ==true )
                                  {
                                       wcscpy (newV, L"[");
                                       wcsncat(newV, ii->ws.c_str(), wsLen);
                                       wcscat (newV, L"]");
                                  }
                                  else
                                  {
                                       wcsncpy(newV, ii->ws.c_str(), wsLen);
                                  }
                                  #else
                                  wcsncpy(newV, ii->ws.c_str(), wsLen);
                                  #endif

                                  vdict.push_back(VCandidate(newV, 0, ii->vtype));
                             }
                        }

                        #if defined(__APPLY_RULE)
                        vcandidate_applyRule(vdict);
                        #endif

                        break;
          }
     }

     #endif

     #if defined(__TEST)
     vdict_type.clear();

     cIdx =0;
     #endif

     #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
     int
     datIdx =0;
     #endif

     for ( ii =vdict.begin(); ii != vdict.end(); ii++ )
     {
           #if defined(__RULE_1)
           if ( gKeybdMode ==for_T9 && wcslen(ii->ws.c_str()) !=wsLen )
           {
                continue;
           }
           #endif

           if ( isT9 ==true || wcslen(ii->ws.c_str()) >=wsLen )
           {
                #if defined(__TEST)
                if ( wsLen ==1 )
                     replaceAll(ii->ws, L" ", L"[space]");
                #endif

                #if defined(__TEST)
                cIdx++;
                if ( gDebug ==true )
                {
                     if ( (ii->vtype != VTYPE_PHRS && wsLen !=1 && ii->ws.find(' ') !=-1) )
                     {
                           printf("   -> Invalid candidate, abandon (%04d) = %ls\r\n", cIdx, ii->ws.c_str());
                     }
                     else
                     {
                           printf(" (%04d) OK candidate =%ls\r\n", cIdx, ii->ws.c_str());
                     }
                }
                #endif

                if ( (ii->vtype == VTYPE_PHRS || wsLen ==1 || ii->ws.find(' ') ==-1) && gCandidateCount <NumberEx )
                {
                     #if defined(__TEST)
                     if ( gOutVer ==false && gAutoTest ==false )
                          replaceAll(ii->ws, L"\'", L"\'\'");
                     #endif

                     switch ( _IME )
                     {
                              case IQQI_IME_Changjie:
                              case IQQI_IME_Chinese_CN:
                              case IQQI_IME_Chinese_TW:
                              {
                                   int
                                   pos =ii->ws.find(L":");

                                   if ( pos >0 )
                                   {
                                        #if defined(__TEST)
                                        if ( gKaiOS_fmt ==true )
                                        #endif
                                             ii->ws.resize(pos);
                                   }
                              }
                     }

                     #if defined(__TEST)
                     vdict_type.push_back(VCandidate((wchar_t *)L"", 0, ii->vtype));
                     #endif

                     #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                     if ( datIdx <Begin )
                     {
                          datIdx++;

                          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                          //KIKA_LOGD("Kika-ENGINE, datIdx =%d, Begin =%d, continue\r\n", datIdx, Begin);
                          #endif
                          continue;
                     }
                     else if ( datIdx >=(Begin +Number) )
                     {
                          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                          KIKA_LOGD("Kika-ENGINE, GetCandidates Break!!!");
                          #endif
                          break;
                     }
                     datIdx++;
                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                     //KIKA_LOGD("Kika-ENGINE, datIdx =%d, Max =%d\r\n", datIdx, Begin +Number);
                     #endif

                     #endif

                     switch ( _IME )
                     {
                              #if !defined(__FOR_HUAWEI)
                              case IQQI_IME_Changjie:
                              case IQQI_IME_Chinese_CN:
                              case IQQI_IME_Chinese_TW:
                              case IQQI_IME_Chinese_HK:
                              #endif
                                   if ( gCandidateCount <(Begin +Number) )
                                   {
                                        #if defined(__TEST)
                                        if ( gDiagnostic ==true )
                                             wcsncpy(Candidates[gCandidateCount++], ii->ws.c_str(), MAX_ENG_WORD_LENGTH);
                                        else
                                             wcsncpy(Candidates[gCandidateCount++], ii->ws.c_str(), MAX_CHI_WORD_LENGTH);
                                        #else
                                         wcsncpy(Candidates[gCandidateCount++], ii->ws.c_str(), MAX_CHI_WORD_LENGTH);
                                        #endif

                                        #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_CANDIDATES_LIST)
                                        KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates 1, get Number =%d, ++gCandidateCount =%d\r\n", Number, gCandidateCount);
                                        #endif
                                   }
                                   break;

                              #if defined(__FOR_HUAWEI)
                              case IQQI_IME_Changjie:
                              case IQQI_IME_Chinese_CN:
                              case IQQI_IME_Chinese_TW:
                              case IQQI_IME_Chinese_HK:
                              #endif
                              default:
                                   if ( gCandidateCount <(Begin +Number) )
                                   {
                                        if ( ii->ws.size() >MAX_ENG_WORD_LENGTH )
                                             wcsncpy(Candidates[gCandidateCount++], ii->ws.c_str(), MAX_ENG_WORD_LENGTH);
                                        else
                                             wcscpy(Candidates[gCandidateCount++], ii->ws.c_str());
                                   }
                                   #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_CANDIDATES_LIST)
                                   KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates 2, get Number =%d, ++gCandidateCount =%d\r\n", Number, gCandidateCount);
                                   #endif
                     }

                     #if defined(__DEMO_AP)
                     if ( gCandidateCount ==1 )
                     {
                          wcscpy(Candidates[gCandidateCount++], L"[kika]");
                     }
                     #endif

                     // 20-18-04-13
                     #if defined(__TEST) && defined(NEW_R_20_18_0413)
                     if ( gTmpOff >0 )
                     {
                          break;
                     }
                     #endif
                }
           }
     }

     #if defined(__TEST)
     if ( gDebug ==true )
     {
          printf("--------------------\r\n");
          printf("final candidate\r\n");
          printf("--------------------\r\n");
     }
     #endif

     #if defined(__AUTO_APPEND_LAST) && !defined(__FOR_PUNKT)

     if ( gCandidateCount ==0 && gKeyLast.size() >wsLen )
     {
          gKeyLast =L"";
          gKeyLastLen =0;
     }

     if ( gCandidateCount ==0 && _IME !=IQQI_IME_Korean )
     {
          // Chinese no need Compensate action
          switch ( _IME )
          {
                   case IQQI_IME_Changjie:
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_HK:
                   case IQQI_IME_Chinese_TW:
                        #if defined(__TEST)
                        if ( gDebug ==true )
                        {
                             printf("--------------------\r\n");
                             printf("[[ 中文 [不需要] 進行倒刪補字 ]]\r\n");
                             printf("--------------------\r\n");
                        }
                        #endif
                        return gCandidateCount;
          }

          #if defined(__TEST)
          if ( gDebug ==true )
          {
               printf("--------------------\r\n");
               printf("[[ 進行倒刪補字, Compensate word !!! ]]\r\n");
               printf("--------------------\r\n");
          }
          #endif

          #if defined(__TEST)
          if ( gTmpOff >0 )
               return 0;
          #endif

          wchar_t
          wsCompensateKey[256] ={0};

          if ( gKeybdMode ==for_T9 )
          {
               #if defined(__TEST)
               int CandidateCount =gCandidateCount;

               wstring
               wKeyinEx =KeyCode;

               gTmpOff =wcslen(KeyCode);

               while ( gCandidateCount ==0 && gKeyLastLen ==0 && wKeyinEx.size() >1 )
               {
                       // backword to get last keyin have candidates
                       wKeyinEx.erase(wKeyinEx.size() -1, 1);

                       IQQI_GetCandidatesEx(_IME, (wchar_t*)wKeyinEx.c_str(), FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, FullMatchQwerty);
               }

               gTmpOff =0;
               gCandidateCount =CandidateCount;

               #endif

               //20-18-04-13
               #if defined(__TEST) && defined(NEW_R_20_18_0413)
               bool
               NeedCut =wcslen(KeyCode) >gTmpOff;

               if ( NeedCut ==false )
                    gKeyLastLen =0;
               #endif

               wstring
               sCutKey =KeyCode;
               sCutKey.erase(0, gKeyLastLen);

               gMap.Numb2keyCode(_IME, sCutKey.c_str(), wsCompensateKey, 256);

               #if defined(__TEST)
               if ( gDebug ==true )
               {
                    wstring
                    sRestKey =KeyCode;
                    sRestKey.resize(gKeyLastLen);

                    int
                    cutLen =(int)(wcslen(KeyCode) -gKeyLastLen);

                    printf("Input          =(Len =%2d) %ls\r\n",
                                            (int)wcslen(KeyCode),
                                            KeyCode);
                    printf("Cut Key 倒刪   =(Len =%2d) %ls\r\n",
                                            cutLen,
                                            sRestKey.c_str());
                    printf("OK candidate   =(Len =%2d) %ls\r\n",
                                            (int)wcslen(gKeyLast.c_str()),
                                            gKeyLast.c_str());
                    printf("compensate Key =(Len =%2d) %ls\r\n",
                                            (int)wcslen(wsCompensateKey),
                                            wsCompensateKey);
                    printf("--------------------\r\n\r\n");
               }

               wstring
               wsNew;

               if ( gBrackets ==true )
               {
                    if ( gKeyLast.find(L"[") ==0 )
                    {
                         wsNew =L"[";
                         wsNew +=gKeyLast.c_str();
                         wsNew +=wsCompensateKey;
                         wsNew +=L"]";
                    }
                    else
                    {
                         wsNew =L"[[";
                         wsNew +=gKeyLast.c_str();
                         wsNew +=L"]";
                         wsNew +=wsCompensateKey;
                         wsNew +=L"]";
                    }
               }
               else
               {
                    wsNew =gKeyLast.c_str();
                    wsNew +=wsCompensateKey;
               }

               #else

               wstring
               wsNew =gKeyLast.c_str();
               wsNew +=wsCompensateKey;

               #endif

               #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
               if ( datIdx >=Begin && datIdx <(Begin +Number) -1 )
               {
                    switch ( _IME )
                    {
                             #if !defined(__FOR_HUAWEI)
                             case IQQI_IME_Changjie:
                             case IQQI_IME_Chinese_HK:
                             case IQQI_IME_Chinese_TW:
                             #endif
                             case IQQI_IME_Chinese_CN:
                                  if ( gCandidateCount <(Begin +Number) )
                                  {
                                       //Chinese no need compensate, probably no change to set MAX_CHI_WORD_LENGTH
                                       wcsncpy(Candidates[gCandidateCount++], wsNew.c_str(), MAX_CHI_WORD_LENGTH);

                                       #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_CANDIDATES_LIST)
                                       KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates 3, ++gCandidateCount =%d\r\n", gCandidateCount);
                                       #endif
                                  }
                                  break;

                             #if defined(__FOR_HUAWEI)
                             case IQQI_IME_Changjie:
                             case IQQI_IME_Chinese_HK:
                             case IQQI_IME_Chinese_TW:
                             #endif
                             default:
                                  if ( wsNew.size() >0 && gCandidateCount <(Begin +Number) )
                                  {
                                       if ( wsNew.size() >MAX_ENG_WORD_LENGTH )
                                            wcsncpy(Candidates[gCandidateCount++], wsNew.c_str(), MAX_ENG_WORD_LENGTH);
                                       else
                                            wcscpy(Candidates[gCandidateCount++], wsNew.c_str());
                                  }
                                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_CANDIDATES_LIST)
                                  KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates 4, ++gCandidateCount =%d\r\n", gCandidateCount);
                                  #endif
                    }
                    datIdx++;
               }
               #else

               switch ( _IME )
               {
                        case IQQI_IME_Changjie:
                        case IQQI_IME_Chinese_CN:
                        case IQQI_IME_Chinese_HK:
                        case IQQI_IME_Chinese_TW:
                             if ( gCandidateCount <(Begin +Number) )
                             {
                                  //Chinese no need compensate, probably no change to set MAX_CHI_WORD_LENGTH
                                  wcsncpy(Candidates[gCandidateCount++], wsNew.c_str(), MAX_CHI_WORD_LENGTH);

                                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_CANDIDATES_LIST)
                                  KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates 5, ++gCandidateCount =%d\r\n", gCandidateCount);
                                  #endif
                             }
                             break;

                        default:
                             if ( wsNew.size() >0 && gCandidateCount <(Begin +Number) )
                             {
                                  if ( wsNew.size() >MAX_ENG_WORD_LENGTH )
                                       wcsncpy(Candidates[gCandidateCount++], wsNew.c_str(), MAX_ENG_WORD_LENGTH);
                                  else
                                       wcscpy(Candidates[gCandidateCount++], wsNew.c_str());
                             }

                             #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_CANDIDATES_LIST)
                             KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates 6, ++gCandidateCount =%d\r\n", gCandidateCount);
                             #endif
               }
               #endif
          }
     }
     else
     {
           gKeyLast =Candidates[0];
           gKeyLastLen =wsLen;
     }

     #endif

     #if !defined(__FOR_ANDROID)
     if ( gCandidateCount >0 )
     {
          switch ( _IME )
          {
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_TW:
                        if ( !gImeX.isV1() )
                        {
                             gCache.push(_IME, gCandidateCount, KeyCodeEx, Candidates);
                        }
                        break;
          }
     }
     #endif

     return gCandidateCount;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_GetCandidatesByMixT9Key(int _IME, wchar_t* KeyCodeEx, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates)
{
     /*
     #if defined(__FOR_HUAWEI)
     if ( gKeybdMode ==for_T9 )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidatesByMixT9Key In T9 Mode !!!");
          #endif

          wstring
          wKeyCode =KeyCodeEx,
          wKeyCodeMatch;

          int
          posKey =wKeyCode.find(L":");

          if ( posKey >0 )
          {
               posKey++;
               wKeyCodeMatch =wKeyCode.substr(posKey, wKeyCode.size() -posKey);

               wKeyCode.resize(posKey -1);

               #if defined(__DEBUG_MSG) || defined(__DEBUG_LOG)
               char aa[256] ={0};
               sprintf(aa, "Pinyn Mode, wKeyCode      =%ls\r\n", wKeyCode.c_str());
               d_pintf(aa);
               sprintf(aa, "Pinyn Mode, wKeyCodeMatch =%ls\r\n", wKeyCodeMatch.c_str());
               d_pintf(aa);
               #endif
          }

          CandidateCH
          CandClass;
          CandClass.alloc(MAX_GROUP_COUNT, MAX_GROUP_LENGTH);

          g_strokes =0;

          g_all.clear();
          g_allCandidate.clear();
          g_allCandidate.shrink_to_fit();
          g_allCandidate2.clear();

          wstring
          oldKeyin =wKeyCode,
          newKeyin =wKeyCode;

          getAll_Key2 *
          allKey =new getAll_Key2[newKeyin.size() +1];

          if ( allKey ==NULL )
          {
               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
               KIKA_LOGD("Kika-ENGINE, Memory not enought !!!");
               #endif
               return ERR_MEMORY_NOT_ENOUGHT;
          }

          while ( newKeyin.size() >0 )
          {
                  int
                  totalClass =IQQI_GetGrouping(_IME, (wchar_t*)newKeyin.c_str(), 0, 20, CandClass.pointer());

                  if ( totalClass ==0 )
                  {
                       newKeyin.resize(newKeyin.size() -1);
                       if ( newKeyin.size() ==0 )
                       {
                            if ( g_all.size() ==0 )
                                 break;

                            newKeyin =g_all[0];
                            break;
                       }
                       continue;
                  }
                  else
                  {
                       #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
                       KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates, %ls, total class =%d\r\n", newKeyin.c_str(), totalClass);
                       #endif

                       for ( int i =0; i <totalClass; i++ )
                       {
                             allKey[g_strokes].set(CandClass.record(i));

                             #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
                             KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates, allStrokes set =%ls\r\n", CandClass.record(i));
                             #endif

                             g_allStrokes[g_strokes].set(newKeyin);
                       }
                       g_strokes++;

                       #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
                       KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates, total, strokes =%d\r\n", g_strokes);
                       #endif

                       g_all.clear();
                       g_all.push_back(newKeyin);
                  }

                  int
                  ss =g_all[0].size();

                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT3)
                  // KIKA_LOGD("Kika-ENGINE, before IQQI_GetCandidates, oldKeyin =%ls\r\n", oldKeyin.c_str());
                  // KIKA_LOGD("Kika-ENGINE, before IQQI_GetCandidates, newKeyin =%ls\r\n", newKeyin.c_str());
                  // KIKA_LOGD("Kika-ENGINE, xxx g_all[0] =%ls, ss =%d\r\n", g_all[0].c_str(), ss);
                  #endif

                  newKeyin =oldKeyin.substr(ss, oldKeyin.size() - ss);
                  oldKeyin =newKeyin;

                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT3)
                  //KIKA_LOGD("Kika-ENGINE, after IQQI_GetCandidates, newKeyin =%ls\r\n", newKeyin.c_str());
                  #endif
          }

          int
          FirstTotalWord = 0;

          CandidateCH
          gCandVocaWord;
          gCandVocaWord.alloc(Number, MAX_ENG_WORD_LENGTH);

          const wchar_t* key2[256] ={0};
          const wchar_t *key3 =IQQI_NMapKeyCode(_IME, wKeyCode.c_str(), 256);

          // 先取得所有Key的組合候選字
          {
               FirstTotalWord =IQQI_GetCandidates_Py(_IME, (wchar_t*)key3,
                               false, 0, 0, Number,
                              (wchar_t**)gCandVocaWord.pointer());

               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
               KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates_Py, key3 =%ls, TotalWord2 =%d\r\n", key3, FirstTotalWord);
               #endif

               if ( FirstTotalWord >0 )
               {
                    wstring
                    wCandidate  =gCandVocaWord.record(0);

                    if ( g_allCandidate2.find(wCandidate) ==g_allCandidate2.end() )
                    {
                         //if ( IQQI_WordExist_Py(_IME, wCandidate.c_str()) )
                         {
                              g_allCandidate2.insert(wCandidate);

                              wstring
                              keyinCode;

                              for ( int x =0; x <g_strokes; x++ )
                              {
                                    keyinCode +=g_allStrokes[x].get(0);
                              }
                              g_allCandidate.push_back(XCandidate(wCandidate, keyinCode, key3, g_allCandidate.size()));

                              #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT3)
                              KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates_Py, Get First, Add Sz =%ls\r\n", wCandidate.c_str());
                              #endif
                         }
                    }
                    #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
                    KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates_Py, key3 get totoal =%d\r\n", (int)g_allCandidate.size());
                    #endif
               }
          }

          if ( newKeyin.size() >0 )
          {
               allKey[g_strokes++].set(newKeyin);

          //   int
          //   currIndex =0;
          //
          //   getCandidateLoop(_IME, currIndex, allKey, newKeyin, Number);
          }

          int
          currIndex =0;

          getCandidateLoop(_IME, currIndex, allKey, newKeyin, Number);

          delete [] allKey;

          if ( FirstTotalWord >1 )
          {
               // g_all.clear();
               // g_all.push_back(key3);

               for ( int ix =1; ix <Number && ix <FirstTotalWord; ix++ )
               {
                     wstring
                     wCandidate  =gCandVocaWord.record(ix);

                     if ( g_allCandidate2.find(wCandidate) ==g_allCandidate2.end() )
                     {
                          if ( !IQQI_WordExist_Py(_IME, wCandidate.c_str()) )
                          {
                                continue;
                          }
                          g_allCandidate2.insert(wCandidate);

                          wstring
                          keyinCode;

                          for ( int x =0; x <g_strokes; x++ )
                          {
                                keyinCode +=g_allStrokes[x].get(0);
                          }
                          g_allCandidate.push_back(XCandidate(wCandidate, keyinCode, key3, g_allCandidate.size()));

                          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT3)
                          KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates_Py, Get Rest of others, Add Sz =%ls\r\n", wCandidate.c_str());
                          #endif
                     }
               }
          }
          gCandidateCount =0;

          if ( g_allCandidate.size() >0 )
          {
               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT2)
               KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates_Py, CandidateCount =%d\r\n", (int)g_allCandidate.size());
               #endif

               sort(g_allCandidate.begin(), g_allCandidate.end(), compX);

               vector<XCandidate>::iterator
               bbi = g_allCandidate.begin();

               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
               KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates, GetNumber =%d\r\n", Number);
               #endif

               for ( int i =0; i <g_allCandidate.size() && i <Number; i++ )
               {
                     wcscpy(Candidates[gCandidateCount++], bbi->wWord.c_str());

                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT2)
                     KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates_Py, Final Add Sz =%ls\r\n", bbi->wWord.c_str());
                     #endif

                     //swprintf(wsLine, 4096, L"(%#6d) %ls [%ls][%ls]\r\n", i, bbi->wWord.c_str(), bbi->wKeyin.c_str(), bbi->wStroke.c_str());
                     bbi++;
               }
          }
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidates_Py, Final Candidate Count =%d\r\n", gCandidateCount);
          #endif

          return gCandidateCount;

          #endif
     }
     #else
     return IQQI_GetCandidatesEx(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates);
     #endif
     */
     return IQQI_GetCandidatesEx(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, false);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_GetCandidatesByHeadWord(int _IME, wchar_t* KeyCodeEx, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates)
{
     #if defined(__FOR_HUAWEI)
     int
     candIndex   =0,
     FirstGetLen =0;

     if ( gKeybdMode ==for_Qwerty )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, IQQI_GetCandidatesByHeadWord In T9 Mode !!!");
          #endif

          #if defined(__CONTINUOUS_KEYIN)

          wstring
          wsFirstCandidate;

          wstring
          oldKeyin =KeyCodeEx,
          newKeyin =KeyCodeEx;

          //先取得所有的4~2字詞
          while ( newKeyin.size() >0 )
          {
                  int
                  cutLen =0,
                  getLen =0;

                  if ( newKeyin.size() >=4 )
                  {
                       getLen =4;
                  }
                  else if ( newKeyin.size() >=3 )
                  {
                       getLen =3;
                  }
                  else if ( newKeyin.size() >=2 )
                  {
                       getLen =2;
                  }
                  else if ( newKeyin.size() >=1 )
                  {
                       getLen =1;
                  }
                  else
                  {
                       break;
                  }

                  cutLen =getLen;

                  wstring
                  getKeyin =newKeyin;
                  getKeyin.resize(getLen);

                  CandidateCH
                  CandVoca;
                  CandVoca.alloc(Number, MAX_CHI_WORD_LENGTH);

                  int
                  GetItem =Number;

                  switch ( FirstGetLen )
                  {
                            case 3:
                            case 4:
                                 //如果第一個已經是四字詞, 後面只取10個來Append就可以,不然字越多越慢
                                 GetItem =10;
                                 break;
                  }

                  //為了加速, 第一次只需要成功取出一個就可以
                  int
                  count =IQQI_GetCandidatesEx(_IME, (wchar_t*)getKeyin.c_str(),
                                              FirstCandidateIsUserTyping, Type, Begin,
                                              1,
                                              CandVoca.pointer(), false);

                  if ( count >0 )
                  {
                       //有找到, 才取出實際所有的候選字
                       count =IQQI_GetCandidatesEx(_IME, (wchar_t*)getKeyin.c_str(),
                                                   FirstCandidateIsUserTyping, Type, Begin,
                                                   GetItem,
                                                   CandVoca.pointer(), false);
                  }

                  if ( count >0 && FirstGetLen ==0)
                  {
                      FirstGetLen =getLen;
                  }

                  for ( int i =0; i <count; i++ )
                  {
                        if ( candIndex >Number -1 )
                             break;

                        wstring
                        wsV =CandVoca.record(i);

                        int
                        posKey =wsV.find(L":");

                        if ( posKey >0 )
                        {
                             wsV.resize(posKey);
                        }

                        if ( i ==0 )
                        {
                             if ( wsFirstCandidate.size() ==0 )
                             {
                                  wsFirstCandidate =wsV.c_str();

                                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                  KIKA_LOGD("%ls, add cand =%ls, final =%ls\r\n", getKeyin.c_str(), wsV.c_str(), wsFirstCandidate.c_str());
                                  #elif defined(__TEST)
                                  printf("%ls, add cand =%ls, final =%ls\r\n", getKeyin.c_str(), wsV.c_str(), wsFirstCandidate.c_str());
                                  #endif
                             }
                             else
                             {
                                  wsFirstCandidate +=wsV.c_str();

                                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                  KIKA_LOGD("%ls, append cand =%ls, final =%ls\r\n", getKeyin.c_str(), wsV.c_str(), wsFirstCandidate.c_str());
                                  #elif defined(__TEST)
                                  printf("%ls, append cand =%ls, final =%ls\r\n", getKeyin.c_str(), wsV.c_str(), wsFirstCandidate.c_str());
                                  #endif

                                  break;
                             }
                        }
                        wcscpy(Candidates[candIndex++], wsV.c_str());

                        #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                        KIKA_LOGD("curr keyin(%ls) GetCandidatesEx, getTotal =%d, cand =%ls\r\n", newKeyin.c_str(), candIndex, Candidates[candIndex -1]);
                        #elif defined(__TEST)
                        printf("curr keyin(%ls) GetCandidatesEx, getTotal =%d, cand =%ls\r\n", newKeyin.c_str(), candIndex, Candidates[candIndex -1]);
                        #endif
                  }
                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                  KIKA_LOGD("curr keyin(%ls) GetCandidatesEx count =%d\r\n", newKeyin.c_str(), count);
                  #elif defined(__TEST)
                  printf("curr keyin(%ls) GetCandidatesEx, count =%d\r\n", newKeyin.c_str(), count);
                  #endif

                  if ( candIndex >Number )
                       break;

                  if ( count ==0 )
                  {
                       if ( newKeyin.size() ==0 )
                       {
                            //再沒有剩下的字元可進行候選字
                            break;
                       }

                       wstring
                       newKeyinTmp;

                       if ( newKeyin.size() ==1 )
                       {
                            //沒找到任何長度Match的Qwerty詞
                            newKeyinTmp =newKeyin.substr(0, 1);

                            CandidateCH
                            CandVocaN;
                            CandVocaN.alloc(Number, MAX_CHI_WORD_LENGTH);

                            //只剩一個字元, 取用這個字來搜尋
                            count =IQQI_GetCandidatesEx(_IME, (wchar_t*)newKeyinTmp.c_str(),
                                                        FirstCandidateIsUserTyping, Type, Begin, 1,
                                                        CandVocaN.pointer(), false);

                            if ( count ==0 )
                            {
                                if ( candIndex >Number -1 )
                                     break;

                                //如果還是找不到, 有可能是非字根字元? 直接加入此單字
                                if ( wsFirstCandidate.size() ==0 )
                                {
                                       break;
                                }
                                else
                                {
                                     wstring
                                     wsV =CandVocaN.record(0);

                                     int
                                     posKey =wsV.find(L":");

                                     if ( posKey >0 )
                                     {
                                          wsV.resize(posKey);
                                     }
                                     wsFirstCandidate +=wsV;

                                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                     KIKA_LOGD("%ls, append cand =%ls, final =%ls\r\n", getKeyin.c_str(), wsV.c_str(), wsFirstCandidate.c_str());
                                     #elif defined(__TEST)
                                     printf("%ls, append cand =%ls, final =%ls\r\n", getKeyin.c_str(), wsV.c_str(), wsFirstCandidate.c_str());
                                     #endif

                                     break;
                                }
                                //
                            }
                            else
                            {
                                for ( int i =0; i <count; i++ )
                                {
                                      if ( candIndex >Number -1 )
                                           break;

                                      wstring
                                      wsV =CandVoca.record(i);

                                      int
                                      posKey =wsV.find(L":");

                                      if ( posKey >0 )
                                      {
                                           wsV.resize(posKey);
                                      }

                                      if ( i ==0 )
                                      {
                                           if ( wsFirstCandidate.size() ==0 )
                                           {
                                                wcscpy(Candidates[candIndex++], wsV.c_str());
                                                wsFirstCandidate =wsV.c_str();
                                           }
                                           else
                                           {
                                                wsFirstCandidate +=wsV.c_str();
                                                break;
                                           }
                                      }
                                }
                                #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                KIKA_LOGD("curr keyin(%ls) GetCandidatesEx, getTotal =%d, cand =%ls\r\n", newKeyin.c_str(), candIndex, Candidates[candIndex -1]);
                                #elif defined(__TEST)
                                printf("curr keyin(%ls) GetCandidatesEx, getTotal =%d, cand =%ls\r\n", newKeyin.c_str(), candIndex, Candidates[candIndex -1]);
                                #endif
                            }
                       }

                       //只需要減一個單位長度來找下一個
                       newKeyinTmp =newKeyin.substr(0, newKeyin.size() - 1);
                       newKeyin =newKeyinTmp;

                       #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                       KIKA_LOGD("next keyin(%ls) len =%d\r\n", newKeyin.c_str(), (int)newKeyin.size());
                       #elif defined(__TEST)
                       printf("next keyin(%ls) len =%d\r\n", newKeyin.c_str(), (int)newKeyin.size());
                       #endif
                       continue;
                  }

                  if ( oldKeyin.size() ==0 )
                       break;

                  newKeyin =oldKeyin.substr(cutLen, oldKeyin.size() - cutLen);
                  oldKeyin =newKeyin;

                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                  KIKA_LOGD("next keyin(%ls) len =%d\r\n", newKeyin.c_str(), (int)newKeyin.size());
                  #elif defined(__TEST)
                  printf("next keyin(%ls) len =%d\r\n", newKeyin.c_str(), (int)newKeyin.size());
                  #endif
          }
          if ( wsFirstCandidate.size() >0 )
          {
               wcscpy(Candidates[0], wsFirstCandidate.c_str());
          }

          //最後再加上第一字詞的所有候選字
          wstring
          newKeyinTmp;
          newKeyinTmp =KeyCodeEx[0];

          CandidateCH
          CandVoca;
          CandVoca.alloc(Number, MAX_CHI_WORD_LENGTH);

          int
          count =IQQI_GetCandidatesEx(_IME, (wchar_t*)newKeyinTmp.c_str(),
                                      FirstCandidateIsUserTyping, Type, Begin, Number,
                                      CandVoca.pointer(), true);

          for ( int i =0; i <count; i++ )
          {
                if ( candIndex >Number -1 )
                     break;

                wstring
                wsV =CandVoca.record(i);

                int
                posKey =wsV.find(L":");

                if ( posKey >0 )
                {
                     wsV.resize(posKey);
                }

                wcscpy(Candidates[candIndex++], wsV.c_str());

                #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                KIKA_LOGD("word keyin(%ls) GetCandidatesEx, getTotal =%d, cand =%ls\r\n", newKeyin.c_str(), candIndex, Candidates[candIndex -1]);
                #elif defined(__TEST)
                printf("word keyin(%ls) GetCandidatesEx, getTotal =%d, cand =%ls\r\n", newKeyin.c_str(), candIndex, Candidates[candIndex -1]);
                #endif
          }

          #else

          return IQQI_GetCandidatesEx(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, false);

          #endif
     }

     gCandidateCount =candIndex;

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("IQQI_GetCandidatesByHeadWord (total =%d)\r\n", gCandidateCount);
     #elif defined(__TEST)
     printf("IQQI_GetCandidatesByHeadWord (total =%d)\r\n", gCandidateCount);
     #endif

     return gCandidateCount;

     #else

     return IQQI_GetCandidatesEx(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, false);

     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_GetCandidates(int _IME, wchar_t* KeyCodeEx, bool FirstCandidateIsUserTyping, int Type, int Begin, int Number, wchar_t** Candidates, wchar_t** Composings)
{
#if defined(__IQQI_PY)
    if (_IME == IQQI_IME_Chinese_CN) {
        if (gKeybdMode == for_T9)
            return IQQI_GetCandidates_KT9(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, Composings);
        else
            return IQQI_GetCandidates_Py(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates);
    }
#endif
     #if defined(__FOR_HUAWEI)
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_TW:
                   if ( gKeybdMode ==for_T9 )
                        return IQQI_GetCandidatesByMixT9Key(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates);
                   else
                        return IQQI_GetCandidatesByHeadWord(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates);
                   break;

              default:
                   return IQQI_GetCandidatesEx(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates, false);
     }
     #else
     return IQQI_GetCandidatesEx(_IME, KeyCodeEx, FirstCandidateIsUserTyping, Type, Begin, Number, Candidates);
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_GetIdiomTradCandidates( wchar_t* U16Word, int Begin, int Number, wchar_t** Candidates)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_GetIdiomSimpCandidates( wchar_t* U16Word, int Begin, int Number, wchar_t** Candidates)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_GetInProcessedCode(int Location)
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_GetWaitProcessCode(int Location)
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_AddPhrase( int _IME, wchar_t* KeyCode, wchar_t* Content)
{
    #if defined(__PHRS_DIC)

    wchar_t
    wsKey[256] ={0};

    bool
    retVal =false;

    if ( gKeybdMode ==for_T9 )
    {
         gMap.keyCode2Numb(_IME, KeyCode, wsKey, 256);
         retVal =gPhrs->append(_IME, wsKey, Content);
    }
    else
    {
         retVal =gPhrs->append(_IME, wsKey, Content);
    }

    #if defined(__DEBUG_MSG) || defined(__TEST)
    char aa[256] ={0};
    sprintf(aa, "AddPhrase (%ls, %ls) retVal =%d\r\n", KeyCode, Content, retVal);
    d_pintf(aa);
    #endif

    if ( retVal ==false )
    {
         #if defined(__TEST)
         showErr(retVal);
         #endif

         return ERR_PHRS_ADD_FAIL;
    }

    #if defined(__PHRS_DIC_SAVE)
    gPhrs->save(gPhrsDBPath.c_str());
    #endif

    #if defined(__TEST)
    d_pintf_ex(aa);
    #endif

    #endif

    return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_RemovePhrase( int _IME, wchar_t* KeyCode, wchar_t* Content)
{
     #if defined(__PHRS_DIC)

     wchar_t
     wsKey[256] ={0};

     bool
     retVal =false;

     if ( gKeybdMode ==for_T9 )
     {
          gMap.keyCode2Numb(_IME, Content, wsKey, 256);
          retVal =gPhrs->remove(_IME, wsKey, Content);
     }
     else
     {
          retVal =gPhrs->remove(_IME, wsKey, Content);
     }

     #if defined(__DEBUG_MSG) || defined(__TEST)
     char aa[256] ={0};
     sprintf(aa, "RemovePhrase (%ls, %ls) retVal =%d\r\n", KeyCode, Content, retVal);
     d_pintf(aa);
     #endif

     if ( retVal ==false )
     {
          #if defined(__TEST)
          showErr(retVal);
          #endif

          return ERR_PHRS_REMOVE_FAIL;
     }

     #if defined(__PHRS_DIC_SAVE)
     gPhrs->save(gPhrsDBPath.c_str());
     #endif

     #if defined(__TEST)
     d_pintf_ex(aa);
     #endif

     #endif

     return 0;
}


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::vector<std::string> string_split(std::string s, const char delimiter)
{
     size_t start =0;
     size_t end=s.find_first_of(delimiter);

     std::vector<std::string> output;

     while ( end <= std::string::npos )
     {
         output.emplace_back(s.substr(start, end -start));

         if ( end == std::string::npos )
               break;

         start=end +1;
         end = s.find_first_of(delimiter, start);
     }

     return output;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_LearnWordByTokenEx(int _IME, char* PinyinCodeTable, char* ContentTable, unsigned int Length, bool ForcePriority, bool IsMultiTap)
{
     #if !defined(__FOR_ANDROID)
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_HK:
              case IQQI_IME_Chinese_TW:
                   #if !defined(__USER_DIC_SAVE)
                    #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                    KIKA_LOGD("Kika-ENGINE, Chinese no need to Learn Word !!!");
                    #elif defined(__TEST) || defined(__DEBUG_MSG)
                    char aa[256] ={0};
                    sprintf(aa, "Kika-ENGINE, Chinese no need to Learn Word !!!");
                     #if defined(__TEST)
                    d_pintf_ex(aa);
                     #else
                    d_pintf(aa);
                     #endif
                    #endif
                   #endif
                   return 0;
     }
     #endif

     #if defined(__USER_DIC)

     gUserFile.check_UserDB_Limit();
     gPrioFile.check_UserDB_Limit();

     bool
     isT9 =setT9(_IME);

     if ( isT9 ==false )
          IQQI_FreeKika();

     const char
     CH =ContentTable[Length -1];

     if ( CH !=KEY_TOKEN )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, last character [0x%x] of ContentTable (Len =%d) must be '|'\r\n", CH, Length);
          #elif defined(__TEST) || defined(__DEBUG_MSG)
          char aa[256] ={0};
          sprintf(aa, "LearnWordByToken last character [0x%x] of ContentTable (Len =%d) must be '|'\r\n", CH, Length);
           #if defined(__TEST)
           d_pintf_ex(aa);
           #else
           d_pintf(aa);
           #endif
          #endif

          return ERR_DICT_INVALID_LAST_CH;
     }

     char
     IME_ErrorList[32] = {0};

     int
     retVal =0;

     if ( isT9 ==false )
     {
          #if !defined(__NO_KIKA_IME)
          // switch to Custom User base at first
          retVal =gUser.Initial(_IME, gUserDBPath.c_str(), IME_ErrorList, true);
          #endif
     }

     if ( retVal ==0 )
     {
          for ( long b =0, e =0; b <Length; e++ ) //auto x :tokens )
          {
                if ( ContentTable[e] ==KEY_TOKEN ) // |
                {
                     try
                     {
                          int sLen =(e -b);

                          #if defined(__FOR_ANDROID)
                          if ( sLen ==1 )
                          {
                               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                               KIKA_LOGD("Kika-ENGINE, Len=1 Word ignore to Learn");
                               #endif
                               continue;
                          }
                          #endif

                          if ( sLen >MAX_LEARN_WORD_LEN )
                          {
                               return ERR_DICT_OUTOF_WORD_LEN;
                          }

                          #if defined(__TEST) || defined(__DEBUG_MSG)
                          {
                              char aa[256] ={0};
                              sprintf(aa, "LearnWordByToken b=%ld, e=%ld, sLen =%d\r\n", b, e, sLen);
                               #if defined(__TEST)
                              d_pintf_ex(aa);
                               #else
                              d_pintf(aa);
                               #endif
                          }
                          #endif

                          char
                          sIn[MAX_LEARN_WORD_LEN +1] ={0};

                          memcpy(sIn, ContentTable +b, sLen);
                          sIn[sLen] =0;

                          b =e +1;

                          #if defined(__TEST) || defined(__DEBUG_MSG)
                          {
                              char aa[256] ={0};
                              sprintf(aa, "LearnWordByToken get %s to Learn\r\n", sIn);
                               #if defined(__TEST)
                              d_pintf_ex(aa);
                               #else
                              d_pintf(aa);
                               #endif
                          }
                          #endif

                          if ( sIn[0] =='[' && sIn[sLen -1] ==']' )
                          {
                               sIn[sLen -1] =0;
                               memcpy(sIn, sIn +1, sLen -1);

                               char aa[256] ={0};
                               sprintf(aa, "sIn =%s\r\n", sIn);
                                #if defined(__TEST)
                               d_pintf_ex(aa);
                                #else
                               d_pintf(aa);
                                #endif
                          }

                          wstring
                          wContent =utf8_to_utf16(sIn);

                          bool
                          chkValid =(isT9 ==true);

                          #if defined(__CHECK_INVALID_LEARN_WORD)
                          chkValid =true;
                          #endif

                          wchar_t
                          wsRetKey[MAX_INPUT_KEY] ={0};

                          if ( chkValid ==true )
                          {
                               //檢查之前是否有記錄到學習這筆( IME+Content )
                               bool
                               found =gUserFile.findKeys(_IME, wContent, wsRetKey, MAX_INPUT_KEY);

                               if ( found ==false )
                               {
                                    //沒有這筆學習記錄( IME+Content )
                                    wstring
                                    wContentEx;

                                    for ( auto c: wContent )
                                    {
                                          wContentEx +=static_cast<wchar_t>(std::towlower(c));
                                    }

                                    #if defined(__FOR_ANDROID)
                                    switch ( _IME )
                                    {
                                             case IQQI_IME_Changjie:
                                             case IQQI_IME_Chinese_CN:
                                             case IQQI_IME_Chinese_HK:
                                             case IQQI_IME_Chinese_TW:
                                                  // temperary not support in Token Mode
                                                  break;

                                             default:
                                                  gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);
                                                  gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);
                                    }
                                    #else

                                    gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);
                                    gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);

                                    #endif
                               }

                               #if defined(__TEST) || defined(__DEBUG_MSG)
                               {
                                   char aa[MAX_INPUT_KEY] ={0};
                                   sprintf(aa, "LearnWordByToken Add Key =%ls, Content =%ls\r\n",
                                                                  wsRetKey, wContent.c_str());
                                   #if defined(__TEST)
                                   d_pintf_ex(aa);
                                   #else
                                   d_pintf(aa);
                                   #endif
                               }
                               #endif

                               if ( wcslen(wsRetKey) ==0 )
                               {
                                    retVal =ERR_DICT_INVALID_KEY_LEN_0;
                                    #if defined(__TEST)
                                    showErr(retVal);
                                    #endif
                                    break;
                               }

                               //-----------------------------------------------
                               // don't 另外處理
                               //-----------------------------------------------
                               int
                               c_cut  =std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_QUOTATION );
                               //c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_MINUS );
                               #if !defined(__SONIM)
                               c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_0 );
                               c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_2 );
                               #endif

                               if ( wcslen(wsRetKey) !=(wContent.size() -c_cut) )
                               {
                                    retVal =ERR_DICT_KEY_WORD_LEN_NOT_SAME;
                                    #if defined(__TEST)
                                    showErr(retVal);
                                    #endif
                                    break;
                               }
                          }

                          int
                          stamp  =gImeX_Prio->getTimeStamp();
                          stamp +=gImeX_Prio->total_u();


                          //---------------------------------------------------
                          // 內建的詞才學
                          //---------------------------------------------------
                          #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                          int
                          IsInnerVoca =0; //gVoca_InnerX.isExist(wContent.c_str());

                          switch ( _IME )
                          {
                                   case IQQI_IME_Changjie:
                                   case IQQI_IME_Chinese_CN:
                                   case IQQI_IME_Chinese_HK:
                                   case IQQI_IME_Chinese_TW:
                                        IsInnerVoca =1;
                                        break;

                                   default:
                                        IsInnerVoca =gVoca_InnerX.isExist(wContent.c_str());
                                        break;
                          }

                          //if ( IsInnerVoca ==1 )
                          if ( ForcePriority ==true )
                          {
                               gImeX_Prio->checkLimit_u(LIMIT_PRIORITY_COUNT);
                               if ( gImeX_Prio->append_u(_IME, wsRetKey, wContent.c_str(), stamp) ==false )
                               {
                               }
                          }
                          #endif
                          //---------------------------------------------------

                          if ( isT9 ==false )
                          {
                               #if !defined(__NO_KIKA_IME)
                               // save to gUserFile
                               retVal =gUser.LearnWord(_IME, L"", wContent.c_str());
                               #endif
                          }
                          else
                          {
                               switch ( _IME )
                               {
                                        #if !defined(__FOR_HUAWEI)
                                        case IQQI_IME_Thai:
                                        case IQQI_IME_Vietnamese:
                                        #endif
                                        #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                                        case IQQI_IME_Changjie:
                                        case IQQI_IME_Chinese_CN:
                                        case IQQI_IME_Chinese_HK:
                                        case IQQI_IME_Chinese_TW:
                                        #endif
                                             if ( gKeybdMode ==for_T9 )
                                             {
                                                  #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                                                  if ( IsInnerVoca ==0 )
                                                  #endif
                                                  {
                                                      if ( gImeX_User->append_u(_IME, wsRetKey, wContent.c_str(), stamp) ==false )
                                                      {
                                                      }
                                                  }

                                                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                                  KIKA_LOGD("Kika-ENGINE, af append user sz =%d\r\n", gImeX_User->total_u());
                                                  #endif
                                             }
                                             break;

                                        default:
                                             retVal =0;
                                             break;
                               }
                          }

                          if ( retVal !=0 )
                          {
                               #if defined(__TEST) || defined(__DEBUG_MSG)
                               char aa[256] ={0};
                               sprintf(aa, "LearnWordByToken error, retVal =%d\r\n", retVal);
                                #if defined(__TEST)
                               d_pintf_ex(aa);
                                #else
                               d_pintf(aa);
                                #endif
                               #endif
                               break;
                          }
                          else
                          {
                               #if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                               gVoca_InnerX.append(wContent.c_str());
                               #endif

                               #if !defined(__USER_DIC_SAVE)

                               // 先刪除原有的

                               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)

                               gUserFile.remove_ex(_IME, (wchar_t *)L"m", wContent.c_str());
                               gUserFile.remove_ex(_IME, (wchar_t *)L"u", wContent.c_str());

                               #else

                               gUserFile.remove(_IME, (wchar_t *)L"m", wContent.c_str());
                               gUserFile.remove(_IME, (wchar_t *)L"u", wContent.c_str());

                               #endif

                               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                               KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordByTokenEx, append_keys 1 [%s], imeID =%d, append_keys (%ls, %ls)\r\n",
                               gUserFile.get_fname().c_str(), _IME, wsRetKey, wContent.c_str());
                               #endif

                               // 再新增
                               gUserFile.append_keys(_IME, IsMultiTap ? L"m": L"u", wsRetKey, wContent.c_str());

                               #endif
                          }
                     }
                     catch ( ... )
                     {
                          #if defined(__TEST) || defined(__DEBUG_MSG)
                          char aa[256] ={0};
                          sprintf(aa, "LearnWordByToken exception error\r\n");
                           #if defined(__TEST)
                          d_pintf_ex(aa);
                           #else
                          d_pintf(aa);
                           #endif
                          #endif

                          return ERR_DICT_INVALID_PARAM;
                     }
                }
          }

          #if !defined(__USER_DIC_SAVE)
          if ( retVal ==0 )
          {
               gUserFile.updateAndSave(false);

               // 是否強迫加入第一優先順序的檔案?
               if ( ForcePriority ==true )
                    gPrioFile.updateAndSave(true);
          }
          #endif

          #if defined(__TEST) || defined(__DEBUG_MSG)
          {
              char aa2[256] ={0};
              sprintf(aa2, "LearnWordByToken Done\r\n");
              #if defined(__TEST)
              d_pintf_ex(aa2);
              #else
              d_pintf(aa2);
              #endif
          }
          #endif
     }
     else
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, LearnWordByToken Initial error retVal =%d\r\n", retVal);
          #elif defined(__TEST) || defined(__DEBUG_MSG)
          char aa[256] ={0};
          sprintf(aa, "LearnWordByToken Initial error retVal =%d\r\n", retVal);
           #if defined(__TEST)
          d_pintf_ex(aa);
           #else
          d_pintf(aa);
           #endif
          #endif
     }

     #if !defined(__NO_KIKA_IME)

     if ( isT9 ==false )
     {
          gUser.Free();
     }
     #endif

     return retVal;

     #else
     return 0;

     #endif
}


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_LearnWordByToken(int _IME, char* PinyinCodeTable, char* ContentTable, unsigned int Length)
{
     return IQQI_LearnWordByTokenEx(_IME, PinyinCodeTable, ContentTable, Length, true, false);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_LearnWordEx(int _IME, wchar_t* KeyCode, wchar_t* Content, bool ForcePriority, bool IsMultiTap)
{
     #if !defined(__FOR_ANDROID)
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_HK:
              case IQQI_IME_Chinese_TW:
                   #if !defined(__USER_DIC_SAVE)
                    #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                    KIKA_LOGD("Kika-ENGINE, Chinese no need to Learn Word !!!");
                    #elif defined(__TEST) || defined(__DEBUG_MSG)
                    char aa[256] ={0};
                    sprintf(aa, "Kika-ENGINE, Chinese no need to Learn Word !!!");
                     #if defined(__TEST)
                    d_pintf_ex(aa);
                     #else
                    d_pintf(aa);
                     #endif
                    #endif
                   #endif
                   return 0;
     }
     #endif

     #if defined(__FOR_ANDROID)
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_HK:
              case IQQI_IME_Chinese_TW:
                   break;

              default:
                   if ( wcslen(Content) ==1 )
                   {
                        #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                        KIKA_LOGD("Kika-ENGINE, Len=1 Word ignore to Learn");
                        #endif
                        return 0;
                   }
     }
     #endif

     #if defined(__USER_DIC)

     gUserFile.check_UserDB_Limit();
     gPrioFile.check_UserDB_Limit();

     bool
     isT9 =setT9(_IME);

     if ( isT9 ==false )
          IQQI_FreeKika();

     char
     IME_ErrorList[32] = {0};

     int
     retVal =0;

     if ( isT9 ==false )
     {
          #if !defined(__NO_KIKA_IME)
          // first, switch to Custom User base ( in here, new Voca not Learn yet!)
          retVal =gUser.Initial(_IME, gUserDBPath.c_str(), IME_ErrorList, true);
          #endif
     }

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     {
           string sKeys =wchar_to_UTF8(KeyCode);
           string sVoca =WstringToString(Content);
           KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEx, _IME =%d, Add KeyCode =%s, Content =%s\r\n", _IME, sKeys.c_str(), sVoca.c_str());
     }
     #endif

     if ( retVal ==0 )
     {
          bool
          chkValid =(isT9 ==true);

          #if defined(__CHECK_INVALID_LEARN_WORD)
          chkValid =true;
          #endif

          wstring
          wContent =Content;

          wchar_t
          wsRetKey[MAX_INPUT_KEY] ={0};

          if ( chkValid ==true )
          {
               //檢查之前是否有記錄學習到這筆( IME+Content )
               bool
               found =gUserFile.findKeys(_IME, Content, wsRetKey, MAX_INPUT_KEY);

               if ( found ==false )
               {
                    //沒有這筆學習記錄( IME+Content )
                    wstring
                    wContentEx;

                    for ( auto c: wContent )
                    {
                          wContentEx +=static_cast<wchar_t>(std::towlower(c));
                    }

                    #if defined(__FOR_ANDROID)
                    switch ( _IME )
                    {
                             case IQQI_IME_Chinese_HK:
                                  if ( wcslen(KeyCode) >MAX_INPUT_KEY )
                                       wcsncpy(wsRetKey, KeyCode, MAX_INPUT_KEY);
                                  else
                                       wcscpy(wsRetKey, KeyCode);
                                  break;

                             case IQQI_IME_Changjie:
                             case IQQI_IME_Chinese_CN:
                             case IQQI_IME_Chinese_TW:
                                  if (  KeyCode[0] >=0 && KeyCode[0] <='9' )
                                  {
                                        if ( wcslen(KeyCode) >MAX_INPUT_KEY )
                                             wcsncpy(wsRetKey, KeyCode, MAX_INPUT_KEY);
                                        else
                                             wcscpy(wsRetKey, KeyCode);
                                  }
                                  else
                                  {
                                        gMap.keyCode2NumbEx(_IME, KeyCode, wsRetKey, MAX_INPUT_KEY);
                                        gMap.keyCode2NumbEx(IQQI_IME_English_US, KeyCode, wsRetKey, MAX_INPUT_KEY);
                                  }
                                  break;

                             #if defined(__I_WITH_DORO)
                             case IQQI_IME_Romanian:
                                  //2020/09/25
                                  if ( wcslen(KeyCode) ==0 )
                                  {
                                       gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);
                                       break;
                                  }
                             #endif

                             default:
                                  //2020/09/25
                                  if ( wcslen(KeyCode) ==0 )
                                  {
                                       gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);
                                       gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);
                                  }
                                  else
                                  {
                                       #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                       {
                                            string sKeys =wchar_to_UTF8(KeyCode);
                                            string sVoca =WstringToString(wContent);

                                            KIKA_LOGD("Kika-ENGINE, 000000 LearnWord already has KeyCode =%s, Content =%s\r\n",
                                            sKeys.c_str(), sVoca.c_str());
                                       }
                                       #endif
                                  }
                    }
                    #else

                    gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);
                    gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsRetKey, MAX_INPUT_KEY);

                    #endif
               }

               {
                   #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                   {
                   string sKeys =wchar_to_UTF8(wsRetKey);
                   string sVoca =WstringToString(wContent);

                   KIKA_LOGD("Kika-ENGINE, LearnWord Add Key =%s, Content =%s\r\n", sKeys.c_str(), sVoca.c_str());

                    #if defined(__DEBUG_MSG2)
                   wchar_t aa[MAX_INPUT_KEY] ={0};
                   wcscpy(aa, L"Kika-ENGINE, LearnWord Add Key KeyCode =");

                   for ( size_t i =0; i <wContent.size(); i++ )
                   {
                         wchar_t aa2[MAX_INPUT_KEY] ={0};
                         swprintf(aa2, MAX_INPUT_KEY, L"0x%x, ", wContent[i]);

                         wcscat(aa, aa2);
                   }
                   KIKA_LOGD("Kika-ENGINE, %ls", aa);
                    #endif
                   }

                   #elif defined(__TEST) || defined(__DEBUG_MSG)
                   char aa[MAX_INPUT_KEY] ={0};
                   sprintf(aa, "LearnWord Add Key =%ls, Content =%ls\r\n", wsRetKey, wContent.c_str());

                    #if defined(__TEST)
                   d_pintf_ex(aa);
                    #else
                   d_pintf(aa);
                    #endif
                   #endif
               }

               if ( wcslen(wsRetKey) ==0 )
               {
                    retVal =ERR_DICT_INVALID_KEY_LEN_0;
                    #if defined(__TEST)
                    showErr(retVal);
                    #endif
                    return retVal;
               }

               //--------------------------------------------------------------
               // don't 另外處理
               //--------------------------------------------------------------
               int
               c_cut  =std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_QUOTATION );
               //c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_MINUS );
               #if !defined(__SONIM)
               c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_0 );
               c_cut +=std::count( wContent.begin(), wContent.end(), KEY_EXTRA_ALLOW_CH_2 );
               #endif

               if ( wcslen(wsRetKey) !=(wContent.size() -c_cut) )
               {
                    switch ( _IME )
                    {
                             case IQQI_IME_Changjie:
                             case IQQI_IME_Chinese_CN:
                             case IQQI_IME_Chinese_HK:
                             case IQQI_IME_Chinese_TW:
                                  break;

                             default:
                                  retVal =ERR_DICT_KEY_WORD_LEN_NOT_SAME;
                                  #if defined(__TEST)
                                  showErr(retVal);
                                  #endif
                                  return retVal;
                    }
               }
          }

          int
          stamp  =gImeX_Prio->getTimeStamp();
          stamp +=gImeX_Prio->total_u();

          //-------------------------------------------------------------------
          // 非內建的詞才學
          //-------------------------------------------------------------------
          #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
          int
          IsInnerVoca =0;

          switch ( _IME )
          {
                   case IQQI_IME_Changjie:
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_HK:
                   case IQQI_IME_Chinese_TW:
                        IsInnerVoca =1;
                        break;

                   default:
                        //IsInnerVoca =gVoca_InnerX.isExist(wContent.c_str());
                        IsInnerVoca =IQQI_WordExist(_IME, wContent.c_str());
          }

          //-------------------------------------------------------------------
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, IsInnerVoca =%d\r\n", IsInnerVoca);
          #endif

          //if ( IsInnerVoca ==1 )
          if ( ForcePriority ==1 )
          {
               gImeX_Prio->checkLimit_u(LIMIT_PRIORITY_COUNT);
               if ( gImeX_Prio->append_u(_IME, wsRetKey, wContent.c_str(), stamp) ==false )
               {
                    #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                    KIKA_LOGD("Kika-ENGINE, _IME =%d, LearnWord append_u prio fail !!!\r\n", _IME);
                    #endif
               }
          }
          #endif
          //-------------------------------------------------------------------

          if ( isT9 ==false )
          {
               #if !defined(__NO_KIKA_IME)
               // 如果不存在 Volatile 站典
               if ( gPrioFile.isExist_Pri(_IME, wsRetKey, wContent.c_str()) ==false )
               {
                    // save to CustomUserFile
                    retVal =gUser.LearnWord(_IME, L"", wContent.c_str());
               }
               #endif
          }
          else
          {
               switch ( _IME )
               {
                        #if !defined(__FOR_HUAWEI)
                        case IQQI_IME_Thai:
                        case IQQI_IME_Vietnamese:
                        #endif
                        #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                        case IQQI_IME_Changjie:
                        case IQQI_IME_Chinese_CN:
                        case IQQI_IME_Chinese_HK:
                        case IQQI_IME_Chinese_TW:
                        #endif
                             if ( gKeybdMode ==for_T9 )
                             {
                                  #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                                  if ( IsInnerVoca ==0 )
                                  #endif
                                  {
                                       #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                                       {
                                             string svoca =wchar_to_UTF8(wsRetKey);
                                             KIKA_LOGD("Kika-ENGINE, _IME =%d, append new keys k =%s\r\n", _IME, svoca.c_str());
                                             svoca =wchar_to_UTF8(wContent.c_str());
                                             KIKA_LOGD("Kika-ENGINE, _IME =%d, append new voca =%ls\r\n", _IME, wContent.c_str());
                                       }
                                       #endif

                                       // 如果不存在 Volatile 站典
                                       if ( gPrioFile.isExist_Pri(_IME, wsRetKey, wContent.c_str()) ==false )
                                       {
                                            if ( gImeX_User->append_u(_IME, wsRetKey, wContent.c_str(), stamp) ==false )
                                            {
                                                 #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                                 KIKA_LOGD("Kika-ENGINE, _IME =%d, LearnWord append_u user fail !!!\r\n", _IME);
                                                 #endif
                                            }
                                       }
                                       else
                                       {
                                            #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                                            string svoca =wchar_to_UTF8(wContent.c_str());
                                            KIKA_LOGD("Kika-ENGINE, _IME =%d, append fail, [ %s ] already exist in .PRI\r\n", _IME, svoca.c_str());
                                            #endif // defined
                                       }
                                  }
                                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                                  KIKA_LOGD("Kika-ENGINE, _IME =%d, af append user sz =%d\r\n", _IME, gImeX_User->total_u());
                                  #endif
                             }
                             break;

                        default:
                             retVal =0;
                             break;
                }
           }

           if ( retVal !=0 )
           {
                #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                KIKA_LOGD("Kika-ENGINE, _IME =%d, LearnWord retVal =%d\r\n", _IME, retVal);
                #elif defined(__TEST) || defined(__DEBUG_MSG)
                char aa[256] ={0};
                sprintf(aa, "Kika-ENGINE, _IME =%d, LearnWord retVal =%d\r\n", _IME, retVal);
                 #if defined(__TEST)
                d_pintf_ex(aa);
                 #else
                d_pintf(aa);
                 #endif
                #endif
           }
           else
           {
                //-------------------------------------------------------------
                #if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                gVoca_InnerX.append(Content);

                  #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                  {
                  string
                  sUTF8_Inner =wchar_to_UTF8(Content);
                  KIKA_LOGD("Kika-ENGINE, _IME =%d, IQQI_LearnWordEx[Voca_Inner], append_u %s\r\n", _IME, sUTF8_Inner.c_str());
                  }
                  #endif

                #endif

                //-------------------------------------------------------------
                #if !defined(__USER_DIC_SAVE)

                  #if defined(__FOR_ANDROID) || defined(__ANDROID_APP)

                if ( IsInnerVoca ==0 )
                {
                     // 非內建的詞才學, 先刪除原有的

                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)

                     gUserFile.remove_ex(_IME, (wchar_t *)L"m", Content);
                     gUserFile.remove_ex(_IME, (wchar_t *)L"u", Content);

                     #else

                     gUserFile.remove(_IME, (wchar_t *)L"m", Content);
                     gUserFile.remove(_IME, (wchar_t *)L"u", Content);

                     #endif

                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                     {
                         string sUTF8_Keys =wchar_to_UTF8(wsRetKey);
                         string sUTF8_Word =wchar_to_UTF8(Content);

                         KIKA_LOGD("Kika-ENGINE, ==========UserFile====================");
                         KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEx, _IME =%d, append f =%s\r\n",  _IME, gUserFile.get_fname().c_str());
                         KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEx, _IME =%d, append_u =[%c], %s, %s\r\n", _IME, IsMultiTap ?L'm': L'u', sUTF8_Keys.c_str(), sUTF8_Word.c_str());
                         KIKA_LOGD("Kika-ENGINE, ======================================");
                     }
                     #endif // defined

                     // 再新增
                     gUserFile.append_keys(_IME, (wchar_t *)IsMultiTap ? L"m": L"u", wsRetKey, Content);

                }
                  #endif

                #endif

                #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEx, _IME =%d, ForcePriority =%d\r\n", _IME, ForcePriority);
                #endif

                //-------------------------------------------------------------
                {
                     // 是否強迫加入第一優先順序的檔案?
                     if ( ForcePriority ==true )
                     {
                          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                          gPrioFile.append_ex(_IME, (wchar_t *)L"", Content);
                          #else
                          gPrioFile.append(_IME, (wchar_t *)L"", Content);
                          #endif

                          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                          {
                              string
                              sUTF8_Keys =wchar_to_UTF8(wsRetKey),
                              sUTF8_Word =wchar_to_UTF8(Content);

                              KIKA_LOGD("Kika-ENGINE, ==========PrioFile====================");
                              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEx, _IME =%d, append f =%s\r\n",  _IME, gPrioFile.get_fname().c_str());
                              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEx, _IME =%d, append_u =[%c], %s, %s\r\n", _IME, IsMultiTap ?L'm': L'u', sUTF8_Keys.c_str(), sUTF8_Word.c_str());
                              KIKA_LOGD("Kika-ENGINE, ======================================");
                          }
                          #endif
                     }
                }

                #if !defined(__USER_DIC_SAVE)

                if ( retVal ==0 )
                {
                     gUserFile.updateAndSave(false);
                     gPrioFile.updateAndSave(true);
                }
                #endif
           }
     }
     else
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, LearnWordEx, _IME =%d, Initial error retVal =%d\r\n", _IME, retVal);
          #elif defined(__TEST) || defined(__DEBUG_MSG)
          char aa[256] ={0};
          sprintf(aa, "Kika-ENGINE, LearnWordEx, _IME =%d, Initial error retVal =%d\r\n", _IME, retVal);
           #if defined(__TEST)
          d_pintf_ex(aa);
           #else
          d_pintf(aa);
           #endif
          #endif
     }

     #if !defined(__NO_KIKA_IME)
     if ( isT9 ==false )
          gUser.Free();
     #endif

     return retVal;

     #else
     return 0;

     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_DeleLearnedWord( int _IME, wchar_t* KeyCode, wchar_t* Content)
{
     #if !defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_HK:
              case IQQI_IME_Chinese_TW:
                   ///break;
                   return 0;
     }
     #endif

     #if defined(__USER_DIC)
     bool
     isT9 =setT9(_IME);

     if ( isT9 ==false )
          IQQI_FreeKika();

     char
     IME_ErrorList[32] = {0};

     int
     retVal =0;

     #if defined(__DEBUG_MSG) || defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, b4 IQQI_DeleLearnedWord, _IME =%d, gImeX_User total =%d\r\n", _IME, gImeX_User->total_u());
     KIKA_LOGD("Kika-ENGINE, b4 IQQI_DeleLearnedWord, _IME =%d, gImeX_Prio total =%d\r\n", _IME, gImeX_Prio->total_u());
     #endif
     if ( isT9 ==false )
     {
          #if !defined(__NO_KIKA_IME)
          retVal =gUser.Initial(_IME, gUserDBPath.c_str(), IME_ErrorList, true);
          #endif
     }

     if ( retVal ==0 )
     {
          wchar_t
          wsKey[MAX_INPUT_KEY] ={0};

          wstring
          wContent =Content;

          wstring
          wContentEx;

          for ( auto c: wContent )
          {
                wContentEx +=static_cast<wchar_t>(std::towlower(c));
          }

          #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP) || defined(__DEBUG_LEARN_WORD)
          switch ( _IME )
          {
                   case IQQI_IME_Chinese_HK:
                        if ( wcslen(KeyCode) >MAX_INPUT_KEY )
                             wcsncpy(wsKey, KeyCode, MAX_INPUT_KEY);
                        else
                             wcscpy(wsKey, KeyCode);
                        break;

                   case IQQI_IME_Changjie:
                   case IQQI_IME_Chinese_CN:
                   case IQQI_IME_Chinese_TW:
                        if (  KeyCode[0] >=0 && KeyCode[0] <='9' )
                        {
                              if ( wcslen(KeyCode) >MAX_INPUT_KEY )
                                   wcsncpy(wsKey, KeyCode, MAX_INPUT_KEY);
                              else
                                   wcscpy(wsKey, KeyCode);
                        }
                        else
                        {
                              gMap.keyCode2NumbEx(_IME, KeyCode, wsKey, MAX_INPUT_KEY);
                              gMap.keyCode2NumbEx(IQQI_IME_English_US, KeyCode, wsKey, MAX_INPUT_KEY);
                        }
                        break;

                   default:
                        gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
                        gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
          }
          #else

          gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
          gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);

          #endif

          bool
          flag =gImeX_User->remove_u(_IME, wsKey, Content);

          retVal =(flag ==true ? 0: -1);
          if ( isT9 ==false )
          {
               #if !defined(__NO_KIKA_IME)
               retVal =gUser.DeleLearnedWord(_IME, wsKey, Content);
               #endif
          }
          else
          {
               //gImeX_Prio->remove_u(_IME, wsKey, Content);

               // bool
               // flag =gImeX_User->remove_u(_IME, wsKey, Content);

               // retVal =(flag ==true ? 0: -1);
          }

          if ( retVal !=0 )
          {
               #if defined(__DEBUG_MSG) || defined(__DEBUG_LEARN_WORD)
               char aa[MAX_INPUT_KEY] ={0};
               sprintf(aa, "DeleLearnedWord retVal =%d\r\n", retVal);
               d_pintf(aa);
               #endif
          }
          else
          {
               #if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
               gVoca_InnerX.erase(Content);
               #endif

               // delete UserData (同時刪除 'u'(Import) & 'm'(MultiTap))

               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)

               gUserFile.remove_ex(_IME, (wchar_t *)L"m", Content);
               gUserFile.remove_ex(_IME, (wchar_t *)L"u", Content);

               // delete PrioData
               bool r1 =gPrioFile.remove_ex(_IME, wsKey, Content);
               bool r2 =gPrioFile.remove_ex(_IME, (wchar_t *)L"", Content);

               #else

               gUserFile.remove(_IME, (wchar_t *)L"m", Content);
               gUserFile.remove(_IME, (wchar_t *)L"u", Content);

               // delete PrioData
               bool r1 =gPrioFile.remove(_IME, wsKey, Content);
               bool r2 =gPrioFile.remove(_IME, (wchar_t *)L"", Content);

               #endif

               // 2020/04/12
               bool r3 =gImeX_Prio->remove_u(_IME, wsKey, Content);

               #if defined(__DEBUG_MSG) || defined(__DEBUG_LEARN_WORD)
               KIKA_LOGD("Kika-ENGINE, IQQI_DeleLearnedWord, _IME =%d, r1 =%d, r2 =%d, r3 =%d\r\n", _IME, r1, r2, r3);
               #endif
               gUserFile.clearKeys();

               #if !defined(__USER_DIC_SAVE)
               gUserFile.updateAndSave(false);
               gPrioFile.updateAndSave(true);
               #endif
          }
     }
     else
     {
          #if defined(__DEBUG_MSG) || defined(__DEBUG_LEARN_WORD)
          char aa[256] ={0};
          sprintf(aa, "DeleLearnedWord Initial retVal =%d\r\n", retVal);
          d_pintf(aa);
          #endif
     }

     #if defined(__DEBUG_MSG) || defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, af IQQI_DeleLearnedWord, _IME =%d, gImeX_User total =%d\r\n", _IME, gImeX_User->total_u());
     KIKA_LOGD("Kika-ENGINE, af IQQI_DeleLearnedWord, _IME =%d, gImeX_Prio total =%d\r\n", _IME, gImeX_Prio->total_u());
     #endif
     #if !defined(__NO_KIKA_IME)
     if ( isT9 ==false )
          gUser.Free();
     #endif

     return retVal;

     #else
     return 0;

     #endif
}

struct Cmp_LearnWord_User_VOCA
{
       PACKAGES_UI           In;

       Cmp_LearnWord_User_VOCA(PACKAGES_UI _In) : In(_In)
       {
       }

       bool operator()(const PACKAGES_UI& chk) const
       {
            #if defined(__DEBUG_LEARN_WORD2)
            string s1 =wchar_to_UTF8(In.wsPhrs.c_str());
            string s2 =wchar_to_UTF8(chk.wsPhrs.c_str());

            KIKA_LOGD("Kika-ENGINE-Cmp_LearnWord_User_VOCA, sz1 =%d, sz2 =%d\r\n", (int)In.wsPhrs.size(), (int)chk.wsPhrs.size());
            KIKA_LOGD("Kika-ENGINE-Cmp_LearnWord_User_VOCA, ws1 =%s, ws2 =%s\r\n", s1.c_str(), s2.c_str());
            #endif

            if ( In.wsPhrs.size() ==chk.wsPhrs.size() )
            {
                 if ( In.wsPhrs ==chk.wsPhrs )
                 {
                      #if defined(__DEBUG_LEARN_WORD2)
                      KIKA_LOGD("Kika-ENGINE-Cmp_LearnWord_User_VOCA, ws1 =%s, ws2 =%s, OK 1\r\n", s1.c_str(), s2.c_str());
                      #endif
                      return true;
                 }
                 // 2020/08/05
                 //return true;
            }
            #if defined(__DEBUG_LEARN_WORD2)
            KIKA_LOGD("Kika-ENGINE-Cmp_LearnWord_User_VOCA, ws1 =%s, ws2 =%s, Not OK\r\n", s1.c_str(), s2.c_str());
            #endif

            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
#include <sys/time.h>
#include <ctime>
#include <chrono>
using namespace std::chrono;
time_t begin2, end2;
steady_clock::time_point gStart;

extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_LearnWordUpdate(int _IME, wchar_t* KeyCode, wchar_t* wsContent, bool ForcePriority, bool IsMultiTap)
{
     if ( wcscmp(wsContent, L"[[begin]]") ==0 )
     {
          //time (&begin2);
          //gStart = high_resolution_clock::now();
          //gStartTime =   System.currentTimeMillis()/1000;

          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, [[begin]]");
              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, update, _IME =%d, USER dict file =%s", _IME, gUserFile.get_fname().c_str());
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          }
          #endif

          gImeX_Packages.clear();
          gImeX_Packages.shrink_to_fit();
     }
     else if ( wcscmp(wsContent, L"[[end]]") ==0 )
     {
          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, [[end]]");
              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, update, _IME =%d, USER dict file =%s", _IME, gUserFile.get_fname().c_str());
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          }
          #endif

          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, update NEW VOCA count =%d\r\n", (int)gImeX_Packages.size());
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          }
          #endif

          if ( gImeX_Packages.size() ==0 ) /// 2020/12/03, 介面端叫用 delete all ?
          {
               #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
               {
                   KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
                   KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, curr_total =%d, delete ALL !!!", gImeX_User->total_u());
                   KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
               }
               #endif

               if ( gImeX_User->total_u() >0 )
               {
                    gUserFile.clearKeys();
                    gUserFile.clear();
                    gImeX_User.clear_u();
                    gImeX_User->clear_u();

                    gPrioFile.clear();
                    gPrioFile.clear_u();
                    gImeX_Prio->clear_u();

                    gUserFile.resetAndSave();
               }
          }

          vector<PHRS_UI>
          uEexistAll;
          gUserFile.getAll_userData(_IME, uEexistAll);

          vector<PHRS_UI>::iterator
          bu_ExistAll =uEexistAll.begin();

          while ( bu_ExistAll !=uEexistAll.end() )
          {
                  #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                  {
                       string svoca =wchar_to_UTF8(bu_ExistAll->wsPhrs.c_str());
                       KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, check exist %s [%c]\r\n", svoca.c_str(), bu_ExistAll->wsKeys.c_str()[0]);
                  }
                  #endif

                  if ( bu_ExistAll->wsKeys.c_str()[0] =='m' )
                  {
                       // 'm'(MultiTap) 的使用者詞要保留
                       bu_ExistAll++;
                       continue;
                  }

                  PACKAGES_UI
                  ui;
                  ui.wsPhrs =bu_ExistAll->wsPhrs;

                  vector<PACKAGES_UI>::iterator
                  ui_found =find_if(gImeX_Packages.begin(), gImeX_Packages.end(), Cmp_LearnWord_User_VOCA(ui));

                  if ( ui_found ==gImeX_Packages.end() )
                  {
                       #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                       {
                           string svoca =wchar_to_UTF8(bu_ExistAll->wsPhrs.c_str());
                           KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, remove old %s\r\n", svoca.c_str());
                       }
                       #endif

                       #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                       //KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, wIsMultiTap =[ %ls, %c ] in .USER\r\n");
                       #endif

                       wchar_t
                       wsKey[MAX_INPUT_KEY] ={0};

                       wstring
                       wContentEx;

                       for ( auto c: ui.wsPhrs )
                       {
                             wContentEx +=static_cast<wchar_t>(std::towlower(c));
                       }

                       gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
                       gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);

                       int
                       retDel =IQQI_DeleLearnedWord(_IME, wsKey, (wchar_t*)ui.wsPhrs.c_str());


                       #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)

                       if ( gUserFile.remove_ex(_IME, L"", (wchar_t*)ui.wsPhrs.c_str()) ==false )
                       {
                            #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                            KIKA_LOGD("Kika-ENGINE,  IQQI_LearnWordUpdate::remove [ ] FAIL!\r\n");
                            #endif
                       }
                       if ( gUserFile.remove_ex(_IME, (wchar_t *)L"m", (wchar_t*)ui.wsPhrs.c_str()) ==false )
                       {
                            #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                            KIKA_LOGD("Kika-ENGINE,  IQQI_LearnWordUpdate::remove [m] FAIL!\r\n");
                            #endif
                       }
                       if ( gUserFile.remove_ex(_IME, (wchar_t *)L"u", (wchar_t*)ui.wsPhrs.c_str()) ==false )
                       {
                            #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                            KIKA_LOGD("Kika-ENGINE,  IQQI_LearnWordUpdate::remove [u] FAIL!\r\n");
                            #endif
                       }

                       #else

                       if ( gUserFile.remove(_IME, L"", (wchar_t*)ui.wsPhrs.c_str()) ==false )
                       {
                            #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                            KIKA_LOGD("Kika-ENGINE,  IQQI_LearnWordUpdate::remove [ ] FAIL!\r\n");
                            #endif
                       }
                       if ( gUserFile.remove(_IME, (wchar_t *)L"m", (wchar_t*)ui.wsPhrs.c_str()) ==false )
                       {
                            #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                            KIKA_LOGD("Kika-ENGINE,  IQQI_LearnWordUpdate::remove [m] FAIL!\r\n");
                            #endif
                       }
                       if ( gUserFile.remove(_IME, (wchar_t *)L"u", (wchar_t*)ui.wsPhrs.c_str()) ==false )
                       {
                            #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                            KIKA_LOGD("Kika-ENGINE,  IQQI_LearnWordUpdate::remove [u] FAIL!\r\n");
                            #endif
                       }

                       #endif

                       gUserFile.delKeys(_IME, ui.wsPhrs.c_str());

                       // delete PrioData
                       ///gPrioFile.remove(_IME, wsKey, (wchar_t*)ui.wsPhrs.c_str());
                       ////gPrioFile.remove(_IME, (wchar_t *)L"", (wchar_t*)ui.wsPhrs.c_str());
                  }
                  else
                  {
                       #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                       string svoca =wchar_to_UTF8(bu_ExistAll->wsPhrs.c_str());
                       KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, no need remove, [ %s ] exist in .USER\r\n", svoca.c_str());
                       #endif // defined
                  }

                  bu_ExistAll++;
          }

          //uEexistAll.clear();
          //gUserFile.getAll_userData(_IME, uEexistAll);

          vector<PACKAGES_UI>::iterator
          bi =gImeX_Packages.begin();

          while ( bi !=gImeX_Packages.end() )
          {
                  if ( gUserFile.isExist_UserData(_IME, bi->wsPhrs.c_str()) ==false )
                  {
                       #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                       {
                           string svoca =wchar_to_UTF8(bi->wsPhrs.c_str());
                           KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, add NEW VOCA =%s\r\n", svoca.c_str());
                       }
                       #endif

                       IQQI_LearnWordEx(bi->_IME, (wchar_t* )bi->wsKeys.c_str(), (wchar_t* )bi->wsPhrs.c_str(), false, IsMultiTap);
                       //IQQI_LearnWordEx(bi->_IME, (wchar_t* )bi->wsKeys.c_str(), (wchar_t* )bi->wsPhrs.c_str(), ForcePriority, IsMultiTap);

                       #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                       KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, add it, USER dict size =[ %d ]\r\n", (int)gImeX_User.total_u());
                       #endif
                  }
                  else
                  {
                       #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                       string svoca =wchar_to_UTF8(bi->wsPhrs.c_str());
                       KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, no need to add, [ %s] already exist in .USER\r\n", svoca.c_str());
                       #endif // defined
                  }
                  bi++;
          }

          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
              KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, update USER done!\r\n");
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          }
          #endif

          gUserFile.updateAndSave(false);
          gPrioFile.updateAndSave(true);

          if ( _IME !=gUserFile.getImeID() )
          {
               #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
               {
                   KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
                   KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, IME =%d Restore to =%d!\r\n", _IME, gUserFile.getImeID());
                   KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
               }
               #endif

               gUserFile.clearKeys();
               gUserFile.clear();
               gImeX_User.clear_u();
               gImeX_User->clear_u();

               gPrioFile.clear();
               gPrioFile.clear_u();
               gImeX_Prio->clear_u();

               //恢復原本IME的使用者詞庫狀況
               // Restore old filename
               // Inital UserDB
               gUserFile.setImeID(gUserFile.getImeID());
               gUserFile.initial_UserData(gImeX_User, gUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);

               // Inital PrioDB
               gPrioFile.setImeID(gUserFile.getImeID());

               //2020/12/03
               //這裡不需要重新Load Prio file, 否則會造成重新存入上一個詞
               /////gPrioFile.initial_UserData(gImeX_Prio, gPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
               gPrioFile.set_fname(gPrioDBPath.c_str());

               #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
               {
                   KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
                   KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, restore USER file =%s\r\n", gUserFile.get_fname().c_str());
                   KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, restore PRIO file =%s\r\n", gPrioFile.get_fname().c_str());
                   KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
               }
               #endif
          }

          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
               KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
               KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, DONE !!!");
               KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          }
          #endif

          {
//              time (&end2);
//              double difference = difftime (end2,begin2);
//              char sss[256] ={0};
//              sprintf(sss, " TASK took : %.2lf S\n", difference);

              //auto stop = high_resolution_clock::now();
              //auto duration = duration_cast<microseconds>(stop - gStart);
              //long int time = static_cast<long int>(std::chrono::duration_cast<std::chrono::nanoseconds>duration.count());

              //string ms = to_string(duration.count());
              //KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, using time [ %s ] \r\n", ms.c_str());
          }
     }
     else
     {
          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
               string svoca =wchar_to_UTF8(wsContent);
               KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordUpdate, append VOCA %s\r\n", svoca.c_str());
          }
          #endif

          /// 2020/08/05
          ////if ( gUserFile.isExist_UserData(_IME, wsContent) ==false )
          {
               PACKAGES_UI
               data;
               data._IME    =_IME;
               data.wsPhrs  =wsContent;
               //data.wsKeys  =wsKey;
               data.orderID =gImeX_User->getOrderID_u(_IME, L"", wsContent);

               gImeX_Packages.push_back(data);
          }
     }
     return 0;
}



#ifdef __TEST
//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  LearnWordUpdateByTokenEx(int _IME, char* PinyinCodeTable, char* ContentTable, unsigned int Length, bool ForcePriority, bool IsMultiTap)
{
     int
     retVal =0;

     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_HK:
              case IQQI_IME_Chinese_TW:
                    char aa[256] ={0};
                    sprintf(aa, "Kika-ENGINE, Chinese no need to Learn Word !!!");
                    d_pintf_ex(aa);
                   return 0;
     }

     #if defined(__USER_DIC)
     {
          for ( long b =0, e =0; b <Length; e++ ) //auto x :tokens )
          {
                if ( ContentTable[e] ==KEY_TOKEN ) // |
                {
                     try
                     {
                          int sLen =(e -b);

                          #if defined(__FOR_ANDROID)
                          if ( sLen ==1 )
                          {
                               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                               KIKA_LOGD("Kika-ENGINE, Len=1 Word ignore to Learn");
                               #endif
                               continue;
                          }
                          #endif

                          if ( sLen >MAX_LEARN_WORD_LEN )
                          {
                               return ERR_DICT_OUTOF_WORD_LEN;
                          }

                          #if defined(__TEST) || defined(__DEBUG_MSG)
                          {
                              char aa[256] ={0};
                              sprintf(aa, "LearnWordByToken b=%ld, e=%ld, sLen =%d\r\n", b, e, sLen);
                              d_pintf_ex(aa);
                          }
                          #endif

                          char
                          sIn[MAX_LEARN_WORD_LEN +1] ={0};

                          memcpy(sIn, ContentTable +b, sLen);
                          sIn[sLen] =0;

                          b =e +1;

                          #if defined(__TEST) || defined(__DEBUG_MSG)
                          {
                              char aa[256] ={0};
                              sprintf(aa, "LearnWordUpdateByTokenEx get %s to Learn\r\n", sIn);
                              d_pintf_ex(aa);
                          }
                          #endif

                          wstring
                          wContent =utf8_to_utf16(sIn);

                          retVal =IQQI_LearnWordUpdate(_IME, (wchar_t *)L"", (wchar_t *)wContent.c_str(), false, false);

                          if ( retVal !=0 )
                          {
                               char aa[256] ={0};
                               sprintf(aa, "LearnWordUpdateByTokenEx error, retVal =%d\r\n", retVal);
                               d_pintf_ex(aa);
                               break;
                          }
                     }
                     catch ( ... )
                     {
                          char aa[256] ={0};
                          sprintf(aa, "LearnWordByToken exception error\r\n");
                          d_pintf_ex(aa);
                          return ERR_DICT_INVALID_PARAM;
                     }
                }
          }
     }
     #endif
     return retVal;
}
#endif // __TEST

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_LearnWordEditSaveAs(int _IME, const char* UserDBName, wchar_t* KeyCode, wchar_t* Content, bool ForcePriority, bool IsMultiTap)
{
     string sUserDBPath =UserDBName;
     string sPrioDBPath =UserDBName;

     //sPrioDBPath =sUserDBPath.c_str();
     sPrioDBPath +=".pri";

     // Set new filename
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEditSaveAs User Path =%s", sUserDBPath.c_str());
     KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEditSaveAs Pri  Path =%s", sPrioDBPath.c_str());
     #endif

     int
     oImeID =gUserFile.getImeID();
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEditSaveAs filename =%s, check IME %d=(%d) \r\n", sUserDBPath.c_str(), oImeID, _IME);
     #endif

     if ( oImeID !=_IME )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
          KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEditSaveAs filename =%s, reload IME %d=(%d)\r\n", sUserDBPath.c_str(), oImeID, _IME);
          #endif

          // Inital UserDB
          gUserFile.initial_UserData(gImeX_User, sUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);
          // Inital PrioDB
          gPrioFile.initial_UserData(gImeX_Prio, sPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
     }
     else
     {
          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          KIKA_LOGD("Kika-ENGINE, ----------------------");
          KIKA_LOGD("Kika-ENGINE, ----------KEEP!!!!!!!%d-", _IME);
          KIKA_LOGD("Kika-ENGINE, ----------------------");
          #endif
     }

     #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
     {
          KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordEditSaveAs call IQQI_LearnWordUpdate\r\n");
     }
     #endif

     int
     retVal =IQQI_LearnWordUpdate(_IME, KeyCode, Content, ForcePriority, IsMultiTap);

     #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          KIKA_LOGD("Kika-ENGINE, ----------------------");
          KIKA_LOGD("Kika-ENGINE, -----------LL done");
          KIKA_LOGD("Kika-ENGINE, ----------------------");
     #endif

     // 因為Thread 執行順序不定, 這裡不應恢復
     // if ( oImeID !=_IME )
     // {
     //      // Restore old filename
     //      // Inital UserDB
     //      gUserFile.setImeID(_IME);
     //      gUserFile.initial_UserData(gImeX_User, gUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);
     //      // Inital PrioDB
     //      gPrioFile.setImeID(_IME);
     //      gPrioFile.initial_UserData(gImeX_Prio, gPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
     // }

     return retVal;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_LearnWordSaveAs(int _IME, const char* UserDBName, wchar_t* KeyCode, wchar_t* Content, bool ForcePriority, bool IsMultiTap)
{
     string sUserDBPath =UserDBName;
     string sPrioDBPath =UserDBName;

     //sPrioDBPath =sUserDBPath.c_str();
     sPrioDBPath +=".pri";

     // Set new filename
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     {
         string sKeyCode =wchar_to_UTF8(Content);
         KIKA_LOGD("Kika-ENGINE, _IME =%d, IQQI_LearnWordSaveAs User Path =%s, Content =%s", _IME, sUserDBPath.c_str(), sKeyCode.c_str());
         KIKA_LOGD("Kika-ENGINE, _IME =%d, IQQI_LearnWordSaveAs Pri  Path =%s, Content =%s", _IME, sPrioDBPath.c_str(), sKeyCode.c_str());
     }
     #endif

     int
     oImeID =gUserFile.getImeID();
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordSaveAs filename =%s, check IME %d=(%d) \r\n", UserDBName, oImeID, _IME);
     #endif

     if ( oImeID !=_IME )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
          KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordSaveAs filename =%s, reload IME %d=(%d)\r\n", UserDBName, oImeID, _IME);
          #endif

          // Inital UserDB
          gUserFile.initial_UserData(gImeX_User, sUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);

          // Inital PrioDB
          //2020/12/03
          //這裡不需要重新Load Prio file, 否則會造成重新存入上一個詞
          /////gPrioFile.initial_UserData(gImeX_Prio, sPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
          gPrioFile.set_fname(sPrioDBPath.c_str());
     }

     int
     retVal =0;
     #if defined(__FOR_PUNKT)
     if ( wcslen(Content) >0 )
     {
         //IQQI_LearnWordUpdate(_IME, KeyCode, (wchar_t*)L"[[begin]]", ForcePriority, IsMultiTap);
         retVal =IQQI_LearnWordUpdate(_IME, KeyCode, Content, ForcePriority, IsMultiTap);
         //IQQI_LearnWordUpdate(_IME, KeyCode, (wchar_t*)L"[[end]]", ForcePriority, IsMultiTap);
     }
     #else
     retVal =IQQI_LearnWordEx(_IME, KeyCode, Content, ForcePriority, IsMultiTap);
     #endif

     // 因為Thread 執行順序不定, 這裡不應恢復
     //if ( oImeID !=_IME )
     //{
     //     // Restore old filename
     //     // Inital UserDB
     //     gUserFile.setImeID(_IME);
     //     gUserFile.initial_UserData(gImeX_User, gUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);
     //     // Inital PrioDB
     //     gPrioFile.setImeID(_IME);
     //     gPrioFile.initial_UserData(gImeX_Prio, gPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
     //}

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_LearnWordSaveAs Final [%s] [load total =%d]\r\n", sUserDBPath.c_str(), gImeX_User.total_u());
     #endif

     return retVal;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_LearnWord(int _IME, wchar_t* KeyCode, wchar_t* Content)
{
     // LearnWord 一律視為 Import 且從 MultiTap 而來
     return IQQI_LearnWordEx(_IME, KeyCode, Content, true, true);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_IsLearnWordExist(int _IME, const wchar_t* KeyCode, const wchar_t* Content)
{
     #if !defined(__FOR_ANDROID)
     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_HK:
              case IQQI_IME_Chinese_TW:
                   #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                    KIKA_LOGD("Kika-ENGINE, Chinese no need to Learn Word 1 !!!");
                    #elif defined(__TEST) || defined(__DEBUG_MSG)
                    char aa[256] ={0};
                    sprintf(aa, "Kika-ENGINE, Chinese no need to Learn Word 1 !!!");
                     #if defined(__TEST)
                    d_pintf_ex(aa);
                     #else
                    d_pintf(aa);
                     #endif
                   #endif
                   return 0;
     }
     #endif

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_IsLearnWordExist [%s] [OK load total =%d]\r\n", gUserFile.get_fname().c_str(), gUserFile.total_u());
     #endif

     return gUserFile.isExist_UserData(_IME, Content);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_DeleLearnedWordFrom( int _IME, const char *UserDBName, wchar_t* KeyCode, wchar_t* Content)
{
     string sUserDBPath =UserDBName;
     string sPrioDBPath =UserDBName;

     //sPrioDBPath =sUserDBPath.c_str();
     sPrioDBPath +=".pri";

     // Set new filename
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_DeleLearnedWordFrom User Path =%s", sUserDBPath.c_str());
     KIKA_LOGD("Kika-ENGINE, IQQI_DeleLearnedWordFrom Pri  Path =%s", sPrioDBPath.c_str());
     #endif

     int
     oImeID =gUserFile.getImeID();
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, IQQI_DeleLearnedWordFrom filename =%s, check IME %d=(%d)\r\n", sUserDBPath.c_str(), oImeID, _IME);
     #endif

     if ( oImeID !=_IME )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
          KIKA_LOGD("Kika-ENGINE, IQQI_DeleLearnedWordFrom filename =%s, reload IME %d=(%d)\r\n", sUserDBPath.c_str(), oImeID, _IME);
          #endif

          // Inital UserDB
          gUserFile.initial_UserData(gImeX_User, sUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);
          // Inital PrioDB
          gPrioFile.initial_UserData(gImeX_Prio, sPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
     }

     int
     retVal =IQQI_DeleLearnedWord(_IME, KeyCode, Content);

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_DeleLearnedWordFrom [%s] [load total =%d]\r\n", gUserFile.get_fname().c_str(), gUserFile.total_u());
     #endif

     if ( oImeID !=_IME )
     {
          gUserFile.clearKeys();
          gUserFile.clear();
          gImeX_User.clear_u();
          gImeX_User->clear_u();

          gPrioFile.clear();
          gPrioFile.clear_u();
          gImeX_Prio->clear_u();
          // Restore old filename
          // Inital UserDB
          gUserFile.setImeID(_IME);
          gUserFile.initial_UserData(gImeX_User, gUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);
          // Inital PrioDB
          gPrioFile.setImeID(_IME);
          gPrioFile.initial_UserData(gImeX_Prio, gPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
     }

     return retVal;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_GetLearnedWordFromFile(int _IME, const char *UserDBName, int Number, wchar_t** Candidates)
{
     string sUserDBPath =UserDBName;
     string sPrioDBPath =UserDBName;

     //sPrioDBPath =sUserDBPath.c_str();
     sPrioDBPath +=".pri";

     // Set new filename
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile User Path =%s", sUserDBPath.c_str());
     KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile Pri  Path =%s", sPrioDBPath.c_str());
     #endif

     int
     oImeID =gUserFile.getImeID();
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile filename =%s, check IME %d=(%d)\r\n", sUserDBPath.c_str(), oImeID, _IME);
     #endif

     if ( oImeID !=_IME )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
          KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile filename =%s, reload IME %d=(%d)\r\n", sUserDBPath.c_str(), oImeID, _IME);
          #endif

          // Inital UserDB
          gUserFile.initial_UserData(gImeX_User, sUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);
          // Inital PrioDB
          gPrioFile.initial_UserData(gImeX_Prio, sPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
     }

     set<wstring>
     all;

     vector<PHRS_UI>
     uEexistAll;
     gUserFile.getAll_userData(_IME, uEexistAll);

     vector<PHRS_UI>::iterator
     bu_ExistAll =uEexistAll.begin();

     while ( bu_ExistAll !=uEexistAll.end() )
     {
             #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
             {
                 string svoca =wchar_to_UTF8(bu_ExistAll->wsPhrs.c_str());
                 KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile, get exist [ %s ]\r\n", svoca.c_str());
             }
             #endif

             #if !defined(__SONIM)
             if ( bu_ExistAll->wsKeys.c_str()[0] =='m' )
             #endif
             {
                  // 'm'(MultiTap) 的使用者詞才需要列出
                  all.insert(bu_ExistAll->wsPhrs.c_str());
             }
             bu_ExistAll++;
     }

     int
     total =0;

     set<wstring>::iterator
     bi =all.begin();

     while ( bi !=all.end() )
     {
             #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
             {
                 string svoca =wchar_to_UTF8(bi->c_str());
                 KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile, set candidate [ %s ]\r\n", svoca.c_str());
             }
             #endif

             if ( total >Number -1 )
             {
                  #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
                  {
                  KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile, out of Number (%d)\r\n", total);
                  }
                  #endif
                  break;
             }
             wcscpy(Candidates[total++], bi->c_str());
             bi++;
     }

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile [%s] [load total =%d] done\r\n", gUserFile.get_fname().c_str(), gUserFile.total_u());
     #endif

     if ( oImeID !=_IME )
     {
          // Restore old filename
          // Inital UserDB
          gUserFile.setImeID(_IME);
          gUserFile.initial_UserData(gImeX_User, gUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);
          // Inital PrioDB
          gPrioFile.setImeID(_IME);
          gPrioFile.initial_UserData(gImeX_Prio, gPrioDBPath.c_str(), _IME, LIMIT_PRIORITY_COUNT);
     }

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_GetLearnedWordFromFile [%s] [load total =%d] final done\r\n", gUserFile.get_fname().c_str(), gUserFile.total_u());
     #endif

     return total;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_ResetLearnedWordFromFile( int _IME, const char *UserDBName)
{
     // Set new filename
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_ResetLearnedWordFromFile User Path =%s\r\n", UserDBName);
     #endif

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_ResetLearnedWordFromFile User Path =%s\r\n", UserDBName);
     #endif

     // Inital UserDB
     gUserFile.reset_UserData(_IME, gImeX_User, UserDBName);

     //gUserFile.clearKeys();
     //gUserFile.clear();
     //gImeX_User.clear_u();
     //gImeX_User->clear_u();

     gPrioFile.clear();
     gPrioFile.clear_u();
     gImeX_Prio->clear_u();

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("----------------------\r\n");
     KIKA_LOGD("Kika-ENGINE, IQQI_ResetLearnedWordFromFile Prio Path =%s\r\n", gPrioFile.get_fname().c_str());
     KIKA_LOGD("Kika-ENGINE, IQQI_ResetLearnedWordFromFile Prio Size =%d\r\n", gImeX_Prio->total_u());
     KIKA_LOGD("----------------------\r\n");
     #endif

     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_LearnIdiomTrad( wchar_t*  U16Word )
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_LearnIdiomSimp( wchar_t*  U16Word )
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_UpdateDBVersionInfo(int _IME)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_GetComposingText( int _IME, wchar_t* KeyCode, int Type, int aIndicator)
{
     #if defined(__DEBUG_LOG) || defined(__DEBUG_MSG)
      #if defined(__FOR_ANDROID)
     KIKA_LOGD("GetComposingText Input KeyCode =%ls (%x, %x)", KeyCode, KeyCode[0], KeyCode[1]);
      #endif
     #endif

     #if !defined(__NO_KOREAN_INC)

     #if !defined(__TEST) && defined(__LOCK_MODEL_NAME)
     if ( wcscmp(gWsModel.c_str(), LOCK_MODEL_NAME) !=0 )
     {
          return (wchar_t *)MSG_INVALID_MODEL;
     }
     #endif

     switch ( _IME )
     {
              #if !defined(__NO_KOREAN)
              case IQQI_IME_Korean:
                   {
                   #if defined(__DEBUG_LOG) && defined(__DEBUG_MSG)
                   {
                       wchar_t aa[MAX_1024] ={0};
                       wcscpy(aa, L"GetComposingText KeyCode =");

                       for ( int i =0; i <gKeyCompBuf.size(); i++ )
                       {
                             wchar_t aa2[MAX_1024] ={0};
                             swprintf(aa2, MAX_1024, L"0x%x, ", KeyCode[i]);

                             wcscat(aa, aa2);
                       }
                       //KIKA_LOGD("GetComposingText KeyCode =%ls", (wchar_t*)KeyCode);
                       #if defined(__FOR_ANDROID)
                       KIKA_LOGD("GetComposingText KeyCode =%ls", aa);
                       KIKA_LOGD("GetComposingText KeyCode =%ls", (wchar_t*)KeyCode);
                       #else
                       char aaa[MAX_1024] ={0};
                       sprintf(aaa, "GetComposingText KeyCode =%ls", aa);
                       d_pintf(aaa);
                       sprintf(aaa, "GetComposingText KeyCompBuf final =%ls", (wchar_t*)gKeyCompBuf.c_str());
                       d_pintf(aaa);
                       #endif
                   }
                   #endif

                   //----------------------------------------------------------
                   #if defined(__SONIM) || defined(__FOR_HUAWEI)
                   gKeyCompBuf =gKoreanX.getComposingText(KeyCode, aIndicator ==1? true: false);
                   #else
                   gKeyCompBuf =gKoreanX.getComposingText(KeyCode, true);
                   #endif
                   //----------------------------------------------------------

                   #if defined(__DEBUG_LOG) && defined(__DEBUG_MSG)
                   {
                       wchar_t aa[MAX_1024] ={0};
                       wcscpy(aa, L"GetComposingText Result =");

                       for ( int i =0; i <gKeyCompBuf.size(); i++ )
                       {
                             wchar_t aa2[MAX_1024] ={0};
                             swprintf(aa2, MAX_1024, L"0x%x, ", gKeyCompBuf[i]);

                             wcscat(aa, aa2);
                       }
                       #if defined(__FOR_ANDROID)
                       KIKA_LOGD("GetComposingText %ls", aa);
                       KIKA_LOGD("GetComposingText KeyCompBuf final =%ls", (wchar_t*)gKeyCompBuf.c_str());
                       #else
                       char aaa[MAX_1024] ={0};
                       sprintf(aaa, "GetComposingText KeyCode =%ls", aa);
                       d_pintf(aaa);
                       sprintf(aaa, "GetComposingText KeyCompBuf final =%ls", (wchar_t*)gKeyCompBuf.c_str());
                       d_pintf(aaa);
                       #endif
                   }
                   #endif

                   return (wchar_t*)gKeyCompBuf.c_str();
                   }
                   break;
              #endif

              default:
                   break;
     }
     #else
      #if defined(__DEBUG_LOG) || defined(__DEBUG_MSG)
     KIKA_LOGD("GetComposingText not supported!");
      #endif
     #endif

     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_ComposingTextExtend( int _IME, wchar_t* ComposingText, int Type)
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_StartInsertCodeMode( int _IME, wchar_t* AllInputCode, int Number, wchar_t** Candidates, wchar_t* ComposingText)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_InsertCode( wchar_t* InsertKeyCode, wchar_t** Candidates, wchar_t* ComposingText)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_BackspaceInsertCode( wchar_t** Candidates, wchar_t* ComposingText)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_DoneInsertCodeMode()
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_CancelInsertCodeMode()
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
bool IQQILIBQTSHARED_EXPORT IQQI_IsInsertCodeMode()
{
     return false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_GetPredictedSentence( wchar_t *KeyCode)
{
     #if defined(__DEBUG_MSG)
     return wcscpy(KeyCode, gDebugSz);
     #else
     return (wchar_t*)L"";
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_GetSelectedCandidateCode(int Location)
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_GetRemainsCode(int Location)
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_GetNextWordCandidates( int _IME, wchar_t* KeyCode, int Begin, int Number, wchar_t **Candidates)
{
#if defined(__IQQI_PY)
    if (_IME == IQQI_IME_Chinese_CN) {
        return IQQI_GetNextWordCandidates_Py(_IME, KeyCode, Begin, Number, Candidates);
    }
#endif

     IQQI_FreeKika();

     //------------------------------------------------------------------------
     #if !defined(__NO_KIKA_IME)

     int
     retVal =gMain.Initial(_IME, (char *)gMainDBPath.c_str(), NULL);

     #if defined(__DEBUG_MSG)
     {
     char aa[256] ={0};
     sprintf(aa, "IQQI_GetNextWordCandidates, _IME =%d, main initial retVal =%d\r\n", _IME, retVal);
     d_pintf(aa);
     }
     #endif

     #endif

     vector<VCandidate>
     vm;

     switch ( _IME )
     {
              case IQQI_IME_Changjie:
              case IQQI_IME_Chinese_CN:
              case IQQI_IME_Chinese_TW:
              case IQQI_IME_Chinese_HK:
                   ///if ( gKeybdMode ==for_T9 )
                   {
                        vector<wstring>
                        vnxt;

                        gImeX->getIdiom(_IME, KeyCode, vnxt, KeyCode, 40);

                        #if defined(__DEBUG_MSG)
                        {
                        char aa[256] ={0};
                        sprintf(aa, "IQQI_GetNextWordCandidates, _IME =%d, getIdiom =<%d>\r\n", _IME, (int)vnxt.size());
                        d_pintf(aa);
                        }
                        #endif

                        vector<wstring>::iterator
                        bi =vnxt.begin();

                        int
                        iSD =0;

                        for ( bi =vnxt.begin(); bi !=vnxt.end(); bi++ )
                        {
                              vm.push_back(VCandidate(bi->c_str(), iSD++));
                        }
                        break;
                   }

              default:
                   #if !defined(__NO_KIKA_IME)

                   gCandidateCount =gMain.GetNextWordCandidates(_IME, KeyCode, vm);

                   if ( gCandidateCount ==ERR_NULL_STR )
                   {
                        #if defined(__DEBUG_MSG)
                        {
                        char aa[256] ={0};
                        sprintf(aa, "IQQI_GetNextWordCandidates, _IME =%d, gCandidateCount =ERR_NULL_STR (%d)\r\n", _IME, gCandidateCount);
                        d_pintf(aa);
                        }
                        #endif

                        return gCandidateCount =0;
                   }
                   #else
                   break;
                   #endif
     }

     {
          #if defined(__DEBUG_MSG)
          {
          char aa[256] ={0};
          sprintf(aa, "IQQI_GetNextWordCandidates, _IME =%d, gCandidateCount =(%d)\r\n", _IME, gCandidateCount);
          d_pintf(aa);
          }
          #endif
     }

     //------------------------------------------------------------------------
     // re-assign counter & content
     gCandidateCount =0;

     vector<VCandidate>::iterator
     ii;

     int
     wsLen =wcslen(KeyCode);

     #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
     int
     datIdx =0;
     #endif

     // main dictionary
     for ( ii = vm.begin(); ii != vm.end(); ii++ )
     {
           #if defined(__RULE_1)
           if ( gKeybdMode ==for_T9 && wcslen(ii->ws.c_str()) !=wsLen )
           {
                continue;
           }
           #endif

           #if !defined(__TEST_USER_ONLY)
           //if ( wcslen(ii->ws.c_str()) >=wsLen )
           {
                #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                if ( datIdx <Begin )
                {
                     datIdx++;

                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                     //KIKA_LOGD("Kika-ENGINE, datIdx =%d, Begin =%d, continue\r\n", datIdx, Begin);
                     #endif

                     continue;
                }
                else if ( datIdx >=(Begin +Number) )
                {
                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                     KIKA_LOGD("Kika-ENGINE, GetCandidates Break!!!\r\n");
                     #endif
                     break;
                }
                datIdx++;

                #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                //KIKA_LOGD("Kika-ENGINE, datIdx =%d, Max =%d\r\n", datIdx, Begin +Number);
                #endif

                #endif

                if ( ii->ws.find(' ') ==-1 && gCandidateCount <Number )
                {
                     wcscpy(Candidates[gCandidateCount++], ii->ws.c_str());
                }
           }
           #endif
     }

     #if defined(__DEBUG_MSG)
     {
     char aa[256] ={0};
     sprintf(aa, "IQQI_GetNextWordCandidates, _IME =%d, final candidates count =[%d]\r\n", _IME, gCandidateCount);
     d_pintf(aa);
     }
     #endif

     return gCandidateCount;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_LearnNextWord(int Language, wchar_t* U16Word )
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_DeleLearnedNextWord(int Language, wchar_t* Content )
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_SwitchToCustomDB( int _IME, char* ItemName)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_MultiType_KeyDefine(wchar_t* KeyDefine, bool TimeCtrl)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
void IQQILIBQTSHARED_EXPORT IQQI_MultiType_Clear()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
int IQQILIBQTSHARED_EXPORT IQQI_MultiType_Interval(int Millisecond)
{
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_MultiType_Input(int KeyCode, wchar_t* ComposingText)
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
#if defined(__0208)
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_MultiTap_Input(int _IME, int KeyCode, int PrevKeyCode, int Count)
#else
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_MultiTap_Input(int _IME, int KeyCode, int Count)
#endif
{
     #if defined(__DEMO_EXPIRED)

     #if defined(__DEBUG_LOG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
     char sss[1024]={0};
     sprintf(sss, "Mtp-now: %d-%d-%d\r\n", gTm.tm_year + 1900, gTm.tm_mon + 1, gTm.tm_mday);
     KIKA_LOGD(sss);
     #endif

     bool
     y0 =gTm.tm_year + 1900 >EXPIRED_YEAR,
     y1 =gTm.tm_year + 1900 ==EXPIRED_YEAR,
     m1 =gTm.tm_mon + 1 >EXPIRED_MONTH,
     m2 =gTm.tm_mon + 1 ==EXPIRED_MONTH,
     d1 =gTm.tm_mday >EXPIRED_DATE;

     #if defined(__DEBUG_LOG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
     sprintf(sss, "Mtp-now: y0 =%d, y1=%d, m1=%d, m2=%d, d1=%d\r\n", y0, y1, m1, m2, d1);
     KIKA_LOGD(sss);
     #endif

     if ( y0 ==true )
     {
          return MSG_EXPIRED_EX;
     }
     else if ( y1 ==true )
     {
          if ( m1 ==true )
          {
               return MSG_EXPIRED_EX;
          }
          else if ( m2 ==true && d1 ==true )
          {
               return MSG_EXPIRED_EX;
          }
     }

     #endif

     #if !defined(__TEST) && defined(__LOCK_MODEL_NAME)

     if ( wcscmp(gWsModel.c_str(), LOCK_MODEL_NAME) !=0 )
     {
          #if defined(__DEBUG_LOG) || (defined(__FOR_ANDROID) && defined(__ANDROID_MSG))
          char sss[1024]={0};
          sprintf(sss, "Invalid Model Name =%ls\r\n", MSG_INVALID_MODEL;
          KIKA_LOGD(sss);
          #endif

          return (wchar_t *)MSG_INVALID_MODEL;
     }
     #endif

     int
     Ch =KeyCode ==KEY_STAR_1 ? 10 :KeyCode -0x30;

     wchar_t
     def[1024] ={0};

     #if defined(__DEBUG_LOG) || defined(__DEBUG_MSG)
     char aa[MAX_AA] ={0};
      #if defined(__0208)
     sprintf(aa, "IME =%d, KeyCode =%x, PrevKeyCode ==%x\r\n", _IME, KeyCode, PrevKeyCode);
      #else
     sprintf(aa, "IME =%d, KeyCode =%x\r\n", _IME, KeyCode);
      #endif
     d_pintf(aa);
     #endif

     #if !defined(__NOT_ALL_FOR_T9_1_KEY)

      #if defined(__USING_GET_VCANDIDATE) || defined(__FOR_HUAWEI)
     vector<wstring>
     vSz;
     gMap.MultiType_ToBufferV(_IME, Ch, vSz);

     static wstring
     gAll;
     gAll =L"";

     vector<wstring>::iterator
     bi =vSz.begin();

     #if defined(__DEBUG_LOG) || defined(__DEBUG_MSG)
     KIKA_LOGD("Kika-ENGINE, KeyCode =%d, vSz size =%d\r\n", Ch, (int)vSz.size());
     #endif

     while ( bi !=vSz.end() )
     {
            if ( gAll.size() >0 )
                 gAll +=L"##";

            gAll +=bi->c_str();
            bi++;
     }
     return (wchar_t*)gAll.c_str();
      #else

     gMap.MultiType_ToBufferEx(_IME, Ch, L"|", Count);

      #endif

     #else

     #if defined(__0208)
     gMap.MultiType_InputFmtSz(_IME, Ch, PrevKeyCode, (wchar_t *)L"|", Count);
     #else
     gMap.MultiType_InputFmtSz(_IME, Ch, 0, (wchar_t *)L"|", Count);
     #endif

     #endif

     return gMap.GetMultiSz();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
void IQQILIBQTSHARED_EXPORT IQQI_SetOption(unsigned int Item, int value)
{
     if ( Item ==opt_KeybdMode )
     {
          switch ( value )
          {
                   case for_T9:
                        gKeybdMode =for_T9;
                        break;

                   default:
                        gKeybdMode =for_Qwerty;
                        break;
          }
          gMap.setKbMode(gKeybdMode);
     }
     else if ( Item ==opt_ReportActivate )
     {
          gReportActivate =value;
     }
     else if ( Item ==opt_FullMatchKey )
     {
          gFullMatchKey =value;
     }
     else if ( Item ==opt_4LenPriority )
     {
          g4LenPriority =value;
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_GetCT2CSData(wchar_t* CT)
{
     return (wchar_t*)L"";
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
#if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_InitialDictionary(int _IME, const char* sDictName)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_InitialDictionary, IME =%d, dictName =%s\r\n", _IME, sDictName);
     #endif

     return gVoca_InnerX.loadDict(sDictName);
}

extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_IsVocabularyExist(int _IME, const wchar_t* wsVoca)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_IsVocabularyExist, IME =%d, check Vocabulary =%ls\r\n", _IME, wsVoca);
     #endif

     return gVoca_InnerX.isExist(wsVoca);
}

extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_AppendVocabulary(int _IME, const wchar_t* wsVoca)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_AppendVocabulary, IME =%d, check Vocabulary =%ls\r\n", _IME, wsVoca);
     #endif

     gVoca_InnerX.append(wsVoca);

     return 1;
}

extern "C"
int  IQQILIBQTSHARED_EXPORT IQQI_DeleteVocabulary(int _IME, const wchar_t* wsVoca)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_DeleteVocabulary, IME =%d, check Vocabulary =%ls\r\n", _IME, wsVoca);
     #endif

     gVoca_InnerX.erase(wsVoca);

     return 1;
}

extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_NMapKeyCode(int _IME, const wchar_t* wsVoca, int mapSize)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_NMapKeyCode, IME =%d, Vocabulary In =%ls\r\n", _IME, wsVoca);
     #endif

     static wchar_t
     wsCompensateKey[4096] ={0};

     gMap.Numb2keyCode(_IME, wsVoca, wsCompensateKey, 4096);

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_NMapKeyCode, IME =%d, Vocabulary Out =%ls\r\n", _IME, wsCompensateKey);
     #endif

     return wsCompensateKey;
}

extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_KeyCodeNMap(int _IME, const wchar_t* wsVoca, int mapSize)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_KeyCodeNMap, IME =%d, Vocabulary In =%ls\r\n", _IME, wsVoca);
     #endif

     static wchar_t
     wsCompensateKey[4096] ={0};

     gMap.keyCode2Numb(_IME, wsVoca, wsCompensateKey, 4096);

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_KeyCodeNMap, IME =%d, Vocabulary Out =%ls\r\n", _IME, wsCompensateKey);
     #endif

     return wsCompensateKey;
}

extern "C"
wchar_t* IQQILIBQTSHARED_EXPORT IQQI_ShiftKeyCode(int _IME, const wchar_t* cCode)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_KeyShiftCode, IME =%d, Char In =%c\r\n", _IME, cCode[0]);
     #endif

     static wchar_t
     wsShiftCode[20];
     wsShiftCode[0] =gMap.keyCode2Shift(_IME, cCode[0]);

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_KeyShiftCode, IME =%d, Char Out =%ls\r\n", _IME, wsShiftCode);
     #endif

     return wsShiftCode;
}

extern "C"
key* IQQILIBQTSHARED_EXPORT IQQI_GetVirtualKeybdLayout(int _IME, bool isQwerty, int &Total)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_GetVirtualKeybdLayout, IME =%d, isQwerty =%d\r\n", _IME, isQwerty);
     #endif

     key*
     bKey =gMap.GetLayout(_IME);

     Total =gMap.GetLayoutTotalBtn();

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, IQQI_GetVirtualKeybdLayout, IME =%d, isQwerty =%d, Total =%d\r\n", _IME, isQwerty, Total);
     #endif

     return bKey;
}

#endif

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
extern "C"
char* IQQILIBQTSHARED_EXPORT IQQI_Version()
{
     return (char *)IQQI_VERSION_NO;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool userData_imp::isExist_Pri(int _IME, const wchar_t *wsKey, const wchar_t *wsPhrs)
{
     if ( gImeX_Prio->isExist_u(_IME, wsKey, wsPhrs) )
     {
          return true;
     }
     return false;
}

bool userData_imp::append_keys(int _IME, const wchar_t *wsTag, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 iNext)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     bool ret = append_ex(_IME, wsTag, wsPhrs, iNext);
     #else
     bool ret = append(_IME, wsTag, wsPhrs, iNext);
     #endif

     #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
     {
         string
         sUTF8_Keys =wchar_to_UTF8(wsKeys),
         sUTF8_Word =wchar_to_UTF8(wsPhrs);

         KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
         KIKA_LOGD("Kika-ENGINE, append_keys, _IME =%d, file =%s\r\n", _IME, get_fname().c_str());
         KIKA_LOGD("Kika-ENGINE, append_keys, add (%s, %s), ret =%d, size =%d\r\n", sUTF8_Keys.c_str(), sUTF8_Word.c_str(), ret, (int)vdict.size());
         KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
     }
     #endif

     //if ( ret ==true )
     if ( wcslen(wsKeys) >0 )
     {
          wchar_t
          wsIME[10];

          swprintf(wsIME, 10, RECORD_KEY_SZ, _IME);

          wstring
          wsPhrsNew =wsIME;
          wsPhrsNew +=wsPhrs;

          KeyForImeContentX.insert(pair<wstring, wstring>(wsPhrsNew, wsKeys));

          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
              string
              sUTF8_Keys =wchar_to_UTF8(wsKeys);

              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
              KIKA_LOGD("Kika-ENGINE, append_keys, add KeyForImeContentX (%s), size =%d\r\n", sUTF8_Keys.c_str(), (int)vdict.size());
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          }
          #endif
     }

     return ret;
}

bool userData_imp::delKeys(int _IME, wstring wContent)
{
     #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
     {
         KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
         KIKA_LOGD("Kika-ENGINE, delKeys, _IME =%d, file =%s\r\n", _IME, get_fname().c_str());
     }
     #endif

     wchar_t
     wsIME[10];

     swprintf(wsIME, 10, RECORD_KEY_SZ, _IME);

     wstring
     wsPhrsNew =wsIME;
     wsPhrsNew +=wContent;

     map<wstring, wstring>::iterator
     pFound = KeyForImeContentX.find(wsPhrsNew);

     if ( pFound ==KeyForImeContentX.end() )
     {
          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
              string
              sUTF8_Word =WstringToString(wContent);

              KIKA_LOGD("Kika-ENGINE, delKeys (%d, %s) FAIL, size =%d\r\n", _IME, sUTF8_Word.c_str(), (int)vdict.size());
          }
          #endif

          return false;
     }

     KeyForImeContentX.erase(pFound, pFound);

     #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID) && defined(__DEBUG_LEARN_WORD)
     {
         string
         sUTF8_Word =WstringToString(wContent);

         KIKA_LOGD("Kika-ENGINE, delKeys (%d, %s) OK, size =%d\r\n", _IME, sUTF8_Word.c_str(), (int)vdict.size());
     }
     #endif

     return false;
}

bool userData_imp::findKeys(int _IME, wstring wContent, wchar_t* wsKey, int size)
{
     wchar_t
     wsIME[10];

     swprintf(wsIME, 10, RECORD_KEY_SZ, _IME);

     wstring
     wsPhrsNew =wsIME;
     wsPhrsNew +=wContent;

     map<wstring, wstring>::iterator
     pFound = KeyForImeContentX.find(wsPhrsNew);

     if ( pFound ==KeyForImeContentX.end() )
     {
          return false;
     }

     if ( size >pFound->second.size() )
     {
          wcscpy(wsKey, pFound->second.c_str());
          return true;
     }
     return false;
}

#if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
bool userData_imp::remove_ex(int _IME, const wchar_t *wsKeys, const wchar_t *wsPhrs)
{
     ///bool ret = remove(_IME, wsKeys, wsPhrs);

     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     PHRS_UI
     ui2(this, _IME, wsKeys, wsContent.c_str());

     KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
     KIKA_LOGD("Kika-ENGINE, remove_ex, _IME =%d, file =%s\r\n", _IME, get_fname().c_str());

     string
     sUTF8_Keys =wchar_to_UTF8(wsKeys),
     sUTF8_Word =wchar_to_UTF8(wsPhrs);

     PHRS_ITR
     ui2_found =std::find(vdict.begin(), vdict.end(), ui2);

     if ( ui2_found != vdict.end() )
     {
          vdict.erase(ui2_found);
          qsort();

          KIKA_LOGD("Kika-ENGINE, remove_ex, remove (%s, %s) OK, size =%d\r\n", sUTF8_Keys.c_str(), sUTF8_Word.c_str(), (int)vdict.size());
          KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          return true;
     }

     KIKA_LOGD("Kika-ENGINE, remove_ex, remove (%s, %s) FAIL, size =%d\r\n", sUTF8_Keys.c_str(), sUTF8_Word.c_str(), (int)vdict.size());
     KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");

     return false;
}

bool userData_imp::append_ex(int _IME, const wchar_t *wsKeys, const wchar_t *wsPhrs, const MEM_INT32 iNext)
{
     KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
     KIKA_LOGD("Kika-ENGINE, append_ex, _IME =%d, file =%s\r\n", _IME, get_fname().c_str());

     string
     sUTF8_Keys =wchar_to_UTF8(wsKeys),
     sUTF8_Word =wchar_to_UTF8(wsPhrs);

     bool ret = append(_IME, wsKeys, wsPhrs, iNext);

     if ( ret ==true )
     {
          KIKA_LOGD("Kika-ENGINE, append_ex, append (%s, %s) OK, size =%d\r\n", sUTF8_Keys.c_str(), sUTF8_Word.c_str(), (int)vdict.size());
          KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          return true;
     }

     KIKA_LOGD("Kika-ENGINE, append_ex, append (%s, %s) FAIL, size =%d\r\n", sUTF8_Keys.c_str(), sUTF8_Word.c_str(), (int)vdict.size());
     KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");

     return false;
}
#endif

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct Cmp_UserData_VOCA
{
       PHRS_UI              In;

       Cmp_UserData_VOCA(PHRS_UI _In) : In(_In)
       {
       }

       bool operator()(const PHRS_UI& chk) const
       {
            if ( In.wsPhrs ==chk.wsPhrs )
            {
                 return true;
            }
            return false;
       }
};

bool userData_imp::isExist_UserData(int _IME, const wchar_t *wsPhrs)
{
     std::wstring
     wsContent =encrypt(wsPhrs, string(ENC_KEY_SZ));

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, isExist_UserData  total =%d\r\n", (int)vdict.size());
     #endif

     PHRS_UI
     ui2(this, _IME, L"", wsContent);

     PHRS_ITR
     ui_found =find_if(vdict.begin(), vdict.end(), Cmp_UserData_VOCA(ui2));

     if ( ui_found ==vdict.end() )
     {
          return false;
     }
     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void userData_imp::getAll_userData(int _IME, vector<PHRS_UI> &All)
{
     if ( vdict.size() >0 )
     {
          int
          index =0,
          total =vdict.size();

          #if defined(__DEBUG_LEARN_WORD) && defined(__DEBUG_LOG)
          {
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
              KIKA_LOGD("Kika-ENGINE, getAll_userData file =%s\r\n", get_fname().c_str());
              KIKA_LOGD("Kika-ENGINE, getAll_userData total =%d\r\n", total);
              KIKA_LOGD("Kika-ENGINE, --------------------------------------------\r\n");
          }
          #endif

          vector<PHRS_UI>::iterator
          bi =vdict.begin();

          while ( bi !=vdict.end() )
          {
                  All.push_back(*bi);

                  index++;

                  #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID) && defined(__ANDROID_MSG) || defined(__DEBUG_LEARN_WORD_EX)
                  {
                       string svoca1 =WstringToString(bi->wsKeys);
                       string svoca2 =WstringToString(bi->wsPhrs);

                       KIKA_LOGD("Kika-ENGINE, getAll_userData(%4d) k=%s, w=%s\r\n", index, svoca1.c_str(), svoca2.c_str());
                  }
                  #endif

                  bi++;
          }
     }
}

#include <unistd.h>
#include <ctime>

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void userData_imp::initial_UserData_load(imebase _imeX, const char *sFile, int _IME, int limitN)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, initial_UserData_load IME old %d=(%d) \r\n", getImeID(), _IME);
     #endif

     if ( imeID != _IME )
     {
          imeID =_IME;

          _imeX->clear_u();
          initial_UserData(_imeX, sFile, _IME, limitN);

          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_LEARN_WORD)
          KIKA_LOGD("Kika-ENGINE, initial_UserData_load IME new %d=(%d) \r\n", getImeID(), _IME);
          #endif
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void userData_imp::initial_UserData(imebase _imeX, const char *sFile, int _IME, int limitN)
{
     setlocale(LC_ALL, "");

     imeX =_imeX;
     sUserWordFname =sFile;

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) || defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, initial_UserData filename =%s\r\n", sUserWordFname.c_str());
     #endif

     int
     loadUserTotal =phrsLoadData(sUserWordFname.c_str());

     if ( loadUserTotal ==-1 )
     {
          #if defined(__TEST)
          printf("%s 檔案格式錯誤 (invalid data format) !!!!!\r\n", sUserWordFname.c_str());
          #else
          char aa[256] ={0};
          sprintf(aa, "%s 檔案格式錯誤 (invalid data format) !!!!!\r\n", sUserWordFname.c_str());
           #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, %s 檔案格式錯誤 (invalid data format) !!!!!\r\n", sUserWordFname.c_str());
           #else
          d_pintf_ex(aa);
           #endif
          #endif
          return;
     }
     else
     {
          #if defined(__TEST)
          printf("Import user vocabulary file =%s, total =%d\r\n", sUserWordFname.c_str(), loadUserTotal);
          #else
          char aa[256] ={0};
          sprintf(aa, "Import user vocabulary file =%s, total =%d\r\n", sUserWordFname.c_str(), loadUserTotal);
           #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, Import user vocabulary file =%s, total =%d\r\n", sUserWordFname.c_str(), loadUserTotal);
           #else
          d_pintf_ex(aa);
           #endif
          #endif
     }

     if ( vdict.size() >0 )
     {
          vector<PHRS_UI>::iterator
          bi =--vdict.end();

          bool
          isT9 =setT9(_IME);

          for ( int idx =0; bi >=vdict.begin(); bi-- )
          {
                wchar_t
                wsKey[MAX_INPUT_KEY] ={0};

                wstring
                wContent =bi->wsPhrs;

                #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID) && defined(__ANDROID_MSG) || defined(__DEBUG_LEARN_WORD_EX)
                {
                    string svoca1 =wchar_to_UTF8(bi->wsKeys.c_str());
                    string svoca2 =WstringToString(bi->wsPhrs);
                    KIKA_LOGD("Kika-ENGINE, Import user vocabulary, origin k=%s, w=%s\r\n", svoca1.c_str(), svoca2.c_str());
                }
                #endif

                wstring
                wContentEx;

                for ( auto c: wContent )
                {
                      wContentEx +=static_cast<wchar_t>(std::towlower(c));
                }

                #if defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                switch ( _IME )
                {
                         case IQQI_IME_Chinese_HK:
                              if ( bi->wsKeys.size() >MAX_INPUT_KEY )
                                   wcsncpy(wsKey, bi->wsKeys.c_str(), MAX_INPUT_KEY);
                              else
                                   wcscpy(wsKey, bi->wsKeys.c_str());
                              break;

                         case IQQI_IME_Changjie:
                         case IQQI_IME_Chinese_CN:
                         case IQQI_IME_Chinese_TW:
                              if (  bi->wsKeys.c_str()[0] >=0 && bi->wsKeys.c_str()[0] <='9' )
                              {
                                    wcscpy(wsKey, bi->wsKeys.c_str());
                              }
                              else
                              {
                                    gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
                                    gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
                              }
                              break;

                         default:
                              gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
                              gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
                }

                #else

                gMap.keyCode2NumbEx(_IME, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);
                gMap.keyCode2NumbEx(IQQI_IME_English_US, wContentEx.c_str(), wsKey, MAX_INPUT_KEY);

                #endif

                {
                       char aa[MAX_INPUT_KEY] ={0};
                       sprintf(aa, "LearnWord Import Add Key =%ls, Content =%ls\r\n",
                                   wsKey, wContent.c_str());
                       #if defined(__TEST)
                       printf("%s", aa);
                       #else
                       d_pintf(aa);
                       #endif
                }

                MEM_INT32
                stamp =imeX->getTimeStamp() +idx++;

                //clock_t
                //now =clock();
                //while ( clock() -now <100 );

                #if defined(__NO_PICKUPED_TO_FIRST)
                if ( isT9 ==false  )
                {
                     // kika engine will autoLoad userVoca at Initial(...)
                }
                else
                {
                     if ( imeX->append_u(_IME, wsKey, wContent.c_str(), stamp) ==false )
                     {
                     }
                }
                #else

                imeX->checkLimit_u(limitN);

                #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID) && defined(__ANDROID_MSG) || defined(__DEBUG_LEARN_WORD)
                {
                    string svoca1 =wchar_to_UTF8(wsKey);
                    string svoca2 =WstringToString(wContent);
                    KIKA_LOGD("Kika-ENGINE, import priority main k=%s, w=%s\r\n", svoca1.c_str(), svoca2.c_str());
                }
                #endif

                ////printf("wContent =%ls\r\n", wContent.c_str());

                if ( imeX->append_u(_IME, wsKey, wContent.c_str(), stamp) ==false )
                {
                }

                #endif

                #if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                gVoca_InnerX.append(wContent.c_str());
                #endif

                #if defined(__TEST)
                printf("add user Voca [ %ls ]\r\n", wContent.c_str());
                #else
                char aa[256] ={0};
                sprintf(aa, "add user Voca [ %ls ]\r\n", wContent.c_str());
                d_pintf_ex(aa);
                #endif
          }
     }
     imeX->load_u("");

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) || defined(__DEBUG_LEARN_WORD)
     KIKA_LOGD("Kika-ENGINE, initial_UserData [%s] load total 1=%d\r\n", sUserWordFname.c_str(), imeX->total_u());
     KIKA_LOGD("Kika-ENGINE, initial_UserData [%s] load total 2=%d\r\n", sUserWordFname.c_str(), (int)vdict.size());
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void userData_imp::reset_UserData(int _IME, imebase _imeX, const char *sFile)
{
     //string sUserDBPath =sUserDBPath;

     setlocale(LC_ALL, "");

     imeX =_imeX;

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, reset_UserData filename =%s", sFile);
     #endif

     int
     loadUserTotal =phrsLoadData(sFile);

     if ( loadUserTotal ==-1 )
     {
          #if defined(__TEST)
          printf("%s 檔案格式錯誤 (invalid data format) !!!!!\r\n", sFile);
          #else
          char aa[256] ={0};
          sprintf(aa, "%s 檔案格式錯誤 (invalid data format) !!!!!\r\n", sFile);
           #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, %s 檔案格式錯誤 (invalid data format) !!!!!\r\n", sFile);
           #else
          d_pintf_ex(aa);
           #endif
          #endif
          return;
     }
     else
     {
          #if defined(__TEST)
          printf("Import reset vocabulary file =%s, total =%d\r\n", sFile, loadUserTotal);
          #else
          char aa[256] ={0};
          sprintf(aa, "reset user vocabulary file =%s, total =%d\r\n", sFile, loadUserTotal);
           #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, reset user vocabulary file =%s, total =%d\r\n", sFile, loadUserTotal);
           #else
          d_pintf_ex(aa);
           #endif
          #endif
     }

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, reset user Total =%d\r\n", loadUserTotal);
     #endif

     if ( vdict.size() >0 )
     {
          vector<PHRS_UI>::iterator
          bi =--vdict.end();

          for ( ; bi >=vdict.begin(); bi-- )
          {
                wchar_t
                wsKey[MAX_INPUT_KEY] ={0};

                wstring
                wContent =bi->wsPhrs;

                wstring
                wIsMultiTap =encrypt(bi->wsKeys, string(ENC_KEY_SZ));

                #if defined(__DEBUG_MSG) && defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                {
                    string svoca1 =wchar_to_UTF8(wsKey);
                    string svoca2 =WstringToString(wContent);
                    KIKA_LOGD("Kika-ENGINE, import priority main k=%s, w=%s\r\n", svoca1.c_str(), svoca2.c_str());
                }
                #endif

                #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                KIKA_LOGD("Kika-ENGINE, reset user voc =%ls, tag =%c\r\n", wContent.c_str(), wIsMultiTap.c_str()[0]);
                #endif

                #if !defined(__SONIM)
                if ( wIsMultiTap.c_str()[0] =='m' )
                #endif
                {
                     vdict.erase(bi);

                     //2020/12/03
                     delKeys(_IME, bi->wsPhrs.c_str());

                     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                     KIKA_LOGD("Kika-ENGINE, reset user voc =%ls, remove this!!\r\n", wContent.c_str());
                     #endif

                     #if defined(__TEST)
                     printf("remove user Voca [ %ls ]\r\n", wContent.c_str());
                     #else
                     char aa[256] ={0};
                     sprintf(aa, "remove user Voca [ %ls ]\r\n", wContent.c_str());
                     d_pintf_ex(aa);
                     #endif
                }
          }
     }

     sUserWordFname =sFile;
     updateAndSave(false);

     //imeX->load_u("");
     initial_UserData(imeX, gUserDBPath.c_str(), _IME, LIMIT_USER_DB_COUNT);

     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, reset_UserData load total =%d\r\n", imeX->total_u());
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
#if !defined(__NO_KIKA_IME)

int  userData_imp::initial_KikaData(Dictionary* mDict)
{
     #if defined(__FOR_TRACE)
     KIKA_LOGD("Kika-ENGINE, initial_KikaData, mDict =%x", mDict);
     #endif

     if ( mDict ==0 )
          return 0;

     vector<PHRS_UI>::iterator
     bi =vdict.begin();

     try
     {
          for ( int i =0; bi !=vdict.end(); bi++ )
          {
                string
                sUTF8 =wchar_to_UTF8(bi->wsPhrs.c_str());

                if ( dictionary_add(mDict, sUTF8.c_str(), i++) ==false )
                {
                     return ERR_DICT_ADD_FAIL;
                }
          }
     }
     catch ( ... )
     {
     }
     return 0;
}

#endif


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  userData_imp::check_UserDB_Limit()
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, check_UserDB_Limit totoal =%d, curr =%d\r\n", LIMIT_USER_DB_COUNT, (int)vdict.size());
     #endif

     #if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)

     while ( vdict.size() >LIMIT_USER_DB_COUNT )
             vdict.erase(vdict.begin());

     return vdict.size();

     #else

     return 0;

     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  userData_imp::check_UserDB_Limit(int LimitCount)
{
     #if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)

     while ( vdict.size() >LimitCount )
             vdict.erase(vdict.begin());

     return vdict.size();

     #else

     return 0;

     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void userData_imp::updateAndSave(bool save_priority)
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, prepare save USER dict total =%d\r\n", (int)vdict.size());

     {
         vector<PHRS_UI>::iterator
         bi =vdict.begin();

         for ( int i =0; bi !=vdict.end(); bi++ )
         {
           string
           sUTF8 =wchar_to_UTF8(bi->wsPhrs.c_str());

           KIKA_LOGD("Kika-ENGINE, prepare save USER dict is =%s\r\n", sUTF8.c_str());
         }
     }

     #elif defined(__TEST) || defined(__DEBUG_MSG)
     char aa[256] ={0};
     sprintf(aa, "user dict size =%d\r\n", (int)vdict.size());
      #if defined(__TEST)
      d_pintf_ex(aa);
      #else
      d_pintf(aa);
      #endif
     #endif

     //if ( sUserWordFname.size() >0 )
     {
          #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
          KIKA_LOGD("Kika-ENGINE, Save user dict =%s\r\n", sUserWordFname.c_str());
          #elif defined(__TEST) || defined(__DEBUG_MSG)
          char aa[256] ={0};
          sprintf(aa, "Save user dict =%s\r\n", sUserWordFname.c_str());
           #if defined(__TEST)
           d_pintf_ex(aa);
           #else
           d_pintf(aa);
           #endif
          #endif

          if ( save_priority ==true )
          {
               #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
               KIKA_LOGD("Kika-ENGINE, Save pri-word size =%d\r\n", imeX->total_u());
               #endif

               if ( vdict.size() >0 && imeX->save_u(sUserWordFname.c_str()) ==false )
               {
                    #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
                    KIKA_LOGD("Kika-ENGINE, Save to pri-word size =%d\r\n", imeX->total_u());
                    #endif
               }
          }
          else //if ( vdict.size() >0  )
          {
               gUserFile.phrsSaveData(sUserWordFname.c_str());
          }
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void userData_imp::resetAndSave()
{
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)
     KIKA_LOGD("Kika-ENGINE, resetAndSave USER dict total =%d\r\n", (int)vdict.size());
     #endif

     gUserFile.phrsSaveData(sUserWordFname.c_str());
}




#if defined(__TEST)
#include <fstream>
#include <locale>
#include <regex>
using namespace std;

#include "DataCandidate.h"
#include "autotest_v1.h"
#include "autotest_kr.h"
#include "Tw1_Data_H.h"
#include "Tw2_Data_H.h"
#include "Py1_Data_H.h"
#include "Py2_Data_H.h"
#include "Py3_Data_H.h"
#include "Py4_Data_H.h"
#include "Py5_Data_H.h"
#include "Py6_Data_H.h"
#include "Py7_Data_H.h"
#include "Py8_Data_H.h"
#include "PyNxt1_Data_H.h"
#include "Hk1_Data_H.h"
#include "Hk2_Data_H.h"
#include "Hk3_Data_H.h"
#include "Hk4_Data_H.h"
#include "Hk5_Data_H.h"
#include "Hk6_Data_H.h"
#include "Vi1_Data_H.h"
#include "Vi2_Data_H.h"
#include "Vi3_Data_H.h"
#include "Thai1_Data_H.h"
#include "Thai2_Data_H.h"
#include "Thai3_Data_H.h"

#define KIKA_OK                        0
#define KIKA_NOT_OK                    -1

#define __FOR_T9

vector<wstring>                        gInput;
vector<wstring>                        gOutput;

struct ID_NAME
{
      int  ID;
      char name[40];
}
id_map[] =
{
      { 1, "English_US"},
      { 2, "French_CA"},
      { 3, "Portugese_BR"},
      { 4, "Spanish_US"},
      { 5, "Assamese"},
      { 6, "Bengali_IN"},
      { 7, "Bodo"},
      { 8, "Dogri"},
      { 9, "Gujarati"},
      {10, "Hindi"},
      {11, "Kannada"},
      {12, "Kashmiri"},
      {13, "Konkani"},
      {14, "Malayalam"},
      {15, "Manipuri"},
      {16, "Marathi"},
      {17, "Nepali"},
      {18, "Oriya"},
      {19, "Punjabi"},
      {20, "Sanskrit"},
      {21, "Santhali"},
      {22, "Sindhi"},
      {23, "Tamil"},
      {24, "Telugu"},
      {25, "Urdu"},
      {26, "Afrikaans"},
      {27, "Arabic"},
      {28, "Chinese_CN"},
      {29, "Dutch"},
      {30, "English_GB"},
      {31, "French_FR"},
      {32, "German"},
      {33, "Hungarian"},
      {34, "Indonesian"},
      {35, "Italian"},
      {36, "Malay"},
      {37, "Persian"},
      {38, "Polish"},
      {39, "Portugese_PT"},
      {40, "Romanian"},
      {41, "Russian"},
      {42, "Spanish_ES"},
      {43, "Swahili"},
      {44, "Thai"},
      {45, "Turkish"},
      {46, "Vietnamese"},
      {47, "Zulu"},
      {48, "Bengali_BD"},
      {49, "Bulgarian"},
      {50, "Croatian"},
      {51, "Czech"},
      {52, "Finnish"},
      {53, "Greek"},
      {54, "Kazakh"},
      {55, "Khmer"},
      {56, "Macedonian"},
      {57, "Serbian"},
      {58, "Sinhala"},
      {59, "Slovak"},
      {60, "Slovenian"},
      {61, "Swedish"},
      {62, "Tagalog"},
      {63, "Ukrainian"},
      {64, "Xhosa"},
      {65, "Albanian"},
      {66, "Armenian"},
      {67, "Azerbaijani"},
      {68, "Belarusian"},
      {69, "Bosnian"},
      {70, "Chinese_HK"},
      {71, "Chinese_TW"},
      {72, "Danish"},
      {73, "Estonian"},
      {74, "Georgian"},
      {75, "Hebrew"},
      {76, "Icelandic"},
      {77, "Lao"},
      {78, "Latvian"},
      {79, "Lithuanian"},
      {80, "Norwegian"},
      {81, "Uzbek (Cyrillic)"},
      {82, "Basque"},
      {83, "Galician"},
      {84, "Malagasy"},
      {85, "Yiddish"},
      {86, "Korean"},
      {87, "Catalan"},
      {88, "Japanese_KN"},
      {89, "Japanese_RM"},
      {90, "Changjie"},
      {91, "Quick_CJ"},
      {92, "Farsi"},
      {93, "Maithili"},
      {94, "Burmese_Z"},
      {95, "Burmese_U"},
      {97, "Tibetan"}
};

    struct LNG_TO_TCL
    {
           int lag_id;
           int tcl_id;
           int level;
    } lag_to_tcl[] =
    {
    { 1,  1, 1},
    { 2,  2, 1},
    { 3,  3, 1},
    { 4,  4, 1},
    { 5,  6, 1},
    { 6, 10, 1},
    { 7, 23, 1},
    { 8, 24, 1},
    { 9, 25, 1},
    {10, 26, 1},
    {11, 27, 1},
    {12, 28, 1},
    {13, 29, 1},
    {14, 30, 1},
    {15, 31, 1},
    {16, 32, 1},
    {17, 33, 1},
    {18, 34, 1},
    {19, 35, 1},
    {20, 36, 1},
    {21, 37, 1},
    {22, 38, 1},
    {23, 39, 1},
    {24, 40, 1},
    {25, 41, 1},
    {26, 42, 1},
    {27, 43, 1},
    {28, 44, 1},
    {29, 45, 1},
    {30, 46, 1},
    {31, 47, 1},
    {32, 48, 2},
    {33, 49, 2},
    {34, 50, 2},
    {35, 51, 2},
    {36, 52, 2},
    {37, 53, 2},
    {38, 54, 2},
    {39, 55, 2},
    {40, 56, 2},
    {41, 57, 2},
    {42, 58, 2},
    {43, 59, 2},
    {44, 60, 2},
    {45, 61, 2},
    {46, 62, 2},
    {47, 63, 2},
    {48, 64, 2},
    {49, 65, 3},
    {50, 66, 3},
    {51, 67, 3},
    {52, 68, 3},
    {53, 69, 3},
    {54, 70, 3},
    {55, 71, 3},
    {56, 72, 3},
    {57, 73, 3},
    {58, 74, 3},
    {59, 75, 3},
    {60, 76, 3},
    {61, 77, 3},
    {62, 78, 3},
    {63, 79, 3},
    {64, 80, 3},
    {65, 81, 3},
    {66,  5, 4},
    {67,  7, 4},
    {68,  8, 4},
    {69,  9, 4},
    {70, 11, 4},
    {71, 12, 4},
    {72, 13, 4},
    {73, 14, 4},
    {74, 15, 4},
    {75, 16, 4},
    {76, 17, 4},
    {77, 18, 4},
    {78, 19, 4},
    {79, 20, 4},
    {80, 21, 4},
    {81, 22, 4},
    {82, 82, 5},
    {83, 83, 5},
    {84, 84, 5},
    {85, 85, 5},
    {86, 86, 1},
    {87, 87, 1},
    {88, 88, 1}
    };

    struct DICT_MAP
    {
           int imeID;
           string dict_main;
           string dict_user;
    } dictMap[] =
    {
        {IQQI_IME_English_US,      "English_US.dict", ""},
        {IQQI_IME_French_CA,       "French_CA.dict", ""},
        {IQQI_IME_Portuguese_BR,   "Portuguese_BR.dict", ""},
        {IQQI_IME_Spanish_US,      "Spanish_US.dict", ""},
        {IQQI_IME_Assamese,        "", ""},
        {IQQI_IME_Bengali_IN,      "Bengali_IN.dict", ""},
        {IQQI_IME_Bodo,            "", ""},
        {IQQI_IME_Dogri,           "", ""},
        {IQQI_IME_Gujarati,        "", ""},
        {IQQI_IME_Hindi,           "Hindi.dict", ""},
        {IQQI_IME_Kannada,         "", ""},
        {IQQI_IME_Kashmiri,        "", ""},
        {IQQI_IME_Konkani,         "", ""},
        {IQQI_IME_Malayalam,       "", ""},
        {IQQI_IME_Manipuri,        "", ""},
        {IQQI_IME_Marathi,         "", ""},
        {IQQI_IME_Nepali,          "", ""},
        {IQQI_IME_Oriya,           "", ""},
        {IQQI_IME_Punjabi,         "", ""},
        {IQQI_IME_Sanskrit,        "", ""},
        {IQQI_IME_Santhali,        "", ""},
        {IQQI_IME_Sindhi,          "", ""},
        {IQQI_IME_Tamil,           "Tamil.dict", ""},
        {IQQI_IME_Telugu,          "Telugu.dict", ""},
        {IQQI_IME_Urdu,            "Urdu.dict", ""},
        {IQQI_IME_Afrikaans,       "Afrikaans.dict", ""},
        {IQQI_IME_Arabic,          "Arabic.dict", ""},
        {IQQI_IME_Chinese_CN,      "Chinese_CN.dict", ""},
        {IQQI_IME_Dutch,           "Dutch.dict", ""},
        {IQQI_IME_English_GB,      "English_GB.dict", ""},
        {IQQI_IME_French_FR,       "French_FR.dict", ""},
        {IQQI_IME_German,          "German.dict", ""},
        {IQQI_IME_Hungarian,       "Hungarian.dict", ""},
        {IQQI_IME_Indonesian,      "Indonesian.dict", ""},
        {IQQI_IME_Italian,         "Italian.dict", ""},
        {IQQI_IME_Malay,           "Malay.dict", ""},
        {IQQI_IME_Persian,         "Persian.dict", ""},
        {IQQI_IME_Polish,          "Polish.dict", ""},
        {IQQI_IME_Portuguese_PT,   "Portuguese_PT.dict", ""},
        {IQQI_IME_Romanian,        "Romanian.dict", ""},
        {IQQI_IME_Russian,         "Russian.dict", ""},
        {IQQI_IME_Spanish_ES,      "Spanish_ES.dict", ""},
        {IQQI_IME_Swahili,         "Swahili.dict", ""},
        {IQQI_IME_Thai,            "Thai.dict", ""},
        {IQQI_IME_Turkish,         "Turkish.dict", ""},
        {IQQI_IME_Vietnamese,      "Vietnamese.dict", ""},
        {IQQI_IME_Zulu,            "Zulu.dict", ""},
        {IQQI_IME_Bengali_BD,      "Bengali_BD.dict", ""},
        {IQQI_IME_Bulgarian,       "Bulgarian.dict", ""},
        {IQQI_IME_Croatian,        "Croatian.dict", ""},
        {IQQI_IME_Czech,           "Czech.dict", ""},
        {IQQI_IME_Finnish,         "Finnish.dict", ""},
        {IQQI_IME_Greek,           "Greek.dict", ""},
        {IQQI_IME_Kazakh,          "Kazakh.dict", ""},
        {IQQI_IME_Khmer,           "Khmer.dict", ""},
        {IQQI_IME_Macedonian,      "Macedonian.dict", ""},
        {IQQI_IME_Serbian,         "Serbian.dict", ""},
        {IQQI_IME_Sinhala,         "Sinhala.dict", ""},
        {IQQI_IME_Slovak,          "Slovak.dict", ""},
        {IQQI_IME_Slovenian,       "Slovenian.dict", ""},
        {IQQI_IME_Swedish,         "Swedish.dict", ""},
        {IQQI_IME_Tagalog,         "Tagalog.dict", ""},
        {IQQI_IME_Ukrainian,       "Ukrainian.dict", ""},
        {IQQI_IME_Xhosa,           "Xhosa.dict", ""},
        {IQQI_IME_Albanian,        "Albanian.dict", ""},
        {IQQI_IME_Armenian,        "Armenian.dict", ""},
        {IQQI_IME_Azerbaijani,     "Azerbaijani.dict", ""},
        {IQQI_IME_Belarusian,      "Belarusian.dict", ""},
        {IQQI_IME_Bosnian,         "Bosnian.dict", ""},
        {IQQI_IME_Chinese_HK,      "Chinese_HK.dict", ""},
        {IQQI_IME_Chinese_TW,      "Chinese_TW.dict", ""},
        {IQQI_IME_Danish,          "Danish.dict", ""},
        {IQQI_IME_Estonian,        "Estonian.dict", ""},
        {IQQI_IME_Georgian,        "Georgian.dict", ""},
        {IQQI_IME_Hebrew,          "Hebrew.dict", ""},
        {IQQI_IME_Icelandic,       "Icelandic.dict", ""},
        {IQQI_IME_Lao,             "Lao.dict", ""},
        {IQQI_IME_Latvian,         "Latvian.dict", ""},
        {IQQI_IME_Lithuanian,      "Lithuanian.dict", ""},
        {IQQI_IME_Norwegian,       "Norwegian.dict", ""},
        {IQQI_IME_Uzbek_Cyrillic,  "Uzbek_Cyrillic.dict", ""},
        {IQQI_IME_Basque,          "Basque.dict", ""},
        {IQQI_IME_Galician,        "Galician.dict", ""},
        {IQQI_IME_Malagasy,        "", ""},
        {IQQI_IME_Yiddish,         "", ""},
        {IQQI_IME_Korean,          "Korean.dict", ""},
        {IQQI_IME_Catalan,         "Catalan.dict", ""},
        {IQQI_IME_Japanese_KN,     "", ""},
        {IQQI_IME_Japanese_RM,     "", ""},
        {IQQI_IME_Changjie,        "Changjie.dict", ""},
        {IQQI_IME_Quick_CJ,        "Quick_CJ.dict", ""},
        {IQQI_IME_Farsi,           "Farsi.dict", ""},
        {IQQI_IME_Maithili,        "Maithili.dict", ""},
        {IQQI_IME_Burmese_Z,       "Burmese_Z.dict", ""},
        {IQQI_IME_Burmese_U,       "Burmese_U.dict", ""},
        {IQQI_IME_Tibetan,         "Tibetan.dict", ""},
        #if defined(__HINDI_BASE)
        {IQQI_IME_Hindi_01,        "Hindi_01.dict", ""}
        #endif
    };



void ss_printf(const wchar_t * msg)
{
     printf("%ls", msg);
     //wcout <<msg;//<<endl;
     gOutput.push_back(msg);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int                          gImeID =0;
int                          gGetNumber   =0;
int                          gTotalSelect =0;
string                       gExportDict  ="";

wstring                      gwsVoca_Del;
wstring                      gwsVoca_Add;
wstring                      gwsVoca_Reset;
string                       gwsVoca_Del_Import;
string                       gwsVoca_Add_Import;
string                       gwsVoca_Update_Import;

wstring                      gwsPhrs_Key;
wstring                      gwsPhrs_Add;
wstring                      gwsPhrs_Del;
wstring                      gChkExistVoca;

bool                         gForFile  =false;
bool                         gNextword =false;
bool                         gMultiTap =false;
bool                         gwsVoca_Add_ImportForcePriority =true;

#if !defined(__NO_KOREAN)
bool                         gKorean_Testing =false;
#endif
bool                         gMultiTap_Looping =false;

bool                         gChkExist =false;
bool                         gChkLimit =false;

int                          gChkLimitCount =0;


map<char, wstring>
gkKKey =
{
       {'0', L"ㅇ"},
       {'1', L"ㅁ"},
       {'2', L"ㅣ"},
       {'3', L"ㆍ"},
       {'4', L"："},
       {'5', L"ㅡ"},
       {'6', L"ㄱ"},
       {'7', L"ㅋ"},
       {'8', L"ㄲ"},
       {'9', L"ㄴ"},
       {'a', L"ㄹ"},
       {'b', L"ㄷ"},
       {'c', L"ㅌ"},
       {'d', L"ㄸ"},
       {'e', L"ㅂ"},
       {'f', L"ㅍ"},
       {'g', L"ㅃ"},
       {'h', L"ㅅ"},
       {'i', L"ㅎ"},
       {'j', L"ㅆ"},
       {'k', L"ㅈ"},
       {'l', L"ㅊ"},
       {'m', L"ㅉ"},
       {'x', L"⌫"},
       {'z', L"cls"}
};


//-----------------------------------------------------------------------------
// for autoTest
//-----------------------------------------------------------------------------
autotest_v1                  autoX;

vector<KEY_LANG>::iterator   auto_ii;
vector<KEY_ANS>::iterator    auto_ii2;

struct RECORD_DATA
{
       int                   count;
       vector<string>        imeName;
};

bool                         gAutoTest_succ;
RECORD_DATA                  gAutoTest_OK;
RECORD_DATA                  gAutoTest_Err;
wstring                      gAutoTest_Result;


//-----------------------------------------------------------------------------
void exportDict(const char *fName, int _IME)
{
     bool
     isT9 =setT9(_IME);

     if ( isT9 ==false )
     {
          printf("not support IME (not T9)\r\n");
          return;
     }
     //gImeX->exportDict(fName);
}

//-----------------------------------------------------------------------------
void replaceAll_s(std::string& source, const std::string& from, const std::string& to)
{
     std::string newString;
     newString.reserve(source.length());  // avoids a few memory allocations

     std::string::size_type lastPos = 0;
     std::string::size_type findPos;

     while ( std::string::npos != (findPos = source.find(from, lastPos)) )
     {
             newString.append(source, lastPos, findPos - lastPos);
             newString += to;
             lastPos = findPos + from.length();
     }

     // Care for the rest after last occurrence
     newString += source.substr(lastPos);

     source.swap(newString);
}

//-----------------------------------------------------------------------------
int testAction(int imeID, wchar_t* wsKeyin, int tabCount, bool &newID)
{
    wstring
    wsNewKeyCode =wsKeyin;
    replaceAll(wsNewKeyCode, L"@", L"*");
    wcscpy(wsKeyin, wsNewKeyCode.c_str());

    if ( gOutVer ==true )
    {
         ss_printf(L"\r\n-------------------------------------------------------------------------\r\n");
    }

    wchar_t
    ss[1024] ={0};

    int
    wsLen =wcslen(wsKeyin);

    if ( gOutVer ==true )
    {
         swprintf(ss, 1024, L"imeID, =%d, wsKeyin =%ls, wsLen =%d\r\n", imeID, wsKeyin, wsLen);
         ss_printf(ss);
    }

    char IME_ErrorList[32] = {0};
    char *dictname =NULL;

    string sDictMain;
    string sDictUser;
    string sDictPhrs;

    int
    maxIME =sizeof(dictMap) /sizeof(DICT_MAP);

    if ( !gwsVoca_Add.empty() || !gwsVoca_Add_Import.empty() || !gwsVoca_Update_Import.empty() )
    {
         imeID =gImeID;
    }
    else if ( !gwsVoca_Reset.empty() || !gwsVoca_Del.empty() || !gwsVoca_Del_Import.empty() )
    {
         imeID =gImeID;
    }

    if ( imeID ==IQQI_IME_Korean && gMultiTap ==false )
    {
         wstring
         wNewKey;

         for ( int i =0; wsKeyin[i]; i++ )
         {
               wstring
               wCH =gkKKey[wsKeyin[i]];

               wNewKey +=wCH;
         }

         wcscpy(wsKeyin, wNewKey.c_str());

         printf("New Korean Keyin =%ls\r\n", wsKeyin);
    }

    //-------------------------------------------------------------------------
    int
    su =-1;

    for ( int i =0; i <maxIME; i++ )
    {
          if ( imeID ==dictMap[i].imeID )
          {
               dictname =(char *)dictMap[i].dict_main.c_str();

               sDictMain ="./edict/";
               sDictMain +=dictname;

               if ( sDictMain.find(".dict") !=std::string::npos )
               {
                    sDictPhrs ="./edict/";
                    sDictPhrs +=dictname;

                    sDictPhrs.replace(sDictPhrs.find(".dict"), sizeof(".phrs")-1, ".phrs");

                    #if defined(__TEST) || defined(__FOR_ANDROID) ||defined(__ANDROID_APP)
                    sDictUser ="./edict/udict/";
                    #else
                    sDictUser ="./edict/";
                    #endif
                    sDictUser +=dictname;

                    sDictUser.replace(sDictUser.find(".dict"), sizeof("_user")-1, "_user");
               }

               if ( gChkExist ==true )
               {
                    string
                    sBdict ="./edict/";
                    sBdict +=dictname;

                    sBdict.replace(sBdict.find(".dict"), sizeof(".bict")-1, ".bict");

                    int
                    retV =IQQI_InitialDictionary(imeID, sBdict.c_str());

                    if ( retV !=0 )
                    {
                         printf("[%s] not found!!\r\n", sBdict.c_str());
                    }
                    else
                    {
                         printf("[%s] load Succ!!\r\n", sBdict.c_str());
                    }
               }

               su = IQQI_Initial(imeID, (char *)sDictMain.c_str(), (char *)sDictUser.c_str(), (char *)sDictPhrs.c_str(), IME_ErrorList);

               printf("load Main %s, su =%d\r\n", (char *)sDictMain.c_str(), su);

               if ( gExportDict.length() )
               {
                    exportDict(gExportDict.c_str(), imeID);
                    exit(1);
               }

               break;
          }
    }

    //-------------------------------------------------------------------------
    #if defined(__USER_DIC_SAVE_CMD)
    char
    sUserWordFname[256] ={0};// ="./" +imeID + "_user.word";

    sprintf(sUserWordFname, "./%d_user.word", gImeID);

    printf("%s loading..... \r\n", sUserWordFname);

    int
    loadUserTotal =gUserFile.phrsLoadData(sUserWordFname);

    if ( loadUserTotal ==-1 )
    {
         printf("%s 檔案格式錯誤 (invalid data format) !!!!!\r\n", sUserWordFname);
    }
    else
    {
         gUserFile.initial_UserData(gImeID);
         printf("%s load OK!! (Total =%d) \r\n", sUserWordFname, loadUserTotal);
    }
    #endif

    //-------------------------------------------------------------------------
    #if defined(__USER_DIC)
    if ( !gwsVoca_Add.empty() )
    {
         int
         retVal =IQQI_LearnWord(gImeID, (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str());

         printf("Add retVal =%d\r\n", retVal);
         showErr(retVal);

         #if defined(__USER_DIC_SAVE_CMD)
         if ( retVal ==0 )
         {
              gUserFile.append_keys(gImeID, (wchar_t *)L"", (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str());
              gUserFile.phrsSaveData(sUserWordFname);
         }
         #endif

         exit(1);
    }

    //-------------------------------------------------------------------------
    // Check Exist Vocabulary
    //-------------------------------------------------------------------------
    if ( gChkExist ==true )
    {
         bool
         isExist =IQQI_IsVocabularyExist(imeID, gChkExistVoca.c_str());

         printf("\r\n[%ls] exist =%d\r\n\r\n", gChkExistVoca.c_str(), isExist);
         exit(1);
    }

    if ( gChkLimit ==true )
    {
         int
         afCount =gUserFile.check_UserDB_Limit(gChkLimitCount);

         gUserFile.updateAndSave(false);

         printf("\r\n Check UserDB limit = %d, after =%d\r\n", gChkLimitCount, afCount);
         exit(1);
    }

    //-------------------------------------------------------------------------
    // 2020/04/16
    //-------------------------------------------------------------------------
    if ( !gwsVoca_Update_Import.empty() )
    {
        /*
         int
         Length =gwsVoca_Update_Import.size();

         const char*
         ContentTable =gwsVoca_Update_Import.c_str();

         const char
         CH =ContentTable[Length -1];

         if ( CH !=KEY_TOKEN ) // |
         {
              printf("\r\nAdd import last character [0x%x] of ContentTable (Len =%d) must be " RED " '@'\r\n\r\n" RESET,
                     CH, Length);

              if ( gDiagnostic ==true )
                   return ERR_DICT_INVALID_LAST_CH;

              exit(ERR_DICT_INVALID_LAST_CH);
         }

         int
         retVal =LearnWordUpdateByTokenEx(gImeID, (char *)"", (char *)ContentTable, Length, gwsVoca_Add_ImportForcePriority, false);
         if ( retVal ==0 )
              printf("Add retVal =%d\r\n\r\n", retVal);
         else
              printf("Add retVal =" RED "%d\r\n\r\n" RESET, retVal);
         showErr(retVal);

         if ( gDiagnostic ==true )
              return retVal;

         exit(retVal);*/

         int
         ret = IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)L"[[begin]]", true, false);
         ret = IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)L"aaa",       true, false);
         ret = IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)L"[[end]]",   true, false);
         ret = IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)L"[[begin]]", true, false);
         ret = IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)L"[[end]]",   true, false);

         exit(0);
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    if ( !gwsVoca_Add_Import.empty() )
    {
         replaceAll_s( gwsVoca_Add_Import, "@", "|");

         int
         Length =gwsVoca_Add_Import.size();

         const char*
         ContentTable =gwsVoca_Add_Import.c_str();

         const char
         CH =ContentTable[Length -1];

         if ( CH !=KEY_TOKEN ) // |
         {
              printf("\r\nAdd import last character [0x%x] of ContentTable (Len =%d) must be " RED " '@'\r\n\r\n" RESET,
                     CH, Length);

              if ( gDiagnostic ==true )
                   return ERR_DICT_INVALID_LAST_CH;

              exit(ERR_DICT_INVALID_LAST_CH);
         }

         int
         retVal =IQQI_LearnWordByTokenEx(gImeID, (char *)"", (char *)ContentTable, Length, gwsVoca_Add_ImportForcePriority, false);

         #if defined(__USER_DIC_SAVE_CMD)
         if ( retVal ==0 )
         {
              for ( long b =0, e =0; b <Length; e++ ) //auto x :tokens )
              {
                    if ( ContentTable[e] ==KEY_TOKEN ) // |
                    {
                         try
                         {
                              int
                              sLen =(e -b);

                              if ( sLen >MAX_LEARN_WORD_LEN )
                              {
                                   continue;
                              }

                              {
                                  char aa[256] ={0};
                                  sprintf(aa, "Add import b=%ld, e=%ld, sLen =%d\r\n", b, e, sLen);
                                  printf("%s", aa);
                              }

                              char
                              sIn[MAX_LEARN_WORD_LEN +1] ={0};

                              memcpy(sIn, ContentTable +b, sLen);
                              sIn[sLen] =0;

                              wstring
                              wsWord =utf8_to_utf16(sIn);

                              b =e +1;

                              {
                                  char aa[256] ={0};
                                  sprintf(aa, "Add import get %s to Learn\r\n", sIn);
                                  printf("%s", aa);
                              }

                              gUserFile.append_keys(gImeID, (wchar_t *)L"", (wchar_t *)L"", (wchar_t *)wsWord.c_str());
                         }
                         catch ( ... )
                         {
                         }
                    }
              }

              gUserFile.phrsSaveData(sUserWordFname);
         }
         #endif

         if ( retVal ==0 )
              printf("Add retVal =%d\r\n\r\n", retVal);
         else
              printf("Add retVal =" RED "%d\r\n\r\n" RESET, retVal);
         showErr(retVal);

         if ( gDiagnostic ==true )
              return retVal;

         exit(retVal);
    }

    //-------------------------------------------------------------------------
    if ( !gwsVoca_Reset.empty() )
    {
         int
         retVal =IQQI_ResetLearnedWordFromFile(gImeID, gUserDBPath.c_str());

         printf("ResetLearnedWordFromFile retVal =%d\r\n", retVal);
         showErr(retVal);

         if ( gDiagnostic ==true )
              return retVal;

         exit(retVal);
    }
    //-------------------------------------------------------------------------
    if ( !gwsVoca_Del.empty() )
    {
         int
         retVal =IQQI_DeleLearnedWord(gImeID, (wchar_t *)L"", (wchar_t *)gwsVoca_Del.c_str());

         printf("Del retVal =%d\r\n", retVal);
         showErr(retVal);

         #if defined(__USER_DIC_SAVE_CMD)
         if ( retVal ==0 )
         {
              // delete UserData (同時刪除 'u'(Import) & 'm'(MultiTap))

              #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)

              // delete UserData (同時刪除 'u'(Import) & 'm'(MultiTap))
              gUserFile.remove_ex(gImeID, (wchar_t *)L"m", (wchar_t *)gwsVoca_Del.c_str());
              gUserFile.remove_ex(gImeID, (wchar_t *)L"u", (wchar_t *)gwsVoca_Del.c_str());

              #else

              gUserFile.remove(gImeID, (wchar_t *)L"m", (wchar_t *)gwsVoca_Del.c_str());
              gUserFile.remove(gImeID, (wchar_t *)L"u", (wchar_t *)gwsVoca_Del.c_str());

              #endif

              gUserFile.phrsSaveData(sUserWordFname);
         }
         #endif

         if ( gDiagnostic ==true )
              return retVal;

         exit(retVal);
    }

    if ( !gwsVoca_Del_Import.empty() )
    {
         replaceAll_s( gwsVoca_Del_Import, "@", "|");

         int
         Length =gwsVoca_Del_Import.size();

         const char*
         ContentTable =gwsVoca_Del_Import.c_str();

         const char
         CH =ContentTable[Length -1];

         if ( CH !=KEY_TOKEN ) // |
         {
              printf("\r\nDel import last character [0x%x] of ContentTable (Len =%d) must be " RED " '@'\r\n\r\n" RESET,
                     CH, Length);

              if ( gDiagnostic ==true )
                   return ERR_DICT_INVALID_LAST_CH;

              exit(ERR_DICT_INVALID_LAST_CH);
         }

         int
         retVal =0;

         #if defined(__USER_DIC_SAVE_CMD)
         {
              for ( long b =0, e =0; e < Length && b <Length; e++ ) //auto x :tokens )
              {
                    if ( ContentTable[e] ==KEY_TOKEN ) // |
                    {
                         try
                         {
                              int
                              sLen =(e -b);

                              if ( sLen >MAX_LEARN_WORD_LEN )
                              {
                                   continue;
                              }

                              {
                                  char aa[256] ={0};
                                  sprintf(aa, "Del import b=%ld, e=%ld, sLen =%d\r\n", b, e, sLen);
                                  printf("%s", aa);
                              }

                              char
                              sIn[MAX_LEARN_WORD_LEN +1] ={0};

                              memcpy(sIn, ContentTable +b, sLen);
                              sIn[sLen] =0;

                              wstring
                              wsWord =utf8_to_utf16(sIn);

                              b =e +1;

                              {
                                  char aa[256] ={0};
                                  sprintf(aa, "Del import get %s to Remove\r\n", sIn);
                                  printf("%s", aa);
                              }

                              retVal =IQQI_DeleLearnedWord(gImeID, (wchar_t *)L"", (wchar_t *)wsWord.c_str());

                              printf("Del retVal =%d\r\n", retVal);
                              showErr(retVal);

                              // delete UserData (同時刪除 'u'(Import) & 'm'(MultiTap))

                              #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG)

                              gUserFile.remove_ex(gImeID, (wchar_t *)L"m", (wchar_t *)wsWord.c_str());
                              gUserFile.remove_ex(gImeID, (wchar_t *)L"u", (wchar_t *)wsWord.c_str());

                              #else

                              gUserFile.remove(gImeID, (wchar_t *)L"m", (wchar_t *)wsWord.c_str());
                              gUserFile.remove(gImeID, (wchar_t *)L"u", (wchar_t *)wsWord.c_str());

                              #endif
                         }
                         catch ( ... )
                         {
                         }
                    }
              }
              gUserFile.phrsSaveData(sUserWordFname);

              printf("\r\n");
         }
         #endif

         if ( gDiagnostic ==true )
              return retVal;

         exit(retVal);
    }

    #endif

    //-------------------------------------------------------------------------

    #if defined(__PHRS_DIC)
    int
    loadVal =gPhrs->phrsLoadData(gPhrsDBPath.c_str());

    if ( loadVal ==0 )
    {
         if ( gOutVer ==true )
         {
              printf("Load %s\r\n", gPhrsDBPath.c_str());
              showErr(ERR_PHRS_LOAD_FAIL);
         }
    }
    else
    {
         printf("Load Phrase Count =%ld\r\n", gPhrs->count());
    }

    if ( su ==0 && !gwsPhrs_Add.empty() )
    {

         int retVal =IQQI_AddPhrase(gImeID, (wchar_t *)gwsPhrs_Key.c_str(), (wchar_t *)gwsPhrs_Add.c_str());
         if ( retVal ==0 )
         {
              #if defined(__PHRS_DIC_SAVE)
              //gPhrs->save(gPhrsDBPath.c_str());
              #endif
         }
         printf("Add retVal =%d\r\n", retVal);
         showErr(retVal);
         exit(1);
    }

    if ( su ==0 && !gwsPhrs_Del.empty() )
    {
         int retVal =IQQI_RemovePhrase(gImeID, (wchar_t *)gwsPhrs_Key.c_str(), (wchar_t *)gwsPhrs_Del.c_str());
         if ( retVal ==0 )
         {
              #if defined(__PHRS_DIC_SAVE)
              //gPhrs->save(gPhrsDBPath.c_str());
              #endif
         }
         printf("del retVal =%d\r\n", retVal);
         showErr(retVal);
         exit(1);
    }
    #endif

    swprintf(ss, 1024, L"Initial DB, su =%d\r\n\r\n", su);
    //ss_printf(ss);

    #if !defined(__TEST_VOCA)

    //-------------------------------------------------------------------------
    wchar_t
    wsOut[4096] ={0};
    wchar_t
    wsLine[4096] ={0};

    int
    GetItem =gGetNumber;

    if ( GetItem ==0 )
    {
         GetItem =MAX_ENG_WORD_GET_NUMBER;

         switch ( imeID )
         {
                  case IQQI_IME_Changjie:
                  case IQQI_IME_Chinese_CN:
                  case IQQI_IME_Chinese_TW:
                  case IQQI_IME_Chinese_HK:
                       GetItem =MAX_CHI_WORD_GET_NUMBER;
                       break;

                  default:
                       break;
         }
    }

    if ( wsLen ==0 )
    {
         return 0;
    }

    if ( gMultiTap ==false )
    {
         //CandidateCH
         //gCandVoca;

         switch ( imeID )
         {
                  case IQQI_IME_Changjie:
                  case IQQI_IME_Chinese_CN:
                  case IQQI_IME_Chinese_TW:
                  case IQQI_IME_Chinese_HK:
                       if ( gDiagnostic ==true )
                            gCandVoca.alloc(GetItem, MAX_ENG_WORD_LENGTH);
                       else
                            gCandVoca.alloc(GetItem, MAX_CHI_WORD_LENGTH);
                       break;

                  default:
                       gCandVoca.alloc(GetItem, MAX_ENG_WORD_LENGTH);
         }

         if ( gNextword ==false )
         {
              int
              kaiOs_idx  =0,
              totalClass =0;

              wstring
              wsKeyCodeMatch;

              CandidateCH
              CandClass;
              CandClass.alloc(MAX_GROUP_COUNT, MAX_GROUP_LENGTH);

              switch ( imeID )
              {
                       case IQQI_IME_Chinese_CN:
                       case IQQI_IME_Chinese_TW:
                            totalClass =gKaiOS_fmt ==true ? IQQI_GetGrouping(imeID, wsKeyin, 0, 20, CandClass.pointer()) :0;
                            if ( totalClass >0 )
                            {
                                  kaiOs_idx =gKaiOS_idx -1;

                                  while ( kaiOs_idx >totalClass -1 )
                                          kaiOs_idx -=totalClass;

                                  for ( int i =0; i <totalClass; i++ )
                                  {
                                        if ( i ==kaiOs_idx )
                                        {
                                             wsKeyCodeMatch =CandClass.record(i);

                                             wstring
                                             wsKeyinEx =wsKeyin;

                                             wsKeyinEx.append(L":");
                                             wsKeyinEx.append(wsKeyCodeMatch);

                                             IQQI_GetCandidates(imeID, (wchar_t*)wsKeyinEx.c_str(), false, 3, 0, GetItem, gCandVoca.pointer());
                                             break;
                                        }
                                  }
                            }
                            else
                            {
                                  IQQI_GetCandidates(imeID, wsKeyin, false, 3, 0, GetItem, gCandVoca.pointer());
                            }
                            break;

                       default:
                            IQQI_GetCandidates(imeID, wsKeyin, false, 3, 0, GetItem, gCandVoca.pointer());
                            break;
              }

              int
              TotalWord = IQQI_GetCandidateCount(0, wsKeyin, false, 0);

              //---------------------------------------------------------------
              vector<VCandidate>::iterator
              ii =vdict_type.begin();

              if ( gAutoTest ==false )
              {
                   //----------------------------------------------------------
                   // output header
                   //----------------------------------------------------------
                   if ( gDiagnostic ==false && gOutVer ==true )
                   {
                        ss_printf(L"-------------------------------------------------------------------------\r\n" );
                        swprintf(ss, 1024, L"[ %2d, %s ] - [ %s ]\r\n", imeID, id_map[imeID -1].name, dictname);
                        ss_printf(ss);
                        swprintf(ss, 1024, L"TotalWord =%2d\r\n", TotalWord);
                        ss_printf(ss);
                        ss_printf(L"-------------------------------------------------------------------------\r\n");
                   }

                   if ( gDiagnostic ==false && gMultiTap_Looping ==false && gOutVer ==false && newID ==true )
                   {
                        swprintf(ss, 1024, L"[ %2d, %s ] - [ %s ]\r\n", imeID, id_map[imeID -1].name, dictname);
                        ss_printf(ss);

                        printf("Length\tKeyin\t");
                        for ( int i =0; i <GetItem; i++ )
                        {
                              printf("#%d\t", i +1);
                        }
                        printf("\r\n");

                        newID =false;
                   }

                   //----------------------------------------------------------
                   // output header
                   //----------------------------------------------------------
                   if ( gDiagnostic ==false && gOutVer ==false )
                   {
                        swprintf(wsLine, 4096, L"%d\t%ls", wsLen, wsKeyin);
                        wcscat(wsOut, wsLine);
                        wcscat(wsOut, L"\t");
                   }

                   int i =0;
                   for ( i =0; i <TotalWord; i++, ii++ )
                   {
                         if ( gDiagnostic ==true )
                         {
                              if ( i >gDiagnosticTestN -1 )
                                   break;

                              if ( i >0 )
                                   gDiagnosticDebug +=L" ";

                              gDiagnosticDebug +=gCandVoca.record(i);
                         }

                         else if ( gOutVer ==true )
                         {
                              if ( ii->vtype == VTYPE_PHRS )
                                   printf(CYN "(phrs)" RESET "-pvoca(%2d) = %ls\r\n", i +1, gCandVoca.record(i));
                              else if ( ii->vtype == VTYPE_USER )
                                   printf(CYN "(user)-pvoca(%2d) = %ls\r\n" RESET, i +1, gCandVoca.record(i));
                              else
                                   printf("[main]-pvoca(%2d) = %ls\r\n", i +1, gCandVoca.record(i));
                         }
                         else
                         {
                               swprintf(wsLine, 4096, L"%ls\t", gCandVoca.record(i));
                               wcscat(wsOut, wsLine);
                         }
                   }
                   if ( gDiagnostic ==false && gOutVer ==false )
                   {
                        //if ( gMultiTap_Looping ==false )
                        {
                             for ( ; i <GetItem; i++ )
                             {
                                   swprintf(wsLine, 4096, L"[null]\t");
                                   wcscat(wsOut, wsLine);
                             }
                        }
                        printf("%ls\r\n", wsOut);
                   }

                   if ( gKaiOS_fmt ==true && (imeID ==IQQI_IME_Chinese_CN || imeID ==IQQI_IME_Chinese_TW) )
                   {
                        wstring
                        wsFmt;

                        for ( i =0; i <totalClass; i++ )
                        {
                              if ( i >0 )
                              {
                                   wsFmt.append(L"|");
                              }
                              if ( i ==kaiOs_idx )
                              {
                                   wsFmt.append(L">");
                              }
                              swprintf(wsLine, 4096, L"%ls", CandClass.record(i));
                              wsFmt.append(wsLine);
                        }
                        if ( totalClass )
                        {
                             wsFmt.append(L"||");

                             for ( i =0; i <TotalWord; i++ )
                             {
                                   if ( i >0 )
                                   {
                                        wsFmt.append(L"|");
                                   }
                                   swprintf(wsLine, 4096, L"%ls", gCandVoca.record(i));
                                   wsFmt.append(wsLine);
                             }
                        }
                        printf("\r\n--------------------------------------------------------\r\n");
                        printf("模擬手機上的輸出, 選了(%ls)\r\n", wsKeyCodeMatch.c_str());
                        printf("--------------------------------------------------------\r\n");
                        printf("%ls\r\n", wsFmt.c_str());
                        printf("--------------------------------------------------------\r\n");
                   }
              }
              else
              {
                     vector<wstring>::iterator
                     auto_ii3 =auto_ii2->ans.begin();

//                     if ( TotalWord <auto_ii->count )
//                     {
//                          //error
//                          gAutoTest_succ =false;
//                     }
//                     else
                       {
                           for ( int i =0; auto_ii3 !=auto_ii2->ans.end(); auto_ii3++, ii++, i++ )
                           {
                                 if ( ii->vtype != VTYPE_MAIN )
                                      continue;

                                 int
                                 cmp =0;

                                 if ( i >=TotalWord )
                                 {
                                      cmp =auto_ii3->compare(L"[null]");
                                 }
                                 else
                                 {
                                      cmp =auto_ii3->compare(gCandVoca.record(i));
                                 }

                                 if ( cmp !=0 )
                                 {
                                      gAutoTest_succ =false;

                                      #if !defined(__OUTPUT_1_ERR)
                                      //error
                                      swprintf(ss, 1024, L"[%d, %s], key =%ls\r\n", auto_ii->id, auto_ii->name.c_str(), wsKeyin);
                                      gAutoTest_Result.append(ss);

                                      swprintf(ss, 1024, L"  第 %#2d 候選字, 預期 =%ls, 實際 =%ls\r\n",
                                                   i +1, auto_ii3->c_str(), gCandVoca.record(i));

                                      gAutoTest_Result.append(ss);
                                      break;
                                      #endif
                                 }
                           }

                           #if defined(__OUTPUT_ALL_ERR)
                           if ( gAutoTest_succ ==false )
                           {
                                swprintf(ss, 1024, L"[%d, %s], key =%ls\r\n", auto_ii->id, auto_ii->name.c_str(), wsKeyin);
                                gAutoTest_Result.append(ss);

                                //---------------------------------------------
                                gAutoTest_Result.append(L" 預期\r\n");
                                auto_ii3 =auto_ii2->ans.begin();

                                int i1 =0;
                                for ( ; i1 <TotalWord && auto_ii3 !=auto_ii2->ans.end(); i1++, auto_ii3++ )
                                {
                                      if ( auto_ii3->compare(gCandVoca.record(i1)) !=0 )
                                           swprintf(ss, 1024, L"   <<%ls>>\t", gCandVoca.record(i1));
                                      else
                                           swprintf(ss, 1024, L"   %ls\t", gCandVoca.record(i1));
                                      gAutoTest_Result.append(ss);
                                }
                                for ( ; i1 <GetItem; i1++ )
                                {
                                      swprintf(ss, 1024, L"[null]\t");
                                      gAutoTest_Result.append(ss);
                                }
                                gAutoTest_Result.append(L"\r\n");

                                //---------------------------------------------
                                gAutoTest_Result.append(L" 實際\n");
                                auto_ii3 =auto_ii2->ans.begin();
                                for ( ; auto_ii3 !=auto_ii2->ans.end(); auto_ii3++ )
                                {
                                      swprintf(ss, 1024, L"   %ls\t", auto_ii3->c_str());
                                      gAutoTest_Result.append(ss);
                                }
                                gAutoTest_Result.append(L"\r\n");
                           }
                           #endif
                     }
              }

              if ( TotalWord >0 && gOutVer ==true )
              {
                   wstring
                   wsWord =(wchar_t *)gCandVoca.record(0);

                   int
                   Total_NextWord = IQQI_GetNextWordCandidates(imeID, (wchar_t *)wsWord.c_str(), 0, GetItem, gCandVoca.pointer());

                   ss_printf(ss);
                   ss_printf(L"-------------------------------------------------------------------------\r\n");
                   swprintf(ss, 1024, L"  pWord = %ls, Total NextWord =%2d\r\n", wsWord.c_str(), Total_NextWord);
                   ss_printf(ss);
                   ss_printf(L"-------------------------------------------------------------------------\r\n");

                   for ( int i =0; i <Total_NextWord; i ++ )
                   {
                         printf("    pvoca(%2d) = %ls\r\n", i +1, gCandVoca.record(i));
                   }

                   printf("\r\n");

              }

              return TotalWord;
         }
         else
         {
              wstring
              wsWord =wsKeyin;

              int
              Total_NextWord = IQQI_GetNextWordCandidates(imeID, (wchar_t *)wsWord.c_str(), 0, GetItem, gCandVoca.pointer());

              //if ( Total_NextWord ==0 )
              //     Total_NextWord = IQQI_GetNextWordCandidates(imeID, (wchar_t *)L"", 0, GetItem, gCandVoca.pointer());

              if ( gDiagnostic ==false )
              {
                   ss_printf(ss);
                   ss_printf(L"-------------------------------------------------------------------------\r\n");
                   swprintf(ss, 1024, L"  pWord =%ls, Total NextWord =%2d\r\n", wsWord.c_str(), Total_NextWord);
                   ss_printf(ss);
                   ss_printf(L"-------------------------------------------------------------------------\r\n");

                   for ( int i =0; i <Total_NextWord; i ++ )
                   {
                         printf("    pvoca(%2d) =%ls\r\n", i +1, gCandVoca.record(i));
                   }
                   printf("\r\n");
              }
              else
              {
                   for ( int i =0; i <Total_NextWord; i ++ )
                   {
                         if ( gDiagnosticDebug.size() )
                              gDiagnosticDebug +=L" ";
                         gDiagnosticDebug +=gCandVoca.record(i);
                   }
              }
              return Total_NextWord;
         }
    }
    else
    {
         if ( wsKeyin[0] ==KEY_STAR_1 || wsKeyin[0] ==KEY_STAR_2 ) // @
         {
              switch ( imeID )
              {
                       case IQQI_IME_Thai:
                            break;
                       default:
                            GetItem =MAX_WORD_SYMB_LENGTH;
                            break;
              }
         }

         if ( gOutVer ==false && gMultiTap_Looping ==false )
         {
              if ( newID ==true )
              {
                   swprintf(ss, 1024, L"[ %2d, %s ]- [ %s ]\r\n", imeID, id_map[imeID -1].name, dictname);
                   ss_printf(ss);

                   printf("Length\t");
                   for ( int i =0; i <GetItem; i++ )
                   {
                         printf("#%d\t", i +1);
                   }
                   printf("\r\n");

                   newID =false;
              }
         }
         else
         {
              ss_printf(L"-------------------------------------------------------------------------\r\n");
              swprintf(ss, 1024, L"[ %2d, %s ]- [ %s ]\r\n", imeID, id_map[imeID -1].name, dictname);
              ss_printf(ss);
              ss_printf(L"-------------------------------------------------------------------------\r\n");
         }

         if ( gForFile ==false )
         {
              wchar_t *
              pMul =0;

              int
              mKCode =wsKeyin[0];

              if ( gLastSelectedBuf.size() >0 )
              {
                   const wchar_t *
                   pSz =gLastSelectedBuf.c_str();

                   #if defined(__0208)
                   pMul =IQQI_MultiTap_Input(imeID, mKCode, pSz[wcslen(pSz) -1], tabCount);
                   #else
                   pMul =IQQI_MultiTap_Input(imeID, mKCode, tabCount);
                   #endif
              }
              else
              {
                   #if defined(__0208)
                   pMul =IQQI_MultiTap_Input(imeID, mKCode, 0, tabCount);
                   #else
                   pMul =IQQI_MultiTap_Input(imeID, mKCode, tabCount);
                   #endif
              }

              if ( gOutVer ==true )
              {
                   swprintf(ss, 1024, L"MultiTap input %ls, count =%d, out=%ls\r\n\r\n", wsKeyin, tabCount, pMul);
                   ss_printf(ss);
              }
              else
              {
                   wstring
                   wsMul =pMul;

                   replaceAll(wsMul, L"|", L"\t");
                   replaceAll(wsMul, L">", L"");
                   replaceAll(wsMul, L"\'", L"\'\'");
                   replaceAll(wsMul, L" ", L"[space]");

                   if ( gMultiTap_Looping ==true )
                   {
                        wchar_t
                        wMulEx[1024] ={0};
                        wcscpy(wMulEx, wsMul.c_str());

                        wstring
                        wsNew;

                        gTotalSelect =0;

                        for ( int tb =0, tx =0; wMulEx[tx] !=0; tx++ )
                        {
                              if ( wMulEx[tx] =='\t' )
                              {
                                   wMulEx[tx] =0;

                                   wchar_t
                                   wsId[10];
                                   swprintf(wsId, 1024, L"(%d) ", ++gTotalSelect);

                                   wsNew +=wsId;
                                   wsNew +=wMulEx +tb;
                                   wsNew +=L"\t";
                                   tb =tx +1;
                              }
                        }

                        swprintf(wsLine, 1024, L"key%c\t%ls", mKCode, wsNew.c_str());
                   }
                   else
                   {
                        swprintf(wsLine, 1024, L"key%c\t%ls", mKCode, wsMul.c_str());
                   }
                   ss_printf(wsLine);
              }
              return wcslen(pMul);
         }
         else
         {
              int
              maxTestCount =20;

              int
              mKCode =wsKeyin[0];

              for ( int i =1; i <=maxTestCount; i++ )
              {
                    wchar_t *
                    #if defined(__0208)
                    pMul =IQQI_MultiTap_Input(imeID, mKCode, 0, i);
                    #else
                    pMul =IQQI_MultiTap_Input(imeID, mKCode, i);
                    #endif

                    if ( gOutVer ==true )
                    {
                         swprintf(ss, 1024, L"MultiTap input %c, count =%2d, out=%ls\r\n", mKCode, i, pMul);
                         ss_printf(ss);
                    }
                    else
                    {
                         wstring
                         wsMul =pMul;

                         replaceAll(wsMul, L"|", L"\t");
                         replaceAll(wsMul, L">", L"");
                         replaceAll(wsMul, L"\'", L"\'\'");
                         replaceAll(wsMul, L" ", L"[space]");
                         swprintf(wsLine, 1024, L"key%c\t%ls", mKCode, wsMul.c_str());
                         ss_printf(wsLine);
                         break;
                    }
              }

              ss_printf(L"\r\n");
              return maxTestCount;
         }
    }

    #endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void runUTest_Group(int _IME, wstring wsIME, wstring wsKeyin, wstring wsAns)
{
     gImeID =_IME;
     gDiagnosticDebug =L"";

     CandidateCH
     CandClass;
     CandClass.alloc(MAX_GROUP_COUNT, MAX_GROUP_LENGTH);

     IQQI_GetGrouping(gImeID, (wchar_t *)wsKeyin.c_str(), 0, 20, CandClass.pointer());

     wchar_t
     wMsg[4096] ={0};

     swprintf(wMsg, 4096, L"%ls 輸入 :%ls\r\n\r\n  " GRN "預期 " RESET ":%ls\r\n  取得 :%ls\r\n",
              wsIME.c_str(),
              wsKeyin.c_str(),
              wsAns.c_str(),
              gDiagnosticDebug.c_str());

     printf("\r\nCheck Group of %ls", wMsg);

     if ( gDiagnosticDebug.compare(wsAns) !=0 )
     {
          printf("  %ls " RED "Group" RESET " fail !!!\r\n\r\n", wsIME.c_str());
          gDiagnosticError++;
     }
     else
     {
          printf(GRN "  %ls Group pass !!!\r\n\r\n" RESET, wsIME.c_str());
     }
     printf("------------------------------------------------------------------------\r\n");
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void runUTest(int _IME, wstring wsIME, wstring wsKeyin, wstring wsAns, int diagnosticTestN)
{
     int
     tabCount  =1;

     gImeID =_IME;
     gOutVer =false;

     gDiagnosticDebug =L"";
     gDiagnosticTestN =diagnosticTestN;

     bool
     newID =true;
     int
     TotalWord =testAction(gImeID, (wchar_t*)wsKeyin.c_str(), tabCount, newID);

     wchar_t
     wMsg[4096] ={0};

     if ( gDiagnostiPressureTest ==true )
     {
          swprintf(wMsg, 4096, L"%ls 輸入 :%ls\r\n\r\n  取得 :%ls\r\n",
                   wsIME.c_str(),
                   wsKeyin.c_str(),
                   gDiagnosticDebug.c_str());
     }
     else
     {
          swprintf(wMsg, 4096, L"%ls 輸入 :%ls\r\n\r\n  " GRN "預期 " RESET ":%ls\r\n\r\n  取得 :%ls\r\n",
                   wsIME.c_str(),
                   wsKeyin.c_str(),
                   wsAns.c_str(),
                   gDiagnosticDebug.c_str());
     }

     printf("\r\nCheck %ls", wMsg);

     if ( gDiagnostiPressureTest ==true )
     {
          printf(GRN "  %ls done !!!\r\n\r\n" RESET, wsIME.c_str());
     }
     else if ( gDiagnosticDebug.compare(wsAns) !=0 )
     {
          printf("  %ls " RED "fail " RESET "!!!\r\n\r\n", wsIME.c_str());
          gDiagnosticError++;
     }
     else
     {
          printf(GRN "  %ls pass !!!\r\n\r\n" RESET, wsIME.c_str());
     }
     printf("------------------------------------------------------------------------\r\n");
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void runDiagnostic_pressureTest()
{
     printf("\r\n");
     gDiagnosticError =0;
     gDiagnostic =true;
     gDiagnostiPressureTest =true;

     string
     keyMin, keyMax;

     long
     iMin =0,
     iMax =0;

     while ( 1 )
     {
             printf("\r\n>>請輸入最 " RED " 小 " RESET " key值 (0~9999999) [Ctrl-C結束]>>\r\n");
             std::getline(std::cin, keyMin);

             iMin = atol(keyMin.c_str());

             if ( iMin >=0 && iMin <9999999 )
                  break;
     }

     while ( 1 )
     {
             printf("\r\n>>請輸入最 " GRN " 大 " RESET " key值 (0~9999999) [Ctrl-C結束]>>\r\n");
             std::getline(std::cin, keyMax);

             iMax = atol(keyMax.c_str());

             if ( iMax >=0 && iMax <9999999 )
                  break;
     }

     wstring
     wsKeyin;

     wstring
     wsAns =L"";

     wsKeyin =L"";
     wsAns =L"";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, 8);

     //------------------------------------------------------------------------
     //
     //------------------------------------------------------------------------
     #if !defined(__TEST_PREESS_10)
     {
          int
          gError_old =gDiagnosticError;

          for ( int i =iMin; i <=iMax; i++ )
          {
                wchar_t
                wsKeyinEx[120] ={0};

                swprintf(wsKeyinEx, 120, L"%d", i);

                wsAns   =L"";
                wsKeyin =wsKeyinEx;

                runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);
          }
          gDiagnosticError =gError_old;
     }
     #endif

     #if defined(__TEST_PREESS1)
     {
          int
          gError_old =gDiagnosticError;

          for ( int i =0; i <9; i++ )
          {
                for ( int j =0; j <9; j++ )
                {
                      for ( int k =0; k <9; k++ )
                      {
                            for ( int l =0; l <9; l++ )
                            {
                                  for ( int m =0; m <9; m++ )
                                  {
                                        for ( int n =0; n <9; n++ )
                                        {
                                              for ( int o =0; o <9; o++ )
                                              {
                                                    for ( int p =0; p <9; p++ )
                                                    {
                                                          for ( int q =0; q <9; q++ )
                                                          {
                                                                wchar_t
                                                                wsKeyinEx[120] ={0};

                                                                swprintf(wsKeyinEx, 120, L"%d%d%d%d%d%d%d%d%d", i, j, k, l, m, n, o, p, q);

                                                                wsAns   =L"";
                                                                wsKeyin =wsKeyinEx;

                                                                runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);
                                                          }
                                                    }
                                               }
                                        }
                                  }
                            }
                      }
                }
          }
          gDiagnosticError =gError_old;
     }
     #endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void runDiagnostic_2()
{
     printf("\r\n");
     gDiagnosticError =0;
     gDiagnostic =true;
     gDiagnostiPressureTest =true;

     wstring
     wsKeyin;

     wstring
     wsAns =L"";

     wsKeyin =L"22";
     wsAns =L"";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, 8);

     //------------------------------------------------------------------------
     //
     //------------------------------------------------------------------------
     #if !defined(__TEST_PREESS2)
     {
          wchar_t
          wsMatch[256][2] ={0};

          wcscpy(wsMatch[0], L"ba");
          wcscpy(wsMatch[1], L"ca");

          int
          gError_old =gDiagnosticError;

          for ( int i =0; i <2; i++ )
          {
                wchar_t
                wsKeyinEx[120] ={0};

                wcscpy(wsKeyinEx, L"22:");
                wcscat(wsKeyinEx, wsMatch[i]);

                wsAns   =L"";
                wsKeyin =wsKeyinEx;

                runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);
          }
          gDiagnosticError =gError_old;
     }
     #endif

     //------------------------------------------------------------------------
     //
     //------------------------------------------------------------------------
     #if !defined(__TEST_PREESS3)
     {
          wchar_t
          wsMatch[256][2] ={0};

          wcscpy(wsMatch[0], L"tan");
          wcscpy(wsMatch[1], L"tao");

          int
          gError_old =gDiagnosticError;

          for ( int i =0; i <2; i++ )
          {
                wchar_t
                wsKeyinEx[120] ={0};

                wcscpy(wsKeyinEx, L"826:");
                wcscat(wsKeyinEx, wsMatch[i]);

                wsAns   =L"";
                wsKeyin =wsKeyinEx;

                runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);
          }
          gDiagnosticError =gError_old;
     }
     #endif

     //------------------------------------------------------------------------
     //
     //------------------------------------------------------------------------
     #if !defined(__TEST_PREESS4)
     {
          wchar_t
          wsMatch[256][4] ={0};

          wcscpy(wsMatch[0], L"gun");
          wcscpy(wsMatch[1], L"guo");
          wcscpy(wsMatch[2], L"hun");
          wcscpy(wsMatch[3], L"huo");

          int
          gError_old =gDiagnosticError;

          for ( int i =0; i <4; i++ )
          {
                wchar_t
                wsKeyinEx[120] ={0};

                //swprintf(wsKeyinEx, 120, L"486:%s", wsMatch[i]);
                wcscpy(wsKeyinEx, L"486:");
                wcscat(wsKeyinEx, wsMatch[i]);

                wsAns   =L"";
                wsKeyin =wsKeyinEx;

                runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);
          }
          gDiagnosticError =gError_old;
     }
     #endif

     //------------------------------------------------------------------------
     //
     //------------------------------------------------------------------------
     #if !defined(__TEST_PREESS5)
     {
          wchar_t
          wsMatch[256][4] ={0};

          wcscpy(wsMatch[0], L"man");
          wcscpy(wsMatch[1], L"mao");
          wcscpy(wsMatch[2], L"nan");
          wcscpy(wsMatch[3], L"nao");

          int
          gError_old =gDiagnosticError;

          for ( int i =0; i <4; i++ )
          {
                wchar_t
                wsKeyinEx[120] ={0};

                //swprintf(wsKeyinEx, 120, L"486:%s", wsMatch[i]);
                wcscpy(wsKeyinEx, L"626:");
                wcscat(wsKeyinEx, wsMatch[i]);

                wsAns   =L"";
                wsKeyin =wsKeyinEx;

                runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);
          }
          gDiagnosticError =gError_old;
     }
     #endif


     //------------------------------------------------------------------------
     //
     //------------------------------------------------------------------------
     #if !defined(__TEST_PREESS6)
     {
          wchar_t
          wsMatch[256][5] ={0};

          wcscpy(wsMatch[0], L"wan");
          wcscpy(wsMatch[1], L"yan");
          wcscpy(wsMatch[2], L"yao");
          wcscpy(wsMatch[3], L"zan");
          wcscpy(wsMatch[4], L"zao");

          int
          gError_old =gDiagnosticError;

          for ( int i =0; i <5; i++ )
          {
                wchar_t
                wsKeyinEx[120] ={0};

                //swprintf(wsKeyinEx, 120, L"486:%s", wsMatch[i]);
                wcscpy(wsKeyinEx, L"926:");
                wcscat(wsKeyinEx, wsMatch[i]);

                wsAns   =L"";
                wsKeyin =wsKeyinEx;

                runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);
          }
          gDiagnosticError =gError_old;
     }
     #endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void runDiagnostic_1()
{

     printf("\r\n");
     gDiagnosticError =0;
     gDiagnostic =true;
     gDiagnostiPressureTest =false;

     wstring
     wsKeyin;

     wstring
     wsAns =L"";

     wsKeyin =L"22:ㄅㄢ";
     wsAns =L"辦:ㄅㄢ 半:ㄅㄢ 班:ㄅㄢ 板:ㄅㄢ 版:ㄅㄢ 般:ㄅㄢ 伴:ㄅㄢ 搬:ㄅㄢ";
     runUTest(IQQI_IME_Chinese_TW, L"Chinese_TW", wsKeyin, wsAns, 8);

     wsKeyin =L"46:ㄎㄨ";
     wsAns =L"苦:ㄎㄨ 庫:ㄎㄨ 枯:ㄎㄨ 哭:ㄎㄨ 酷:ㄎㄨ 褲:ㄎㄨ 窟:ㄎㄨ 骷:ㄎㄨ 挎:ㄎㄨ 矻:ㄎㄨ 嚳:ㄎㄨ 顝:ㄎㄨ 跍:ㄎㄨ 刳:ㄎㄨ 胐:ㄎㄨ 桍:ㄎㄨ 瘔:ㄎㄨ 趶:ㄎㄨ 楛:ㄎㄨ 喾:ㄎㄨ 秙:ㄎㄨ 库:ㄎㄨ 郀:ㄎㄨ 裤:ㄎㄨ 袴:ㄎㄨ 俈:ㄎㄨ 絝:ㄎㄨ 绔:ㄎㄨ 圐:ㄎㄨ 堀:ㄎㄨ 圣:ㄎㄨ 橭:ㄎㄨ 軲:ㄎㄨ 鮬:ㄎㄨ 扝:ㄎㄨ 崫:ㄎㄨ 捁:ㄎㄨ 焅:ㄎㄨ 空:ㄎㄨㄥ 款:ㄎㄨㄢ 況:ㄎㄨㄤ 快:ㄎㄨㄞ 控:ㄎㄨㄥ 恐:ㄎㄨㄥ 困:ㄎㄨㄣ 擴:ㄎㄨㄛ 跨:ㄎㄨㄚ 塊:ㄎㄨㄞ 寬:ㄎㄨㄢ 狂:ㄎㄨㄤ 孔:ㄎㄨㄥ 礦:ㄎㄨㄤ 虧:ㄎㄨㄟ 誇:ㄎㄨㄚ 曠:ㄎㄨㄤ 昆:ㄎㄨㄣ 闊:ㄎㄨㄛ 綑:ㄎㄨㄣ 窺:ㄎㄨㄟ 捆:ㄎㄨㄣ 潰:ㄎㄨㄟ 垮:ㄎㄨㄚ 愧:ㄎㄨㄟ 崑:ㄎㄨㄣ 匡:ㄎㄨㄤ 會:ㄎㄨㄞ 魁:ㄎㄨㄟ 睽:ㄎㄨㄟ 夸:ㄎㄨㄚ 誑:ㄎㄨㄤ 坤:ㄎㄨㄣ 膾:ㄎㄨㄞ 廓:ㄎㄨㄛ 胯:ㄎㄨㄚ 框:ㄎㄨㄤ 葵:ㄎㄨㄟ 筐:ㄎㄨㄤ 夔:ㄎㄨㄟ 匱:ㄎㄨㄟ 饋:ㄎㄨㄟ";

     runUTest(IQQI_IME_Chinese_TW, L"Chinese_TW", wsKeyin, wsAns, 80);

     wsAns =L"網:ㄨㄤ 望:ㄨㄤ 王:ㄨㄤ 往:ㄨㄤ 亡:ㄨㄤ 旺:ㄨㄤ 忘:ㄨㄤ 妄:ㄨㄤ 枉:ㄨㄤ 汪:ㄨㄤ 罔:ㄨㄤ 魍:ㄨㄤ 惘:ㄨㄤ 尪:ㄨㄤ 暀:ㄨㄤ 网:ㄨㄤ 輞:ㄨㄤ 瀇:ㄨㄤ 朢:ㄨㄤ 菵:ㄨㄤ 尢:ㄨㄤ 莣:ㄨㄤ 迋:ㄨㄤ 臦:ㄨㄤ 尣:ㄨㄤ 尩:ㄨㄤ 棢:ㄨㄤ 徃:ㄨㄤ 彺:ㄨㄤ 辋:ㄨㄤ 蛧:ㄨㄤ 蝄:ㄨㄤ 蚟:ㄨㄤ 尫:ㄨㄤ 罒:ㄨㄤ 誷:ㄨㄤ 迬:ㄨㄤ 亾:ㄨㄤ 兦:ㄨㄤ 俇:ㄨㄤ 仼:ㄨㄤ 朚:ㄨㄤ 冈:ㄨㄤ";

     wsKeyin =L"68:ㄨㄤ";
     runUTest(IQQI_IME_Chinese_TW, L"Chinese_TW", wsKeyin, wsAns, 44);

     //------------------------------------------------------------------------
     Tw1_DATA_UI
     TwD1;

     wsAns =L"";
     for ( int tIdx =0; tIdx <MAX_Tw1_DATA_UI; tIdx++ )
     {
           if ( tIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=TwD1.GetWDataSz(tIdx);
     }

     wsKeyin =L"9:ㄩ";
     runUTest(IQQI_IME_Chinese_TW, L"Chinese_TW", wsKeyin, wsAns, MAX_Tw1_DATA_UI);

     //------------------------------------------------------------------------
     Tw2_DATA_UI
     TwD2;

     wsAns =L"";
     for ( int tIdx =0; tIdx <MAX_Tw2_DATA_UI; tIdx++ )
     {
           if ( tIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=TwD2.GetWDataSz(tIdx);
     }

     wsKeyin =L"3:ㄧ";
     runUTest(IQQI_IME_Chinese_TW, L"Chinese_TW", wsKeyin, wsAns, MAX_Tw2_DATA_UI);

     //------------------------------------------------------------------------
     // Test Chinese_CN (Group)
     //------------------------------------------------------------------------
     wsAns =L"ㄅㄢ ㄉㄢ ㄓㄢ";
     wsKeyin =L"22";
     runUTest_Group(IQQI_IME_Chinese_TW, L"Chinese_TW", wsKeyin, wsAns);

     //------------------------------------------------------------------------
     // Test Chinese_TW (nextWord)
     //------------------------------------------------------------------------
     printf("Check Next Word of Chinese_TW\r\n\r\n");

     gNextword =true;
     wsAns =L"內 際 家 外 民 產 慶 立 美 小 防 會 籍 道 中 人 王 企 足 旗 度 貿 情 債 營 力 米 腳 稅";
     wsKeyin =L"國";
     runUTest(IQQI_IME_Chinese_TW, L"Chinese_TW", wsKeyin, wsAns, 10);

     gNextword =false;

     //------------------------------------------------------------------------
     //
     // Test Chinese CN
     //
     //------------------------------------------------------------------------
     Py1_DATA_UI
     pyD1;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py1_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD1.GetWDataSz(pIdx);
     }

     wsKeyin =L"22:ba";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py1_DATA_UI);

     //------------------------------------------------------------------------
     Py2_DATA_UI
     pyD2;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py2_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD2.GetWDataSz(pIdx);
     }

     wsKeyin =L"926:yan";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py2_DATA_UI);

     //------------------------------------------------------------------------
     Py3_DATA_UI
     pyD3;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py3_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD3.GetWDataSz(pIdx);
     }

     wsKeyin =L"986:zuo";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py3_DATA_UI);

     //------------------------------------------------------------------------
     Py4_DATA_UI
     pyD4;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py4_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD4.GetWDataSz(pIdx);
     }

     wsKeyin =L"926:zan";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py4_DATA_UI);

     //------------------------------------------------------------------------
     //測試補連續Key(數字按鍵)
     //------------------------------------------------------------------------
     Py5_DATA_UI
     pyD5;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py5_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD5.GetWDataSz(pIdx);
     }

     wsKeyin =L"86";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py5_DATA_UI);

     //------------------------------------------------------------------------
     Py6_DATA_UI
     pyD6;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py6_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD6.GetWDataSz(pIdx);
     }

     wsKeyin =L"866:tong";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py6_DATA_UI);

     //------------------------------------------------------------------------
     Py7_DATA_UI
     pyD7;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py7_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD7.GetWDataSz(pIdx);
     }

     wsKeyin =L"9466";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py7_DATA_UI);

     //------------------------------------------------------------------------
     Py8_DATA_UI
     pyD8;

     wsAns =L"";
     for ( int pIdx =0; pIdx <MAX_Py8_DATA_UI; pIdx++ )
     {
           if ( pIdx )
           {
                wsAns +=L" ";
           }
           wsAns +=pyD8.GetWDataSz(pIdx);
     }

     wsKeyin =L"9466:zhon";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_Py8_DATA_UI);

     //------------------------------------------------------------------------
     // Test Chinese_CN (Group)
     //------------------------------------------------------------------------
     wsAns =L"ba ca";
     wsKeyin =L"22";
     runUTest_Group(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns);

     //------------------------------------------------------------------------
     // Test Chinese_CN (nextWord)
     //------------------------------------------------------------------------
     printf("Check Next Word of Chinese_CN\r\n\r\n");

     gNextword =true;

     PyNxt1_DATA_UI
     PyNxt1_D1;

     wsAns =L"";
     for ( int nxIdx2 =0; nxIdx2 <MAX_PyNxt1_DATA_UI; nxIdx2++ )
     {
           if ( nxIdx2 )
           {
                wsAns +=L" ";
           }
           wsAns +=PyNxt1_D1.GetWDataSz(nxIdx2);
     }

     wsKeyin =L"假";
     runUTest(IQQI_IME_Chinese_CN, L"Chinese_CN", wsKeyin, wsAns, MAX_PyNxt1_DATA_UI);

     gNextword =false;

     //------------------------------------------------------------------------
     //
     // Test Chinese HK
     //
     //------------------------------------------------------------------------
     Hk1_DATA_UI
     hkD1;

     wsAns =L"";
     for ( int hIdx1 =0; hIdx1 <MAX_Hk1_DATA_UI; hIdx1++ )
     {
           if ( hIdx1 )
           {
                wsAns +=L" ";
           }
           wsAns +=hkD1.GetWDataSz(hIdx1);
     }

     wsKeyin =L"11";
     runUTest(IQQI_IME_Chinese_HK, L"Chinese_HK", wsKeyin, wsAns, MAX_Hk1_DATA_UI);

     Hk2_DATA_UI
     hkD2;

     wsAns =L"";
     for ( int hIdx2 =0; hIdx2 <MAX_Hk2_DATA_UI; hIdx2++ )
     {
           if ( hIdx2 )
           {
                wsAns +=L" ";
           }
           wsAns +=hkD2.GetWDataSz(hIdx2);
     }

     wsKeyin =L"23";
     runUTest(IQQI_IME_Chinese_HK, L"Chinese_HK", wsKeyin, wsAns, MAX_Hk2_DATA_UI);

     //------------------------------------------------------------------------
     Hk3_DATA_UI
     hkD3;

     wsAns =L"";
     for ( int hIdx3 =0; hIdx3 <MAX_Hk3_DATA_UI; hIdx3++ )
     {
           if ( hIdx3 )
           {
                wsAns +=L" ";
           }
           wsAns +=hkD3.GetWDataSz(hIdx3);
     }

     wsKeyin =L"666";
     runUTest(IQQI_IME_Chinese_HK, L"Chinese_HK", wsKeyin, wsAns, MAX_Hk3_DATA_UI);

     //------------------------------------------------------------------------
     Hk4_DATA_UI
     hkD4;

     wsAns =L"";
     for ( int hIdx4 =0; hIdx4 <MAX_Hk4_DATA_UI; hIdx4++ )
     {
           if ( hIdx4 )
           {
                wsAns +=L" ";
           }
           wsAns +=hkD4.GetWDataSz(hIdx4);
     }

     wsKeyin =L"16";
     runUTest(IQQI_IME_Chinese_HK, L"Chinese_HK", wsKeyin, wsAns, MAX_Hk4_DATA_UI);

     //------------------------------------------------------------------------
     Hk5_DATA_UI
     hkD5;

     wsAns =L"";
     for ( int hIdx5 =0; hIdx5 <MAX_Hk5_DATA_UI; hIdx5++ )
     {
           if ( hIdx5 )
           {
                wsAns +=L" ";
           }
           wsAns +=hkD5.GetWDataSz(hIdx5);
     }

     wsKeyin =L"611";
     runUTest(IQQI_IME_Chinese_HK, L"Chinese_HK", wsKeyin, wsAns, MAX_Hk5_DATA_UI);

     //------------------------------------------------------------------------
     Hk6_DATA_UI
     hkD6;

     wsAns =L"";
     for ( int hIdx6 =0; hIdx6 <MAX_Hk6_DATA_UI; hIdx6++ )
     {
           if ( hIdx6 )
           {
                wsAns +=L" ";
           }
           wsAns +=hkD6.GetWDataSz(hIdx6);
     }

     wsKeyin =L"623";
     runUTest(IQQI_IME_Chinese_HK, L"Chinese_HK", wsKeyin, wsAns, MAX_Hk6_DATA_UI);

     //------------------------------------------------------------------------
     //
     // Test Chinese Thai
     //
     //------------------------------------------------------------------------
     Thai1_DATA_UI
     thD1;

     wsAns =L"";
     for ( int tIdx =0; tIdx <MAX_Thai1_DATA_UI; tIdx++ )
     {
           if ( tIdx )
           {
                wsAns +=L" ";
           }

           wsAns +=thD1.GetWDataSz(tIdx);
     }

     gImeID  =IQQI_IME_Thai;
     wsKeyin =L"7@69@";
     runUTest(IQQI_IME_Thai, L"Thai", wsKeyin, wsAns, MAX_Thai1_DATA_UI);

     //------------------------------------------------------------------------
     Thai2_DATA_UI
     thD2;

     wsAns =L"";
     for ( int tIdx =0; tIdx <MAX_Thai2_DATA_UI; tIdx++ )
     {
           if ( tIdx )
           {
                wsAns +=L" ";
           }

           wsAns +=thD2.GetWDataSz(tIdx);
     }

     wsKeyin =L"5@@";
     runUTest(IQQI_IME_Thai, L"Thai", wsKeyin, wsAns, 20);

     //------------------------------------------------------------------------
     if ( gDiagnosticError >0 )
     {
          printf(RED "diagnostic fail !!! 自我診斷測試 錯誤 !!!\r\n\r\n" RESET);
     }

     //------------------------------------------------------------------------
     Thai3_DATA_UI
     thD3;

     wsAns =L"";
     for ( int tIdx =0; tIdx <MAX_Thai3_DATA_UI; tIdx++ )
     {
           if ( tIdx )
           {
                wsAns +=L" ";
           }

           wsAns +=thD3.GetWDataSz(tIdx);
     }

     wsKeyin =L"18@@6";
     runUTest(IQQI_IME_Thai, L"Thai", wsKeyin, wsAns, MAX_Thai3_DATA_UI);

     //------------------------------------------------------------------------
     //
     // Test Vietnamese
     //
     //------------------------------------------------------------------------
     Vi1_DATA_UI
     viD1;

     wsAns =L"";
     for ( int vIdx1 =0; vIdx1 <MAX_Vi1_DATA_UI; vIdx1++ )
     {
           if ( vIdx1 )
           {
                wsAns +=L" ";
           }
           wsAns +=viD1.GetWDataSz(vIdx1);
     }

     wsKeyin =L"6426";
     runUTest(IQQI_IME_Vietnamese, L"Vietnamese", wsKeyin, wsAns, MAX_Vi1_DATA_UI);

     //------------------------------------------------------------------------
     Vi2_DATA_UI
     viD2;

     wsAns =L"";
     for ( int vIdx2 =0; vIdx2 <MAX_Vi2_DATA_UI; vIdx2++ )
     {
           if ( vIdx2 )
           {
                wsAns +=L" ";
           }
           wsAns +=viD2.GetWDataSz(vIdx2);
     }

     //------------------------------------------------------------------------
     wsKeyin =L"7826";
     runUTest(IQQI_IME_Vietnamese, L"Vietnamese", wsKeyin, wsAns, MAX_Vi2_DATA_UI);
     Vi3_DATA_UI
     viD3;

     wsAns =L"";
     for ( int vIdx3 =0; vIdx3 <MAX_Vi3_DATA_UI; vIdx3++ )
     {
           if ( vIdx3 )
           {
                wsAns +=L" ";
           }
           wsAns +=viD3.GetWDataSz(vIdx3);
     }

     wsKeyin =L"728";
     runUTest(IQQI_IME_Vietnamese, L"Vietnamese", wsKeyin, wsAns, MAX_Vi3_DATA_UI);

     //------------------------------------------------------------------------
     if ( gDiagnosticError >0 )
     {
          printf(RED "diagnostic fail !!! 自我診斷測試 錯誤 !!!\r\n\r\n" RESET);
     }

     //------------------------------------------------------------------------
     #if defined(__TEST_ADDB)
     string
     sTestLrn  ="oops@hahaha@oxoxx@";

     gForFile =true;

     printf("add import vocabulary =%s\r\n", sTestLrn.c_str());

     gwsVoca_Add_Import =sTestLrn;

     bool
     newID =true;

     int
     tabCount =1,
     retVal =testAction(gImeID, (wchar_t*)L"", tabCount, newID);

     if ( retVal !=0 )
     {
          printf("add import vocabulary " RED "Fail !" RESET "\r\n");
     }
     #endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void showHelp()
{
     #if defined(__HINDI_BASE)
     #define MAX_UI_IME_ID   IQQI_IME_Hindi_01
     #else
     #define MAX_UI_IME_ID   IQQI_IME_Tibetan
     #endif

     string ss;
     for ( int id =0; id <MAX_UI_IME_ID; )
     {
           for ( int c =0;  id <MAX_UI_IME_ID && c <4; c++, id++ )
           {
                 char item[120] ={0};
                 sprintf(item, "     %2d (0x%2x)    %s", id +1, id +1, id_map[id].name);

                 string sItem =item;
                 for ( int l =strlen(item); l <34; l++ )
                 {
                       sItem +=" ";
                 }
                 ss +=sItem;
           }
           ss +="\r\n";
     }

     printf("\r\nlang ID :\r\n");
     printf("%s", ss.c_str());
     printf("\r\n");

     printf("usage : ./demo [langID] [(t/tv)|(q/qv)|(m/mv)|(n)] [input text] [option]\r\n\r\n");
     printf("    ex: ./demo 1 t 22                      (EN-US, T9 mode, input 22, test count =1,   getItem 預設 20) 預設 平行輸出\r\n");
     printf("    ex: ./demo 1 tv 22                     (EN-US, T9 mode, input 22, test count =1,   getItem 預設 20) v=垂直輸出\r\n");
     printf("    ex: ./demo 1 tv 22 1 20                (EN-US, T9 mode, input 22, test count =1,   getItem 預設 20) v=垂直輸出\r\n");
     printf("    ex: ./demo 1 tv 22 100 20              (EN-US, T9 mode, input 22, test count =100, getItem 預設 20) v=垂直輸出\r\n");
     printf("    ex: ./demo 1 tv /in testcase.txt       (EN-US, T9 mode, for testcase file, getItem 預設 20) v=垂直輸出\r\n");
     printf("    ex: ./demo 1 tv /in testcase.txt 10    (EN-US, T9 mode, for testcase file, getItem 10)      v=垂直輸出\r\n\r\n");

     printf("    ex: ./demo 28/70 tk                    (拼音/注音專用, T9 (KaiOS) mode, input 22, test count =1,   getItem 預設 20) 預設 平行輸出\r\n\r\n");

     printf("    ex: ./demo 1 t /autotest autotest.txt   (EN-US, T9 mode, 自動測試, 測試檔案 =autotest.txt)\r\n");
     printf("    ex: ./demo 999 t /autotest autotest.txt (測檔案中的 Lang ID, T9 mode, 自動測試, 測試檔案 =autotest.txt)\r\n\r\n");

     printf("    ex: ./demo 1 t aa@bb /addb             (EN-US, T9 mode, add custom voca aa & bb) 新增自訂詞 ByToken\r\n");
     printf("    ex: ./demo 1 t testfile /addbf         (EN-US, T9 mode, add custom voca from testfile) 新增自訂詞 ByToken\r\n");
     printf("    ex: ./demo 1 t testfile /addbf_imp     (EN-US, T9 mode, add custom voca from testfile) 新增自訂詞 ByToken, Just Import\r\n");
     printf("    ex: ./demo 1 t testfile /addbf_update  (EN-US, T9 mode, add custom voca from testfile) 新增自訂詞 ByToken, Just Import\r\n");
     printf("    ex: ./demo 1 t aa@bb /delb             (EN-US, T9 mode, del custom voca aa & bb) 刪除自訂詞 ByToken\r\n");
     printf("    ex: ./demo 1 t aa /add                 (EN-US, T9 mode, add custom voca aa) 新增自訂詞\r\n");
     printf("    ex: ./demo 1 t aa /del                 (EN-US, T9 mode, del custom voca aa) 刪除自訂詞\r\n");
     printf("    ex: ./demo 1 t aa /reset               (EN-US, T9 mode, aa 不重要, 隨便輸入任意字) Reset MultiTap 的所有學習到的詞\r\n");
     printf("    ex: ./demo 1 t aa /chk                 (EN-US, T9 mode, check Vocabulary aa exist or not) 檢查詞彙[aa]是否已經存在 \r\n");
     printf("    ex: ./demo 1 t 100 /chklimit           (EN-US, T9 mode, check Vocabulary over 100 or not ) 自學詞庫是否超過指定(100)範圍\r\n");
     printf("    ex: ./demo 1 t \"syl\" \"see you later\" /add       (EN-US, T9 mode, add phrase syl='see you later') 新增略語\r\n");
     printf("    ex: ./demo 1 t \"syl\" \"see you later\" /del       (EN-US, T9 mode, del phrase syl='see you later') 刪除略語\r\n");
     printf("    ex: ./demo 1 n how                     (EN-US, Check NextWord, input [how]） 聯想詞\r\n");
     printf("    ex: ./demo 1 n \"how are you\"         (EN-US, Check NextWord, input [how are you]） 聯想詞\r\n\r\n");
     printf("    ex: ./demo 1 qv abc                    (EN-US, Qwerty mode, input abc） v=垂直輸出\r\n");
     printf("    ex: ./demo 1 mv 2 3                    (EN-US, gMultiTap, input 2, count =3) v=垂直輸出\r\n");
     printf("    ex: ./demo 10 mk                       (HINDI, gMultiTap) 使用人機介面測試\r\n");

     printf("    ex: ./demo 1 t aa /loop                (EN-US) 使用人機介面測試自學詞功能\r\n");
     #if !defined(__NO_KOREAN)
     printf("    ex: ./demo 86 kr                       (KOREAN) 使用人機介面測試組字規則\r\n");
     printf("    ex: ./demo 86 krfile testcast.txt      (KOREAN) 使用自動測試組字規則\r\n");
     #endif
     printf("    ex: ./demo /diagAll                    =/diag + /diag2 + diagP\r\n");
     printf("    ex: ./demo /diagP                      diagnostic (Pressure Test) 自我診斷(壓力)測試\r\n");
     printf("    ex: ./demo /diag2                      diagnostic 自我診斷測試 2\r\n");
     printf("    ex: ./demo /diag or /diag1             diagnostic 自我診斷測試 1\r\n");
     //printf("    ex: ./demo 1 o dict.txt              (EN-US, Output dict to text)\r\n");
     printf("    ex: ./demo /ver                        (show kernel version) 版號\r\n\r\n");
     printf(" ps: @ =輸入*\r\n\r\n");
     exit(1);
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class autotest_kr_ex :public autotest_kr
{
public:
      long                   max;

      autotest_kr_ex()
      {
      }

      void evt_onPause()
      {
      }

      void evt_progress(long line)
      {
           printf("\rMax =%ld, ", max);
      }

      void evt_setRange(long min, long _max)
      {
           max =_max;
           printf("Total test =%ld\r\n", max);
      }

      void evt_errReport(long line, wstring wMsg)
      {
      }

      void evt_errReportEx(long line, wstring wKeyinNumber, wstring wKeyin, wstring wAns, wstring wGetAns, int cmp)
      {
          wchar_t
          aa[1024] ={0};

          #if defined(_WIN32)

          swprintf(aa, L"line =%d\tinput =%ls\tstroke =%ls\tExpect =%ls\tGet =%ls\r\n",
                   line, wKeyinNumber.c_str(), wKeyin.c_str(), wAns.c_str(), wGetAns.c_str());

          #else

          swprintf(aa, 1024, L"line =%d\tinput =%ls\tstroke =%ls\tExpect =%ls\tGet =%ls\r\n",
                   line, wKeyinNumber.c_str(), wKeyin.c_str(), wAns.c_str(), wGetAns.c_str());

          #endif

          printf("%ls", aa);
          if ( cmp !=0 )
               printf(RED "   error!\r\n" RESET);
      }
};

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
int main_ex(int argc, char *argv[])
{
    wchar_t wsKeyin[1024] ={0};
    int     imeID =IQQI_IME_English_US;

    int     tabCount  =1;
    int     maxTest   =1;
    int     getNumber =20;

    string  fOpt  ="./__opt.br";
    string  fDbg  ="./__opt.db";
    string  fKey  ="./__fkey.db";
    string  fLen4 ="./__fLen4.db";

    string  KrTestFile;
    bool    TestLearnWord_Looping =false;





    gInput.clear();

    //printf("argc =%d\r\n", argc);

    if ( argc ==1 )
    {
         showHelp();
         exit(1);
    }

    if ( argc ==2 && (strcmp(argv[1], "/ver") ==0 || strcmp(argv[1], "/v") ==0 || strcmp(argv[1], "--ver") ==0) )
    {
         printf("\r\n\r\nkernel version =%s\r\n\r\n", IQQI_Version());
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/diagAll") ==0 )
    {
         runDiagnostic_pressureTest();
         runDiagnostic_2();
         runDiagnostic_1();
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/diagP") ==0 )
    {
         runDiagnostic_pressureTest();
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/diag2") ==0 )
    {
         runDiagnostic_2();
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/diag1") ==0 )
    {
         runDiagnostic_1();
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/diag") ==0 )
    {
         runDiagnostic_1();
         exit(1);
    }
    //-------------------------------------------------------------------------
    else if ( argc ==2 && strcmp(argv[1], "/debug1") ==0 )
    {
         fstream
         fp;
         fp.open(fDbg, ios::out);

         if ( !fp )
         {
              cout<<"Fail to create file: " <<fOpt <<endl;
         }
         else
         {
              fp <<"dbon";
         }
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/debug2") ==0 )
    {
         std::remove(fDbg.c_str());
         exit(1);
    }
    //-------------------------------------------------------------------------
    else if ( argc ==2 && strcmp(argv[1], "/fullkey1") ==0 )
    {
         fstream
         fp;
         fp.open(fKey, ios::out);

         if ( !fp )
         {
              cout<<"Fail to create file: " <<fKey <<endl;
         }
         else
         {
              fp <<"dbon";
         }
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/fullkey2") ==0 )
    {
         std::remove(fKey.c_str());
         exit(1);
    }
    //-------------------------------------------------------------------------
    else if ( argc ==2 && strcmp(argv[1], "/len4on") ==0 )
    {
         fstream
         fp;
         fp.open(fLen4, ios::out);

         if ( !fp )
         {
              cout<<"Fail to create file: " <<fLen4 <<endl;
         }
         else
         {
              fp <<"dbon";
         }
         exit(1);
    }
    else if ( argc ==2 && strcmp(argv[1], "/len4off") ==0 )
    {
         std::remove(fLen4.c_str());
         exit(1);
    }
    //-------------------------------------------------------------------------
    else if ( argc ==2 && strcmp(argv[1], "/bon") ==0 )
    {
         fstream
         fp;
         fp.open(fOpt, ios::out);

         if ( !fp )
         {
              cout<<"Fail to create file: " <<fOpt <<endl;
         }
         else
         {
              fp <<"on";
         }
         exit(1);
    }

    else if ( argc ==2 && strcmp(argv[1], "/boff") ==0 )
    {
         std::remove(fOpt.c_str());
         exit(1);
    }
    else if ( argc <=3 && strcmp(argv[2], "mk") !=0 && strcmp(argv[2], "kr") )
    {
         showHelp();
    }

    else if ( argc >3 )
    {
         if ( strcmp(argv[2], "t") ==0 || strcmp(argv[2], "tv") ==0 || strcmp(argv[2], "tk") ==0 )
         {
              gOutVer    =strcmp(argv[2], "tv") ==0;
              gKaiOS_fmt =strcmp(argv[2], "tk") ==0;

              if ( gKaiOS_fmt ==true )
              {
                   if ( argc >4 )
                   {
                        gKaiOS_idx =atoi(argv[4]);

                        if ( gKaiOS_idx <1 )
                             gKaiOS_idx =1;
                        printf("KaiOS mode, cursor Index =%d\r\n", gKaiOS_idx);
                   }
                   else
                   {
                        printf("KaiOS Mode, def cursor Index =0\r\n");
                   }
              }
              IQQI_SetOption(opt_KeybdMode, for_T9);
              printf("for T9 Mode\r\n");
         }
         else if ( strcmp(argv[2], "q") ==0 || strcmp(argv[2], "qv") ==0 )
         {
              gOutVer =strcmp(argv[2], "qv") ==0;

              IQQI_SetOption(opt_KeybdMode, for_Qwerty);
              printf("for Qwerty Mode\r\n");
         }
         else if ( strcmp(argv[2], "m") ==0 || strcmp(argv[2], "mv") ==0 )
         {
              gOutVer =strcmp(argv[2], "mv") ==0;
         }
         else if ( strcmp(argv[2], "n") ==0 || strcmp(argv[2], "nv") ==0 )
         {
              gOutVer =strcmp(argv[2], "nv") ==0;

              gNextword =true;
              printf("for Nextword Mode\r\n");
         }
         #if !defined(__NO_KOREAN)
         else if ( strcmp(argv[2], "krfile") ==0 )
         {
              if ( strlen(argv[3]) ==0 )
              {
                   showHelp();
                   exit(1);
              }
              KrTestFile =argv[3];

              imeID =atoi(argv[1]);
              gImeID =imeID;

              gKorean_Testing =true;
              gOutVer =false;
         }
         #endif
         else if ( strcmp(argv[2], "o") ==0 )
         {
              if ( strlen(argv[3]) ==0 )
              {
                   showHelp();
                   exit(1);
              }
              gExportDict =argv[3];
         }
         else
         {
              showHelp();
         }

         //--------------------------------------------------------------------
         imeID =atoi(argv[1]);
         gImeID =imeID;

         if ( imeID !=999 && (imeID <1 || imeID >MAX_UI_IME_ID) )
         {
              printf("error Lang ID\r\n");
              printf("usage : ./demo help\r\n");
              exit(1);
         }
         //--------------------------------------------------------------------
         wcscpy(wsKeyin, utf8_to_utf16(argv[3]).c_str());

         //--------------------------------------------------------------------
         if ( argc >4 && strcmp(argv[3], "/autotest") ==0 )
         {
              gAutoTest =true;

              printf("autotest file =%s\r\n", argv[4]);

              std::ifstream infile(argv[4]);
              std::string   line;

              while ( std::getline(infile, line) )
              {
                     line.erase(remove(line.begin(), line.end(), '\r'), line.end());
                     line.erase(remove(line.begin(), line.end(), '\n'), line.end());

                     //cout <<".size() ="<<line.size()<<"=====>"<<line<<"\r\n";

                     if ( line.size() >0 )
                     {
                          autoX.action(line, "\t");
                     }
              }
              infile.close();

              autoX.actionDone();
         }
         else if ( argc >4 && strcmp(argv[3], "/in") ==0 )
         {
              gForFile =true;

              printf("testcase =%s\r\n", argv[4]);

              std::ifstream infile(argv[4]);
              std::string   line;

              while ( std::getline(infile, line) )
              {
                     line.erase(remove(line.begin(), line.end(), '\r'), line.end());
                     line.erase(remove(line.begin(), line.end(), '\n'), line.end());

                     //cout <<".size() "<<line.size()<<","<<line<<"\r\n";

                     if ( line.size() >0 )
                     {
                          gInput.push_back(utf8_to_utf16(line));
                     }
              }
              infile.close();
         }
         else if ( argc >5 && strcmp(argv[5], "/add") ==0 )
         {
              gForFile =true;

              printf("add phrase key=%s, content=%s\r\n", argv[3], argv[4]);

              gwsPhrs_Key =utf8_to_utf16(argv[3]);
              gwsPhrs_Add =utf8_to_utf16(argv[4]);
         }
         else if ( argc >4 && strcmp(argv[4], "/del") ==0 )
         {
              gForFile =true;

              printf("del phrase key=%s, content=%s\r\n", argv[3], argv[3]);

              gwsPhrs_Key =L"";//utf8_to_utf16(argv[3]);
              gwsPhrs_Del =utf8_to_utf16(argv[3]);
         }

         //--------------------------------------------------------------------
         else if ( argc >4 && strcmp(argv[4], "/chklimit") ==0 )
         {
              gChkLimit      =true;
              gChkLimitCount =atoi(argv[3]);

              printf("\r\nfor Check User DB Limit\r\n\r\n");
         }
         else if ( argc >4 && strcmp(argv[4], "/chk") ==0 )
         {
              gChkExist     =true;
              gChkExistVoca =utf8_to_utf16(argv[3]);

              printf("\r\nfor Check Exist Vocabulary Mode\r\n\r\n");
         }
         else if ( argc >4 && strcmp(argv[4], "/loop") ==0 )
         {
              TestLearnWord_Looping =true;
         }
         else if ( argc >4 && strcmp(argv[4], "/add") ==0 )
         {
              gForFile =true;

              printf("add vocabulary =%s\r\n", argv[3]);

              gwsVoca_Add =utf8_to_utf16(argv[3]);
         }
         else if ( argc >4 && strcmp(argv[4], "/del") ==0 )
         {
              gForFile =true;

              printf("del vocabulary =%s\r\n", argv[3]);

              gwsVoca_Del =utf8_to_utf16(argv[3]);
         }
         else if ( argc >4 && strcmp(argv[4], "/reset") ==0 )
         {
              gForFile =true;

              printf("reset vocabulary =%s\r\n", argv[3]);

              gwsVoca_Reset =utf8_to_utf16(argv[3]);
         }

         //--------------------------------------------------------------------
         else if ( argc >4 && strcmp(argv[4], "/addb") ==0 )
         {
              gForFile =true;

              printf("add import vocabulary =%s\r\n", argv[3]);

              gwsVoca_Add_Import =argv[3];
         }
         //--------------------------------------------------------------------
         else if ( argc >4 && strcmp(argv[4], "/addbf") ==0 )
         {
              gForFile =true;
              gwsVoca_Add_ImportForcePriority =true;

              printf("add import vocabulary file =%s\r\n", argv[3]);

              std::ifstream infile(argv[3]);
              std::string   line;

              while ( std::getline(infile, line) )
              {
                     line.erase(remove(line.begin(), line.end(), '\r'), line.end());
                     line.erase(remove(line.begin(), line.end(), '\n'), line.end());

                     //cout <<".size() "<<line.size()<<","<<line<<"\r\n";

                     if ( line.size() >0 )
                     {
                          gwsVoca_Add_Import +=line;
                     }
              }
              infile.close();
         }
         //--------------------------------------------------------------------
         else if ( argc >4 && strcmp(argv[4], "/addbf_imp") ==0 )
         {
              gForFile =true;
              gwsVoca_Add_ImportForcePriority =false;

              printf("add import vocabulary file =%s\r\n", argv[3]);

              std::ifstream infile(argv[3]);
              std::string   line;

              while ( std::getline(infile, line) )
              {
                     line.erase(remove(line.begin(), line.end(), '\r'), line.end());
                     line.erase(remove(line.begin(), line.end(), '\n'), line.end());

                     //cout <<".size() "<<line.size()<<","<<line<<"\r\n";

                     if ( line.size() >0 )
                     {
                          gwsVoca_Add_Import +=line;
                     }
              }
              infile.close();
         }
         //--------------------------------------------------------------------
         else if ( argc >4 && strcmp(argv[4], "/addbf_update") ==0 )
         {
              gForFile =true;
              gwsVoca_Add_ImportForcePriority =false;

              printf("update import vocabulary file =%s\r\n", argv[3]);

              std::ifstream infile(argv[3]);
              std::string   line;

              while ( std::getline(infile, line) )
              {
                     line.erase(remove(line.begin(), line.end(), '\r'), line.end());
                     line.erase(remove(line.begin(), line.end(), '\n'), line.end());

                     //cout <<".size() "<<line.size()<<","<<line<<"\r\n";

                     if ( line.size() >0 )
                     {
                          gwsVoca_Update_Import +=line;
                     }
              }
              infile.close();
         }
         else if ( argc >4 && strcmp(argv[4], "/delb") ==0 )
         {
              gForFile =true;

              printf("del import vocabulary =%s\r\n", argv[3]);

              gwsVoca_Del_Import =argv[3];
         }

         //--------------------------------------------------------------------
         else if ( strcmp(argv[2], "m") ==0 || strcmp(argv[2], "mv") ==0 )
         {
              if ( argc >4 && gForFile ==false )
              {
                   tabCount =atoi(argv[4]);
              }
              gMultiTap =true;

              printf("for MultiTap Mode\r\n");
         }
    }

    if ( strcmp(argv[2], "mk") ==0 )
    {
         imeID =atoi(argv[1]);
         gImeID =imeID;

         gMultiTap =true;
         gMultiTap_Looping =true;

         gOutVer =false;
    }
    #if !defined(__NO_KOREAN)
    else if ( strcmp(argv[2], "kr") ==0 )
    {
         imeID =atoi(argv[1]);
         gImeID =imeID;

         gKorean_Testing =true;
         gOutVer =false;
    }
    #endif

    if ( gInput.size() ==0 )
    {
         gInput.push_back(wsKeyin);
    }

    #if !defined(__NO_KOREAN)
    if ( gKorean_Testing ==false && gKaiOS_fmt ==false && gMultiTap ==false && gNextword ==false && gChkExist ==false && gChkLimit ==false && TestLearnWord_Looping ==false )
    #else
    if ( gKaiOS_fmt ==false && gMultiTap ==false && gNextword ==false && gChkExist ==false && gChkLimit ==false && TestLearnWord_Looping ==false )
    #endif
    {
         if ( argc >4 )
              maxTest =atoi(argv[4]);

         if ( gGetNumber ==0 && argc >5 )
              gGetNumber =atoi(argv[5]);
    }

    //-------------------------------------------------------------------------
    struct stat
    buffer;

    if ( (stat (fOpt.c_str(), &buffer) ==0) )
    {
         gBrackets =true;
    }
    else
    {
         gBrackets =false;
    }

    if ( (stat (fDbg.c_str(), &buffer) ==0) )
    {
         gDebug =true;
    }
    else
    {
         gDebug =false;
    }

    if ( (stat (fKey.c_str(), &buffer) ==0) )
    {
          printf("get opt_FullMatchKey =%d\r\n", gFullMatchKey);
          printf("set param FullMatchKey =1\r\n");
          IQQI_SetOption(opt_FullMatchKey, 1);
    }
    else
    {
          #if defined(__FOR_PUNKT) || defined(__FOR_HUAWEI)
          #else
          printf("get opt_FullMatchKey =%d\r\n", gFullMatchKey);
          printf("set param FullMatchKey =0\r\n");
          IQQI_SetOption(opt_FullMatchKey, 0);
          #endif
    }

    if ( (stat (fLen4.c_str(), &buffer) ==0) )
    {
         IQQI_SetOption(opt_4LenPriority, 1);
    }
    else
    {
         IQQI_SetOption(opt_4LenPriority, 0);
    }

    //-------------------------------------------------------------------------
    int
    maxIME =sizeof(dictMap) /sizeof(DICT_MAP);

    bool
    newID =true;

    //-------------------------------------------------------------------------

    if ( gAutoTest ==true )
    {
         int
         imeID_Test =imeID;

         wstring
         wsOut;

         wchar_t
         wsItem[1024] ={0};

         #if defined(__OUT_FORMAT_ANS)

         vector<KEY_LANG>::iterator
         ii =autoX.mKeys.begin();

         for ( ; ii !=autoX.mKeys.end(); ii++ )
         {
               swprintf(wsItem, 1024, L"Length\tKeyin\t");
               wsOut.append(wsItem);

               for ( int i =0; i <ii->count; i++ )
               {
                     swprintf(wsItem, 1024, L"#%d\t", i +1);
                     wsOut.append(wsItem);
               }
               wsOut.append(L"\r\n");

               // answer line
               KEY_LANG
               lang =*ii;

               swprintf(wsItem, 1024, L"name=%s\r\n", ii->name.c_str());
               wsOut.append(wsItem);

               // answer line
               vector<KEY_ANS>::iterator
               ii2 =ii->keys.begin();

               for ( ; ii2 !=ii->keys.end(); ii2++ )
               {
                     string
                     key1 =ii2->key;

                     swprintf(wsItem, 1024, L"%d\t%s", key1.size(), key1.c_str());
                     wsOut.append(wsItem);
                     wsOut.append(L"\t");

                     vector<wstring>::iterator
                     ii3 =ii2->ans.begin();

                     for ( int N =0; ii3 !=ii2->ans.end() && N <ii->count; ii3++, N++ )
                     {
                           swprintf(wsItem, 1024, L"%ls\t", ii3->c_str());
                           wsOut.append(wsItem);
                     }
                     wsOut.append(L"\r\n");
               }
               wsOut.append(L"----------------------------------------------------\r\n");
               wsOut.append(L"\r\n");

               printf("%ls\r\n", wsOut.c_str());
               wsOut.resize(0);
               printf("%ls\r\n", wsOut.c_str());
         }

         #endif

         auto_ii =autoX.mKeys.begin();

         for ( ; auto_ii !=autoX.mKeys.end(); auto_ii++ )
         {
               gAutoTest_succ =true;

               if ( imeID_Test ==999 || imeID_Test ==auto_ii->id )
               {
                    imeID =auto_ii->id;

                    char *
                    dictname =(char *)dictMap[imeID -1].dict_main.c_str();

                    if ( dictname !=NULL && dictname[strlen(dictname) -1] =='/' )
                    {
                         //printf("%2d,    %s, %s\r\n", dx +1, id_map[imeID -1].name, dictname);
                    }
                    else
                    {
                         auto_ii2 =auto_ii->keys.begin();

                         for ( ; auto_ii2 !=auto_ii->keys.end(); auto_ii2++ )
                         {
                               int
                               TotalWord =testAction(imeID, (wchar_t *)auto_ii2->key.c_str(), tabCount, newID);
                         }
                         char sss[256] ={0};
                         sprintf(sss, "%d, %s", auto_ii->id, auto_ii->name.c_str());

                         if ( gAutoTest_succ ==true )
                         {
                              gAutoTest_OK.count++;
                              gAutoTest_OK.imeName.push_back(sss);
                         }
                         else
                         {
                              if ( gAutoTest_Result.size() >0 )
                                   gAutoTest_Result.append(L"\r\n");

                              gAutoTest_Err.count++;
                              gAutoTest_Err.imeName.push_back(sss);
                         }
                    }
                    newID =true;
               }
         }

         printf("\r\n\r\n完全正確語言：%d\r\n", gAutoTest_OK.count);

         vector<string>::iterator
         iix =gAutoTest_OK.imeName.begin();

         for ( ; iix !=gAutoTest_OK.imeName.end(); iix++ )
         {
               printf("[%s]\r\n", iix->c_str());
         }

         printf("\r\n\r\n錯誤語言：" RED "%d\r\n" RESET, gAutoTest_Err.count);

         iix =gAutoTest_Err.imeName.begin();

         for ( ; iix !=gAutoTest_Err.imeName.end(); iix++ )
         {
               printf("[%s]\r\n", iix->c_str());
         }

         printf("\r\n\r\n錯誤內容：\r\n%ls", gAutoTest_Result.c_str());

    }
    //-------------------------------------------------------------------------
    else if ( gForFile ==true )
    {
         for ( int dx =0; dx <maxIME; dx++ )
         {
               if ( lag_to_tcl[dx].level <=3 )
               {
                    imeID =lag_to_tcl[dx].tcl_id;

                    char *
                    dictname =(char *)dictMap[imeID -1].dict_main.c_str();

                    if ( dictname !=NULL && dictname[strlen(dictname) -1] =='/' )
                    {
                         //printf("%2d,    %s, %s\r\n", dx +1, id_map[imeID -1].name, dictname);
                    }
                    else
                    {
                         vector<wstring>::iterator
                         it_i;

                         for ( it_i =gInput.begin(); it_i !=gInput.end(); ++it_i )
                         {
                               wstring
                               ws =*it_i;

                               if ( !gwsPhrs_Add.empty() || !gwsPhrs_Del.empty() || !gwsVoca_Add.empty() || !gwsVoca_Del.empty() )
                               {
                                    imeID =gImeID;
                               }

                               int
                               TotalWord =testAction(imeID, (wchar_t *)ws.c_str(), tabCount, newID);

                               if ( gForFile ==false && TotalWord ==0 )
                               {
                                    printf("%2d, %s, %s\r\n", imeID, id_map[imeID -1].name, dictname);
                                    printf("\r\n\r\nERROR!!!!!!!\r\n\r\n");
                                    exit(1);
                               }
                          }
                    }
                    newID =true;

                    if ( gOutVer ==false )
                    {
                         printf("\r\n\r\n\r\n");
                    }
               }
          }
    }
    else
    {
          int
          TotalWord =0;

          char *
          dictname =(char *)dictMap[imeID -1].dict_main.c_str();

          vector<wstring>::iterator
          it_i;

          #if !defined(__NO_KOREAN)
          if ( gKorean_Testing ==false && gMultiTap_Looping ==false )
          #else
          if ( gMultiTap_Looping ==false )
          #endif
          {
               for ( int ts =0; ts <maxTest; ts++ )
               {
                     for ( it_i =gInput.begin(); it_i !=gInput.end(); ++it_i )
                     {
                           wstring
                           ws =*it_i;

                           TotalWord =testAction(imeID, (wchar_t *)ws.c_str(), tabCount, newID);

                           if ( TotalWord ==0 )
                           {
                                printf("%2d, %s, %s\r\n", imeID, id_map[imeID -1].name, dictname);
                                printf("\r\n\r\nKey =%ls, TotalWord =0, ERROR!!!!!!!\r\n\r\n", ws.c_str());
                                exit(1);
                           }
                     }
                }
           }

           //------------------------------------------------------------------
           //
           //------------------------------------------------------------------
           if ( TestLearnWord_Looping ==true )
           {
                gLastSelectedBuf.clear();

                wstring
                wsKeyinTest =wsKeyin;

                while ( 1 )
                {
                        //printf("\r\n-------------------------------------------------------------------------");
                        printf("\r\n\r\n");
                        printf("\r\n編輯區內容 : %ls", gLastSelectedBuf.c_str());

                        string
                        key1;

                        while ( 1 )
                        {
                                printf("\r\n>>請輸入 Next Key (0-9), a-add, d-del, i=import, b=begin, e=end, c=clear, [Ctrl-C結束]>>\r\n");

                                while ( true )
                                {
                                        int
                                        c =getchar();

                                        int
                                        Ch =c ==KEY_STAR_2 ?KEY_STAR_2 :c;

                                        if ( Ch =='c' )
                                        {
                                             cin.ignore();
                                             gLastSelectedBuf.clear();
                                             wsKeyinTest.clear();
                                             break;
                                        }
                                        else if ( Ch =='a' || Ch =='d' || Ch =='i'|| Ch =='b' || Ch =='e' )
                                        {
                                             cin.ignore();

                                             int
                                             SelectedIndex =1;

                                             printf("\r\n>>請選擇 輸出(Output) 哪一個候選字 [Enter 2次結束選取]>>\r\n");

                                             while ( 1 )
                                             {
                                                     while ( true )
                                                     {
                                                             int
                                                             c =getchar();
                                                             key1.append(1, c);
                                                             if ( c ==0 || c ==0xa )
                                                             {
                                                                  break;
                                                             }
                                                             cin.ignore();
                                                     }
                                                     SelectedIndex =atoi(key1.c_str());

                                                     if ( SelectedIndex <1 || SelectedIndex >TotalWord )
                                                     {
                                                          char fmt[120] ={0};
                                                          sprintf(fmt, ">>" RED " error! " RESET "您選了[%d], 但只能在(1-%d)之間做選取!!", SelectedIndex, TotalWord);
                                                          printf("%s", fmt);
                                                          key1.clear();
                                                          continue;
                                                     }
                                                     break;
                                             }
                                             printf(">>  您選了 %d\r\n\r\n", SelectedIndex);

                                             if ( Ch =='a' )
                                             {
                                                  gwsVoca_Add =gCandVoca.record(SelectedIndex -1);

                                                  replaceAll(gwsVoca_Add, L"\'\'", L"\'");

                                                  int
                                                  //retVal =IQQI_LearnWord(gImeID, (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str());
                                                  retVal =IQQI_LearnWordEx(gImeID, (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str(), true, true);

                                                  printf("Add retVal =%d\r\n", retVal);
                                                  showErr(retVal);

                                                  #if defined(__USER_DIC_SAVE_CMD)
                                                  if ( retVal ==0 )
                                                  {
                                                       gUserFile.append_keys(gImeID, (wchar_t *)L"", (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str());
                                                       gUserFile.phrsSaveData(sUserWordFname);
                                                  }
                                                  #endif

                                                  gwsVoca_Add.clear();
                                             }
                                             else if ( Ch =='d' )
                                             {
                                                  gwsVoca_Del =gCandVoca.record(SelectedIndex -1);

                                                  replaceAll(gwsVoca_Del, L"\'\'", L"\'");

                                                  int
                                                  retVal =IQQI_DeleLearnedWord(gImeID, (wchar_t *)L"", (wchar_t *)gwsVoca_Del.c_str());

                                                  printf("Del retVal =%d\r\n", retVal);
                                                  showErr(retVal);

                                                  #if defined(__USER_DIC_SAVE_CMD)
                                                  if ( retVal ==0 )
                                                  {
                                                  // delete UserData (同時刪除 'u'(Import) & 'm'(MultiTap))
                                                  gUserFile.remove(gImeID, (wchar_t *)L"m", (wchar_t *)gwsVoca_Del.c_str());
                                                  gUserFile.remove(gImeID, (wchar_t *)L"u", (wchar_t *)gwsVoca_Del.c_str());
                                                  gUserFile.phrsSaveData(sUserWordFname);
                                                  }
                                                  #endif

                                                  gwsVoca_Del.clear();
                                             }
                                             else if ( Ch =='b' )
                                             {
                                                  int retVal =IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)L"[[begin]]", false, false);
                                             }
                                             else if ( Ch =='e' )
                                             {
                                                  int retVal =IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)L"[[end]]", false, false);
                                             }
                                             else if ( Ch =='i' )
                                             {
                                                  gwsVoca_Add =gCandVoca.record(SelectedIndex -1);

                                                  replaceAll(gwsVoca_Add, L"\'\'", L"\'");

                                                  int
                                                  //retVal =IQQI_LearnWordEx(gImeID, (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str(), false, false);
                                                  retVal =IQQI_LearnWordUpdate(gImeID, (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str(), false, false);

                                                  printf("Add retVal =%d\r\n", retVal);
                                                  showErr(retVal);

                                                  #if defined(__USER_DIC_SAVE_CMD)
                                                  if ( retVal ==0 )
                                                  {
                                                       gUserFile.append_keys(gImeID, (wchar_t *)L"", (wchar_t *)L"", (wchar_t *)gwsVoca_Add.c_str());
                                                       gUserFile.phrsSaveData(sUserWordFname);
                                                  }
                                                  #endif

                                                  gwsVoca_Add.clear();
                                             }
                                             gLastSelectedBuf.clear();
                                             wsKeyinTest.clear();
                                             break;
                                        }
                                        else
                                        {
                                             wsKeyinTest.append(1, Ch);
                                             cin.ignore();

                                             newID =true;

                                             TotalWord =testAction(imeID, (wchar_t *)wsKeyinTest.c_str(), 1, newID);

                                             if ( TotalWord ==0 )
                                             {
                                                  printf("%2d, %s, %s, Keyin =%ls\r\n", imeID, id_map[imeID -1].name, dictname, wsKeyinTest.c_str());
                                                  printf("\r\n\r\nERROR!!!!!!!\r\n\r\n");
                                                  ///exit(1);
                                                  gLastSelectedBuf.clear();
                                                  wsKeyinTest.clear();
                                             }
                                             break;
                                        }
                                        //cin.ignore();
                                }
                                break; // temprary no check key is OK(0-9) or Not!!
                        }
                }
                return 1;
           }

           //------------------------------------------------------------------
           // 測試韓文輸入
           //------------------------------------------------------------------
           #if !defined(__NO_KOREAN)
           if ( gKorean_Testing ==true )
           {
                if ( KrTestFile.size() !=0 )
                {
                     autotest_kr_ex
                     runx;

                     printf("run Test file =%s\r\n", KrTestFile.c_str());

                     struct stat
                     myStat;

                     bool
                     exist =(stat(KrTestFile.c_str(), &myStat) == 0);

                     if ( exist ==true )
                     {
                          runx.calcTest(KrTestFile.c_str());
                          runx.runTest(KrTestFile.c_str(), true, false);
                     }
                     else
                          printf("%s not found!!\r\n", KrTestFile.c_str());
                }
                else
                {
                     while ( 1 )
                     {
                             //printf("\r\n-------------------------------------------------------------------------");
                             printf("\r\n\r\n\r\n");
                             printf("\r\n編輯區 Key = : %ls, 組字內容 =%ls", gKeyComp.c_str(), gKeyCompBuf.c_str());

                             string
                             key1;

                             while ( 1 )
                             {
                                     printf("\r\n>>請輸入 Key [Ctrl-C結束]>>\r\n");

                                     map<char, wstring>::iterator
                                     kk =gkKKey.begin();

                                     for ( int y =0; y <5; y++ )
                                     {
                                           for ( int x =0; x <7; x++ )
                                           {
                                                 if ( kk ==gkKKey.end() )
                                                      break;

                                                 printf("%c :" RED "%ls      " RESET, kk->first, kk->second.c_str());
                                                 kk++;
                                           }

                                           printf("\r\n");

                                     }

                                     while ( true )
                                     {
                                             int
                                             c =getchar();

                                             if ( c =='z' )
                                             {
                                                  gKeyComp.resize(0);
                                             }
                                             else if ( c =='x' )
                                             {
                                                  if ( gKeyComp.size() >0 )
                                                       gKeyComp.erase(gKeyComp.size() -1, 1);
                                             }
                                             else
                                             {
                                                  wstring
                                                  wCH =gkKKey[c];

                                                  gKeyComp +=wCH;
                                             }
                                             break;
                                     }
                                     break;
                             }

                             //gKeyComp +=key1.c_str();
                             IQQI_GetComposingText(imeID, (wchar_t *)gKeyComp.c_str(), 0, 0);//gKoreanX.getComposingText(gKeyComp);

                             key1.clear();
                             cin.ignore();
                     }
                }
           }
           #endif

           //------------------------------------------------------------------
           // 測試動態輪循
           //------------------------------------------------------------------
           #if !defined(__NO_KOREAN)
           if ( gKorean_Testing ==false && gMultiTap_Looping ==true )
           #else
           if ( gMultiTap_Looping ==true )
           #endif
           {
                gLastSelectedBuf.clear();

                while ( 1 )
                {
                        //printf("\r\n-------------------------------------------------------------------------");
                        printf("\r\n\r\n\r\n");
                        printf("\r\n編輯區內容 : %ls", gLastSelectedBuf.c_str());

                        string
                        key1;

                        while ( 1 )
                        {
                                printf("\r\n>>請輸入(MultiTap) Key (0-9) [Ctrl-C結束]>>\r\n");

                                while ( true )
                                {
                                        int
                                        c =getchar();
                                        //if ( c ==0xd || c ==0xa )
                                        //     break;

                                        //0x40 ='@'
                                        int
                                        Ch =c ==KEY_STAR_2 ?KEY_STAR_2 :c;

                                        key1.append(1, Ch);
                                        break;
                                }
                                break; // temprary no check key is OK(0-9) or Not!!
                        }

                        newID =true;

                        TotalWord =testAction(imeID, (wchar_t *)key1.c_str(), 1, newID);

                        if ( TotalWord ==0 )
                        {
                             printf("%2d, %s, %s\r\n", imeID, id_map[imeID -1].name, dictname);
                             printf("\r\n\r\nERROR!!!!!!!\r\n\r\n");
                             exit(1);
                        }

                        key1.clear();
                        cin.ignore();

                        while ( 1 )
                        {
                                int
                                SelectedIndex =1;

                                while ( 1 )
                                {
                                        printf("\r\n>>請選擇 輸出(Output) 哪一個候選字 [Ctrl-C結束]>>\r\n");

                                        int
                                        c =getchar();
                                        while ( true )
                                        {
                                                key1.append(1, c);
                                                c =getchar();
                                                if ( c ==0 || c ==0xa )
                                                     break;
                                        }

                                        SelectedIndex =atoi(key1.c_str());

                                        if ( SelectedIndex <1 || SelectedIndex >gTotalSelect )
                                        {
                                             char fmt[120] ={0};
                                             sprintf(fmt, ">>" RED " error! " RESET "只能在(1-%d)之間做選取!!", gTotalSelect);
                                             printf("%s", fmt);
                                             key1.clear();
                                             continue;
                                        }
                                        break;
                                }
                                printf(">>  您選了 %d\r\n\r\n", SelectedIndex);

                                wchar_t
                                MultiSz[1024] ={0};
                                wcscpy(MultiSz, gMap.GetMultiSz());

                                int
                                index =0,
                                wsLen =wcslen(MultiSz);

                                wchar_t*
                                pSz =MultiSz;

                                for ( int i =0; i <wsLen; i++ )
                                {
                                      if ( MultiSz[i] =='|' )
                                      {
                                           MultiSz[i] =0;
                                           if ( ++index ==SelectedIndex )
                                           {
                                                 gLastSelectedBuf +=(index ==1 ? pSz +1 :pSz);
                                                 break;
                                           }
                                           pSz =MultiSz +(i +1);
                                     }
                                }
                                break; // temprary no check number is OK or Not!!
                        }
                        //printf("\r\n輸出      : %ls", gLastSelectedBuf.c_str());
                        //cin.ignore();
                }
           }
    }
    printf("\r\n");
    exit(1);
}

#if defined(__MAIN)

int main(int argc, char *argv[])
{
    return main_ex(argc, argv);
}

#else

int testEngine(int imeID, const wchar_t* wsKeyin)
{
    bool
    newID =true;

    switch ( imeID )
    {
             case IQQI_IME_Changjie:
             case IQQI_IME_Chinese_CN:
             case IQQI_IME_Chinese_TW:
                  gKaiOS_fmt =true;
                  break;
             default:
                  break;
    }

    int
    TotalWord =testAction(imeID, (wchar_t* )wsKeyin, 1, newID);

    if ( TotalWord ==0 )
    {

    }
}

#endif

#endif
