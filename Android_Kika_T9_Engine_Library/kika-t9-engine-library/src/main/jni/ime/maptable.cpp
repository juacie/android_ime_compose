#include "stdafx.h"

#if defined(__LINUX_BASE)
 #include <sys/time.h>
#endif

//#define __DEBUG_MSG
//#define __DEBUG_TBL_MSG
//#define __DEBUG_KEYCODE_MSG
//#define __DEBUG_MSG_HINDI
//#define __DEBUG_MSG_MAPTABLE_EX

#if defined(__DEBUG_MSG)
 #define LOG_KIKA_TAG        "Kika-ENGINE"
 #include <android/log.h>
 #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)
#endif


#pragma warning(disable: 4786)
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

#include "maptable.h"

#if !defined(__NO_KIKA_IME)

#include "lang/Afrikaans.h"
#include "lang/Albanian.h"
#include "lang/Arabic.h"
#include "lang/Armenian.h"
#include "lang/Azerbaijani.h"
#include "lang/Belarusian.h"
#include "lang/Bengali_BD.h"
#include "lang/Bengali_IN.h"
#include "lang/Bosnian.h"
#include "lang/Bulgarian.h"
#include "lang/Chinese_HK.h"
#include "lang/Chinese_TW.h"
#include "lang/Chinese_CN.h"
#include "lang/Croatian.h"
#include "lang/Czech.h"
#include "lang/Danish.h"
#include "lang/Dutch.h"
#include "lang/English_GB.h"
#include "lang/English_US.h"
#include "lang/Estonian.h"
#include "lang/Finnish.h"
#include "lang/French_CA.h"
#include "lang/French_FR.h"
#include "lang/Georgian.h"
#include "lang/German.h"
#include "lang/Greek.h"
#include "lang/Hebrew.h"
#include "lang/Hindi.h"
#include "lang/Hungarian.h"
#include "lang/Icelandic.h"
#include "lang/Indonesian.h"
#include "lang/Italian.h"
#include "lang/Kazakh.h"
#include "lang/Khmer.h"
#include "lang/Lao.h"
#include "lang/Latvian.h"
#include "lang/Lithuanian.h"
#include "lang/Macedonian.h"
#include "lang/Malay.h"
#include "lang/Norwegian.h"
#include "lang/Persian.h"
#include "lang/Polish.h"
#include "lang/Portuguese_BR.h"
#include "lang/Portuguese_PT.h"
#include "lang/Romanian.h"
#include "lang/Russian.h"
#include "lang/Serbian.h"
#include "lang/Sinhala.h"
#include "lang/Slovak.h"
#include "lang/Slovenian.h"
#include "lang/Spanish_ES.h"
#include "lang/Spanish_US.h"
#include "lang/Swahili.h"
#include "lang/Swedish.h"
#include "lang/Tagalog.h"
#include "lang/Tamil.h"
#include "lang/Telugu.h"
#include "lang/Thai.h"
#include "lang/Turkish.h"
#include "lang/Ukrainian.h"
#include "lang/Urdu.h"
#include "lang/Uzbek_Cyrillic.h"
#include "lang/Vietnamese.h"
#include "lang/Xhosa.h"
#include "lang/Zulu.h"
#include "lang/Basque.h"
#include "lang/Catalan.h"
#include "lang/Galician.h"
#include "lang/Changjie.h"
#include "lang/Malayalam.h"
#include "lang/Kannada.h"
#include "lang/Assamese.h"
#include "lang/Gujarati.h"
#include "lang/Marathi.h"
#include "lang/Nepali.h"
#include "lang/Oriya.h"
#include "lang/Punjabi.h"
#include "lang/Farsi.h"
#include "lang/Maithili.h"
#include "lang/Burmese_Z.h"
#include "lang/Burmese_U.h"
#include "lang/Tibetan.h"
#include "lang/Uyghur.h"
#include "lang/Jawa.h"
#include "lang/Maori.h"
#include "lang/Mongolian.h"
#include "lang/Amharic.h"

#if !defined(__NO_KOREAN)
#include "lang/Korean.h"
#endif

#endif

struct timeval maptable::MultiType_StartTime = {0};

int maptable::MultiType_KeyCount    = 0;
int maptable::MultiType_Time        = 1000;
int maptable::MultiType_LastKeyCode = -1;

#define KEY_EXTRA_ALLOW_CH_QUOTATION       '\''
//#define KEY_EXTRA_ALLOW_CH_MINUS           '-'
#define KEY_EXTRA_ALLOW_CH_0               '0'
#define KEY_EXTRA_ALLOW_CH_2               '2'

#if !defined(__NO_KIKA_IME)

vector<wstring> Hindi_wData1_T2;
vector<wstring> Hindi_wData2_T3;
vector<wstring> Hindi_wData3_T4;

vector<wstring> Tamil_wData1_T2;
vector<wstring> Tamil_wData2_T3;

vector<wstring> Telugu_wData1_T2;
vector<wstring> Telugu_wData1_T3;

#if !defined(__HUAWEI) && defined(__SONIM)
vector<wstring> Bengali_BD_wData1_T2;
vector<wstring> Bengali_BD_wData2_T3;

vector<wstring> Bengali_IN_wData1_T2;
vector<wstring> Bengali_IN_wData2_T3;
#endif


#endif

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
maptable::maptable()
{
     mBTotal =0;
     mKbMode =for_T9;

     setlocale( LC_ALL, "");

     //------------------------------------------------------------------------
     #if !defined(__NO_KIKA_IME)

     wchar_t*
     p = Hindi::wData1;

     for ( int i =0; Hindi::wData1[i] !='@'; i++ )
     {
           if ( Hindi::wData1[i] =='\t' )
           {
                Hindi::wData1[i] ='\0';

                Hindi_wData1_T2.push_back(p);
                p =Hindi::wData1 +(i +1);
           }
     }

     p = Hindi::wData2;

     for ( int i =0; Hindi::wData2[i] !='@'; i++ )
     {
           if ( Hindi::wData2[i] =='\t' )
           {
                Hindi::wData2[i] ='\0';

                Hindi_wData2_T3.push_back(p);
                p =Hindi::wData2 +(i +1);
           }
     }

     p = Hindi::wData3;

     for ( int i =0; Hindi::wData3[i] !='@'; i++ )
     {
           if ( Hindi::wData3[i] =='\t' )
           {
                Hindi::wData3[i] ='\0';

                Hindi_wData3_T4.push_back(p);
                p =Hindi::wData3 +(i +1);
           }
     }

     #if defined(__DEBUG_MSG_HINDI) && defined(__TEST)

     vector<wstring>::iterator
     itr =Hindi_wData1_T2.begin();

     for ( int i =0; itr !=Hindi_wData1_T2.end(); itr++ )
     {
           printf("Hindi_wData1_T2(%02d) =%ls\r\n", i, itr->c_str());
     }
     #endif

     //------------------------------------------------------------------------
     p = Tamil::wData1;

     for ( int i =0; Tamil::wData1[i] !='@'; i++ )
     {
           if ( Tamil::wData1[i] =='\t' )
           {
                Tamil::wData1[i] ='\0';

                Tamil_wData1_T2.push_back(p);
                p =Tamil::wData1 +(i +1);
           }
     }

     p = Tamil::wData2;

     for ( int i =0; Tamil::wData2[i] !='@'; i++ )
     {
           if ( Tamil::wData2[i] =='\t' )
           {
                Tamil::wData2[i] ='\0';

                Tamil_wData2_T3.push_back(p);
                p =Tamil::wData2 +(i +1);
           }
     }

     //------------------------------------------------------------------------
     p = Telugu::wData1;

     for ( int i =0; Telugu::wData1[i] !='@'; i++ )
     {
           if ( Telugu::wData1[i] =='\t' )
           {
                Telugu::wData1[i] ='\0';

                Telugu_wData1_T2.push_back(p);
                p =Telugu::wData1 +(i +1);
           }
     }

     p = Telugu::wData2;

     for ( int i =0; Telugu::wData2[i] !='@'; i++ )
     {
           if ( Telugu::wData2[i] =='\t' )
           {
                Telugu::wData2[i] ='\0';

                Telugu_wData1_T3.push_back(p);
                p =Telugu::wData2 +(i +1);
           }
     }

     #if !defined(__HUAWEI) && defined(__SONIM)
     //------------------------------------------------------------------------
     p = Bengali_BD::wData1;

     for ( int i =0; Bengali_BD::wData1[i] !='@'; i++ )
     {
           if ( Bengali_BD::wData1[i] =='\t' )
           {
                Bengali_BD::wData1[i] ='\0';

                Bengali_BD_wData1_T2.push_back(p);
                p =Bengali_BD::wData1 +(i +1);
           }
     }

     p = Bengali_BD::wData2;

     for ( int i =0; Bengali_BD::wData2[i] !='@'; i++ )
     {
           if ( Bengali_BD::wData2[i] =='\t' )
           {
                Bengali_BD::wData2[i] ='\0';

                Bengali_BD_wData2_T3.push_back(p);
                p =Bengali_BD::wData2 +(i +1);
           }
     }

     //------------------------------------------------------------------------
     p = Bengali_IN::wData1;

     for ( int i =0; Bengali_IN::wData1[i] !='@'; i++ )
     {
           if ( Bengali_IN::wData1[i] =='\t' )
           {
                Bengali_IN::wData1[i] ='\0';

                Bengali_IN_wData1_T2.push_back(p);
                p =Bengali_IN::wData1 +(i +1);
           }
     }

     p = Bengali_IN::wData2;

     for ( int i =0; Bengali_IN::wData2[i] !='@'; i++ )
     {
           if ( Bengali_IN::wData2[i] =='\t' )
           {
                Bengali_IN::wData2[i] ='\0';

                Bengali_IN_wData2_T3.push_back(p);
                p =Bengali_IN::wData2 +(i +1);
           }
     }
     #endif

     #endif

     //------------------------------------------------------------------------
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄅ", L"1"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄆ", L"q"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄇ", L"a"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄈ", L"z"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄉ", L"2"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄊ", L"w"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄋ", L"s"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄌ", L"x"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄍ", L"e"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄎ", L"d"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄏ", L"c"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄐ", L"r"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄑ", L"f"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄒ", L"v"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄓ", L"5"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄔ", L"t"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄕ", L"g"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄖ", L"b"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄗ", L"y"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄘ", L"h"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄙ", L"n"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄚ", L"8"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄛ", L"i"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄜ", L"k"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄝ", L","));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄞ", L"9"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄟ", L"o"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄠ", L"l"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄡ", L"."));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄢ", L"0"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄣ", L"p"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄤ", L";"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄥ", L"/"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄦ", L"-"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄧ", L"u"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄨ", L"j"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄩ", L"m"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ˋ",  L"4"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ˊ",  L"6"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ˇ",  L"3"));
     mZhStrokes.insert(pair<wstring, wstring>(L"˙",  L"7"));
     mZhStrokes.insert(pair<wstring, wstring>(L"ㄦ", L"-"));

     //------------------------------------------------------------------------
     mEngZhStrokes.insert(pair<wstring, wstring>(L"1", L"ㄅ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"q", L"ㄆ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"a", L"ㄇ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"z", L"ㄈ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"2", L"ㄉ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"w", L"ㄊ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"s", L"ㄋ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"x", L"ㄌ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"e", L"ㄍ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"d", L"ㄎ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"c", L"ㄏ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"r", L"ㄐ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"f", L"ㄑ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"v", L"ㄒ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"5", L"ㄓ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"t", L"ㄔ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"g", L"ㄕ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"b", L"ㄖ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"y", L"ㄗ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"h", L"ㄘ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"n", L"ㄙ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"8", L"ㄚ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"i", L"ㄛ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"k", L"ㄜ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L",", L"ㄝ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"9", L"ㄞ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"o", L"ㄟ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"l", L"ㄠ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L".", L"ㄡ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"0", L"ㄢ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"p", L"ㄣ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L";", L"ㄤ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"/", L"ㄥ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"-", L"ㄦ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"u", L"ㄧ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"j", L"ㄨ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"m", L"ㄩ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"4", L"ˋ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"6", L"ˊ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"3", L"ˇ"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"7", L"˙"));
     mEngZhStrokes.insert(pair<wstring, wstring>(L"-", L"ㄦ"));
     
     //------------------------------------------------------------------------
     mCjStrokes.insert(pair<wstring, wstring>(L"日", L"a"));
     mCjStrokes.insert(pair<wstring, wstring>(L"月", L"b"));
     mCjStrokes.insert(pair<wstring, wstring>(L"金", L"c"));
     mCjStrokes.insert(pair<wstring, wstring>(L"木", L"d"));
     mCjStrokes.insert(pair<wstring, wstring>(L"水", L"e"));
     mCjStrokes.insert(pair<wstring, wstring>(L"火", L"f"));
     mCjStrokes.insert(pair<wstring, wstring>(L"土", L"g"));
     mCjStrokes.insert(pair<wstring, wstring>(L"竹", L"h"));
     mCjStrokes.insert(pair<wstring, wstring>(L"戈", L"i"));
     mCjStrokes.insert(pair<wstring, wstring>(L"十", L"j"));
     mCjStrokes.insert(pair<wstring, wstring>(L"大", L"k"));
     mCjStrokes.insert(pair<wstring, wstring>(L"中", L"l"));
     mCjStrokes.insert(pair<wstring, wstring>(L"一", L"m"));
     mCjStrokes.insert(pair<wstring, wstring>(L"弓", L"n"));
     mCjStrokes.insert(pair<wstring, wstring>(L"人", L"o"));
     mCjStrokes.insert(pair<wstring, wstring>(L"心", L"p"));
     mCjStrokes.insert(pair<wstring, wstring>(L"手", L"q"));
     mCjStrokes.insert(pair<wstring, wstring>(L"口", L"r"));
     mCjStrokes.insert(pair<wstring, wstring>(L"尸", L"s"));
     mCjStrokes.insert(pair<wstring, wstring>(L"廿", L"t"));
     mCjStrokes.insert(pair<wstring, wstring>(L"山", L"u"));
     mCjStrokes.insert(pair<wstring, wstring>(L"女", L"v"));
     mCjStrokes.insert(pair<wstring, wstring>(L"田", L"w"));
     mCjStrokes.insert(pair<wstring, wstring>(L"難", L"x"));
     mCjStrokes.insert(pair<wstring, wstring>(L"卜", L"y"));
     mCjStrokes.insert(pair<wstring, wstring>(L"Z", L"z"));
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void maptable::setKbMode(KEYBD_MODE mode)
{
     #if defined(__DEBUG_MSG_MAPTABLE_EX)
     KIKA_LOGD("maptabl_Numb2keyCode::setKbMode =%d\r\n", mode);
     #endif

     mKbMode =mode;
}

#if !defined(__NO_KIKA_IME)

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void maptable::MapVkTableEx(int imeID, unsigned int &total, VirtualKeybdMapping **table, KEYBD_MODE mode)
{
     //------------------------------------------------------------------------
     struct _T9
     {
          int                     total;
          _VirtualKeybdMapping*   table;
     } T9[] =
     {
      {ENGLISH_US_LAYOUT_LETTER_NUM_T9,      English_US::VK_Layout_T9},      //IQQI_IME_English_US
      {FRENCH_CA_LAYOUT_LETTER_NUM_T9,       French_CA::VK_Layout_T9},       //IQQI_IME_French_CA
      {PORTUGUESE_BR_LAYOUT_LETTER_NUM_T9,   Portuguese_BR::VK_Layout_T9},   //IQQI_IME_Portuguese_BR
      {SPANISH_US_LAYOUT_LETTER_NUM_T9,      Spanish_US::VK_Layout_T9},      //IQQI_IME_Spanish_US
      {ASSAMESE_LAYOUT_LETTER_NUM_T9,        Assamese::VK_Layout_T9},        //IQQI_IME_Assamese
      {BENGALI_IN_LAYOUT_LETTER_NUM_T9,      Bengali_IN::VK_Layout_T9},      //IQQI_IME_Bengali_IN
      {0,                                    0                        }  ,   //IQQI_IME_Bodo
      {0,                                    0                        },     //IQQI_IME_Dogri
      {GUJARATI_LAYOUT_LETTER_NUM_T9,        Gujarati::VK_Layout_T9},        //IQQI_IME_Gujarati
      {HINDI_LAYOUT_LETTER_NUM_T9,           Hindi::VK_Layout_T9},           //IQQI_IME_Hindi
      {KANNADA_LAYOUT_LETTER_NUM_T9,         Kannada::VK_Layout_T9},         //IQQI_IME_Kannada
      {0,                                    0                        },     //IQQI_IME_Kashmiri
      {0,                                    0                        },     //IQQI_IME_Konkani
      {MALAYALAM_LAYOUT_LETTER_NUM_T9,       Malayalam::VK_Layout_T9},       //IQQI_IME_Malayalam
      {0,                                    0                        },     //IQQI_IME_Manipuri
      {MARATHI_LAYOUT_LETTER_NUM_T9,         Marathi::VK_Layout_T9},         //IQQI_IME_Marathi
      {NEPALI_LAYOUT_LETTER_NUM_T9,          Nepali::VK_Layout_T9},          //IQQI_IME_Nepali
      {ORIYA_LAYOUT_LETTER_NUM_T9,           Oriya::VK_Layout_T9},           //IQQI_IME_Oriya
      {PUNJABI_LAYOUT_LETTER_NUM_T9,         Punjabi::VK_Layout_T9},         //IQQI_IME_Punjabi
      {0,                                    0                        },     //IQQI_IME_Sanskrit
      {0,                                    0                        },     //IQQI_IME_Santhali
      {0,                                    0                        },     //IQQI_IME_Sindhi
      {TAMIL_LAYOUT_LETTER_NUM_T9,           Tamil::VK_Layout_T9},           //IQQI_IME_Tamil
      {TELUGU_LAYOUT_LETTER_NUM_T9,          Telugu::VK_Layout_T9},          //IQQI_IME_Telugu
      {URDU_LAYOUT_LETTER_NUM_T9,            Urdu::VK_Layout_T9},            //IQQI_IME_Urdu
      {AFRIKAANS_LAYOUT_LETTER_NUM_T9,       Afrikaans::VK_Layout_T9},       //IQQI_IME_Afrikaans
      {ARABIC_LAYOUT_LETTER_NUM_T9,          Arabic::VK_Layout_T9},          //IQQI_IME_Arabic
      {CHINESE_CN_LAYOUT_LETTER_NUM_T9,      Chinese_CN::VK_Layout_T9},      //IQQI_IME_Chinese_CN
      {DUTCH_LAYOUT_LETTER_NUM_T9,           Dutch::VK_Layout_T9},           //IQQI_IME_Dutch
      {ENGLISH_GB_LAYOUT_LETTER_NUM_T9,      English_GB::VK_Layout_T9},      //IQQI_IME_English_GB
      {FRENCH_FR_LAYOUT_LETTER_NUM_T9,       French_FR::VK_Layout_T9},       //IQQI_IME_French_FR
      {GERMAN_LAYOUT_LETTER_NUM_T9,          German::VK_Layout_T9},          //IQQI_IME_German
      {HUNGARIAN_LAYOUT_LETTER_NUM_T9,       Hungarian::VK_Layout_T9},       //IQQI_IME_Hungarian
      {INDONESIAN_LAYOUT_LETTER_NUM_T9,      Indonesian::VK_Layout_T9},      //IQQI_IME_Indonesian
      {ITALIAN_LAYOUT_LETTER_NUM_T9,         Italian::VK_Layout_T9},         //IQQI_IME_Italian
      {MALAY_LAYOUT_LETTER_NUM_T9,           Malay::VK_Layout_T9},           //IQQI_IME_Malay
      {PERSIAN_LAYOUT_LETTER_NUM_T9,         Persian::VK_Layout_T9},         //IQQI_IME_Persian
      {POLISH_LAYOUT_LETTER_NUM_T9,          Polish::VK_Layout_T9},          //IQQI_IME_Polish
      {PORTUGUESE_PT_LAYOUT_LETTER_NUM_T9,   Portuguese_PT::VK_Layout_T9},   //IQQI_IME_Portuguese_PT
      {ROMANIAN_LAYOUT_LETTER_NUM_T9,        Romanian::VK_Layout_T9},        //IQQI_IME_Romanian
      {RUSSIAN_LAYOUT_LETTER_NUM_T9,         Russian::VK_Layout_T9},         //IQQI_IME_Russian
      {SPANISH_ES_LAYOUT_LETTER_NUM_T9,      Spanish_ES::VK_Layout_T9},      //IQQI_IME_Spanish_ES
      {SWAHILI_LAYOUT_LETTER_NUM_T9,         Swahili::VK_Layout_T9},         //IQQI_IME_Swahili
      {THAI_LAYOUT_LETTER_NUM_T9,            Thai::VK_Layout_T9},            //IQQI_IME_Thai
      {TURKISH_LAYOUT_LETTER_NUM_T9,         Turkish::VK_Layout_T9},         //IQQI_IME_Turkish
      {VIETNAMESE_LAYOUT_LETTER_NUM_T9,      Vietnamese::VK_Layout_T9},      //IQQI_IME_Vietnamese
      {ZULU_LAYOUT_LETTER_NUM_T9,            Zulu::VK_Layout_T9},            //IQQI_IME_Zulu
      {BENGALI_BD_LAYOUT_LETTER_NUM_T9,      Bengali_BD::VK_Layout_T9},      //IQQI_IME_Bengali_BD
      {BULGARIAN_LAYOUT_LETTER_NUM_T9,       Bulgarian::VK_Layout_T9},       //IQQI_IME_Bulgarian
      {CROATIAN_LAYOUT_LETTER_NUM_T9,        Croatian::VK_Layout_T9},        //IQQI_IME_Croatian
      {CZECH_LAYOUT_LETTER_NUM_T9,           Czech::VK_Layout_T9},           //IQQI_IME_Czech
      {FINNISH_LAYOUT_LETTER_NUM_T9,         Finnish::VK_Layout_T9},         //IQQI_IME_Finnish
      {GREEK_LAYOUT_LETTER_NUM_T9,           Greek::VK_Layout_T9},           //IQQI_IME_Greek
      {KAZAKH_LAYOUT_LETTER_NUM_T9,          Kazakh::VK_Layout_T9},          //IQQI_IME_Kazakh
      {KHMER_LAYOUT_LETTER_NUM_T9,           Khmer::VK_Layout_T9},           //IQQI_IME_Khmer
      {MACEDONIAN_LAYOUT_LETTER_NUM_T9,      Macedonian::VK_Layout_T9},      //IQQI_IME_Macedonian
      {SERBIAN_LAYOUT_LETTER_NUM_T9,         Serbian::VK_Layout_T9},         //IQQI_IME_Serbian
      {SINHALA_LAYOUT_LETTER_NUM_T9,         Sinhala::VK_Layout_T9},         //IQQI_IME_Sinhala
      {SLOVAK_LAYOUT_LETTER_NUM_T9,          Slovak::VK_Layout_T9},          //IQQI_IME_Slovak
      {SLOVENIAN_LAYOUT_LETTER_NUM_T9,       Slovenian::VK_Layout_T9},       //IQQI_IME_Slovenian
      {SWEDISH_LAYOUT_LETTER_NUM_T9,         Swedish::VK_Layout_T9},         //IQQI_IME_Swedish
      {TAGALOG_LAYOUT_LETTER_NUM_T9,         Tagalog::VK_Layout_T9},         //IQQI_IME_Tagalog
      {UKRAINIAN_LAYOUT_LETTER_NUM_T9,       Ukrainian::VK_Layout_T9},       //IQQI_IME_Ukrainian
      {XHOSA_LAYOUT_LETTER_NUM_T9,           Xhosa::VK_Layout_T9},           //IQQI_IME_Xhosa
      {ALBANIAN_LAYOUT_LETTER_NUM_T9,        Albanian::VK_Layout_T9},        //IQQI_IME_Albanian
      {ARMENIAN_LAYOUT_LETTER_NUM_T9,        Armenian::VK_Layout_T9},        //IQQI_IME_Armenian
      {AZERBAIJANI_LAYOUT_LETTER_NUM_T9,     Azerbaijani::VK_Layout_T9},     //IQQI_IME_Azerbaijani
      {BELARUSIAN_LAYOUT_LETTER_NUM_T9,      Belarusian::VK_Layout_T9},      //IQQI_IME_Belarusian
      {BOSNIAN_LAYOUT_LETTER_NUM_T9,         Bosnian::VK_Layout_T9},         //IQQI_IME_Bosnian
      {CHINESE_HK_LAYOUT_LETTER_NUM_T9,      Chinese_HK::VK_Layout_T9},      //IQQI_IME_Chinese_HK
      {CHINESE_TW_LAYOUT_LETTER_NUM_T9,      Chinese_TW::VK_Layout_T9},      //IQQI_IME_Chinese_TW
      {DANISH_LAYOUT_LETTER_NUM_T9,          Danish::VK_Layout_T9},          //IQQI_IME_Danish
      {ESTONIAN_LAYOUT_LETTER_NUM_T9,        Estonian::VK_Layout_T9},        //IQQI_IME_Estonian
      {GEORGIAN_LAYOUT_LETTER_NUM_T9,        Georgian::VK_Layout_T9},        //IQQI_IME_Georgian
      {HEBREW_LAYOUT_LETTER_NUM_T9,          Hebrew::VK_Layout_T9},          //IQQI_IME_Hebrew
      {ICELANDIC_LAYOUT_LETTER_NUM_T9,       Icelandic::VK_Layout_T9},       //IQQI_IME_Icelandic
      {LAO_LAYOUT_LETTER_NUM_T9,             Lao::VK_Layout_T9},             //IQQI_IME_Lao
      {LATVIAN_LAYOUT_LETTER_NUM_T9,         Latvian::VK_Layout_T9},         //IQQI_IME_Latvian
      {LITHUANIAN_LAYOUT_LETTER_NUM_T9,      Lithuanian::VK_Layout_T9},      //IQQI_IME_Lithuanian
      {NORWEGIAN_LAYOUT_LETTER_NUM_T9,       Norwegian::VK_Layout_T9},       //IQQI_IME_Norwegian
      {UZBEK_CYRILLIC_LAYOUT_LETTER_NUM_T9,  Uzbek_Cyrillic::VK_Layout_T9},  //IQQI_IME_Uzbek_Cyrillic
      {BASQUE_LAYOUT_LETTER_NUM_T9,          Basque::VK_Layout_T9},          //IQQI_IME_Basque
      {GALICIAN_LAYOUT_LETTER_NUM_T9,        Galician::VK_Layout_T9},        //IQQI_IME_Galician
      {0,                                    0                        },     //IQQI_IME_Malagasy
      {0,                                    0                        },     //IQQI_IME_Yiddish
      #if !defined(__NO_KOREAN)
      {KOREAN_LAYOUT_LETTER_NUM_T9,          Korean::VK_Layout_T9},          //IQQI_IME_Korean
      #endif
      {CATALAN_LAYOUT_LETTER_NUM_T9,         Galician::VK_Layout_T9},        //IQQI_IME_Catalan
      {0,                                    0                        },     //IQQI_IME_Japanese_KN
      {0,                                    0                        },     //IQQI_IME_Japanese_RM
      {CHANGJEI_LAYOUT_LETTER_NUM_T9,        Changjie::VK_Layout_T9},        //IQQI_IME_Changjie
      {0,                                    0                        },     //IQQI_IME_Quick_CJ
      {FARSI_LAYOUT_LETTER_NUM_T9,           Farsi::VK_Layout_T9},           //IQQI_IME_Farsi
      {MAITHILI_LAYOUT_LETTER_NUM_T9,        Maithili::VK_Layout_T9},        //IQQI_IME_Maithil
      {BURMESE_Z_LAYOUT_LETTER_NUM_T9,       Burmese_Z::VK_Layout_T9},       //IQQI_IME_Burmese
      {BURMESE_U_LAYOUT_LETTER_NUM_T9,       Burmese_U::VK_Layout_T9},       //IQQI_IME_Burmese
      {TIBETAN_LAYOUT_LETTER_NUM_T9,         Tibetan::VK_Layout_T9},         //IQQI_IME_Tibetan
      {0,                                    0                        },     //IQQI_IME_Uyghur
      {JAWA_LAYOUT_LETTER_NUM_T9,            Jawa::VK_Layout_T9          },  //IQQI_IME_Jawa
      {MAORI_LAYOUT_LETTER_NUM_T9,           Maori::VK_Layout_T9         },  //IQQI_IME_Maori
      {MONGOLIAN_LAYOUT_LETTER_NUM_T9,       Mongolian::VK_Layout_T9},       //IQQI_IME_Mongolian
      {0,                                    0                        },     //IQQI_IME_Amharic
     };

     //------------------------------------------------------------------------
     struct _Qwerty
     {
          int                     total;
          _VirtualKeybdMapping*   table;
     } Qwerty[] =
     {
      {ENGLISH_US_LAYOUT_LETTER_NUM_QWERTY,        English_US::VK_Layout_Qwerty},       //IQQI_IME_English_US
      {0,                                          0                          },        //IQQI_IME_French_CA
      {PORTUGUESE_BR_LAYOUT_LETTER_NUM_QWERTY,     Portuguese_BR::VK_Layout_Qwerty},    //IQQI_IME_Portuguese_BR
      {SPANISH_US_LAYOUT_LETTER_NUM_QWERTY,        Spanish_US::VK_Layout_Qwerty},       //IQQI_IME_Spanish_US
      {ASSAMESE_LAYOUT_LETTER_NUM_QWERTY,          Assamese::VK_Layout_Qwerty},         //IQQI_IME_Assamese
      {BENGALI_IN_LAYOUT_LETTER_NUM_QWERTY,        Bengali_IN::VK_Layout_Qwerty},       //IQQI_IME_Bengali_IN
      {0,                                          0                          },        //IQQI_IME_Bodo
      {0,                                          0                          },        //IQQI_IME_Dogri
      {GUJARATI_LAYOUT_LETTER_NUM_QWERTY,          Gujarati::VK_Layout_Qwerty},         //IQQI_IME_Gujarati
      {HINDI_LAYOUT_LETTER_NUM_QWERTY,             Hindi::VK_Layout_Qwerty},            //IQQI_IME_Hindi
      {KANNADA_LAYOUT_LETTER_NUM_QWERTY,           Kannada::VK_Layout_Qwerty},          //IQQI_IME_Kannada
      {0,                                          0                          },        //IQQI_IME_Kashmiri
      {0,                                          0                          },        //IQQI_IME_Konkani
      {MALAYALAM_LAYOUT_LETTER_NUM_QWERTY,         Malayalam::VK_Layout_Qwerty},        //IQQI_IME_Malayalam
      {0,                                          0                          },        //IQQI_IME_Manipuri
      {MARATHI_LAYOUT_LETTER_NUM_QWERTY,           Marathi::VK_Layout_Qwerty},          //IQQI_IME_Marathi
      {NEPALI_LAYOUT_LETTER_NUM_QWERTY,            Nepali::VK_Layout_Qwerty},           //IQQI_IME_Nepali
      {ORIYA_LAYOUT_LETTER_NUM_QWERTY,             Oriya::VK_Layout_Qwerty},            //IQQI_IME_Oriya
      {PUNJABI_LAYOUT_LETTER_NUM_QWERTY,           Punjabi::VK_Layout_Qwerty},          //IQQI_IME_Punjabi
      {0,                                          0                          },        //IQQI_IME_Sanskrit
      {0,                                          0                          },        //IQQI_IME_Santhali
      {0,                                          0                          },        //IQQI_IME_Sindhi
      {TAMIL_LAYOUT_LETTER_NUM_QWERTY,             Tamil::VK_Layout_Qwerty},            //IQQI_IME_Tamil
      {TELUGU_LAYOUT_LETTER_NUM_QWERTY,            Telugu::VK_Layout_Qwerty},           //IQQI_IME_Telugu
      {URDU_LAYOUT_LETTER_NUM_QWERTY,              Urdu::VK_Layout_Qwerty},             //IQQI_IME_Urdu
      {0,                                          0                          },        //IQQI_IME_Afrikaans
      {ARABIC_LAYOUT_LETTER_NUM_QWERTY,            Arabic::VK_Layout_Qwerty},           //IQQI_IME_Arabic
      {CHINESE_CN_LAYOUT_LETTER_NUM_QWERTY,        Chinese_CN::VK_Layout_Qwerty},       //IQQI_IME_Chinese_CN
      {DUTCH_LAYOUT_LETTER_NUM_QWERTY,             Dutch::VK_Layout_Qwerty},            //IQQI_IME_Dutch
      {ENGLISH_GB_LAYOUT_LETTER_NUM_QWERTY,        English_GB::VK_Layout_Qwerty},       //IQQI_IME_English_GB
      {FRENCH_FR_LAYOUT_LETTER_NUM_QWERTY,         French_FR::VK_Layout_Qwerty},        //IQQI_IME_French_FR
      {GERMAN_LAYOUT_LETTER_NUM_QWERTY,            German::VK_Layout_Qwerty},           //IQQI_IME_German
      {HUNGARIAN_LAYOUT_LETTER_NUM_QWERTY,         Hungarian::VK_Layout_Qwerty},        //IQQI_IME_Hungarian
      {INDONESIAN_LAYOUT_LETTER_NUM_QWERTY,        Indonesian::VK_Layout_Qwerty},       //IQQI_IME_Indonesian
      {ITALIAN_LAYOUT_LETTER_NUM_QWERTY,           Italian::VK_Layout_Qwerty},          //IQQI_IME_Italian
      {MALAY_LAYOUT_LETTER_NUM_QWERTY,             Malay::VK_Layout_Qwerty},            //IQQI_IME_Malay
      {PERSIAN_LAYOUT_LETTER_NUM_QWERTY,           Persian::VK_Layout_Qwerty},          //IQQI_IME_Persian
      {POLISH_LAYOUT_LETTER_NUM_QWERTY,            Polish::VK_Layout_Qwerty},           //IQQI_IME_Polish
      {PORTUGUESE_PT_LAYOUT_LETTER_NUM_QWERTY,     Portuguese_PT::VK_Layout_Qwerty},    //IQQI_IME_Portuguese_PT
      {ROMANIAN_LAYOUT_LETTER_NUM_QWERTY,          Romanian::VK_Layout_Qwerty},         //IQQI_IME_Romanian
      {RUSSIAN_LAYOUT_LETTER_NUM_QWERTY,           Russian::VK_Layout_Qwerty},          //IQQI_IME_Russian
      {SPANISH_ES_LAYOUT_LETTER_NUM_QWERTY,        Spanish_ES::VK_Layout_Qwerty},       //IQQI_IME_Spanish_ES
      {SWAHILI_LAYOUT_LETTER_NUM_QWERTY,           Swahili::VK_Layout_Qwerty},          //IQQI_IME_Swahili
      {THAI_LAYOUT_LETTER_NUM_QWERTY,              Thai::VK_Layout_Qwerty},             //IQQI_IME_Thai
      {TURKISH_LAYOUT_LETTER_NUM_QWERTY,           Turkish::VK_Layout_Qwerty},          //IQQI_IME_Turkish
      {VIETNAMESE_LAYOUT_LETTER_NUM_QWERTY,        Vietnamese::VK_Layout_Qwerty},       //IQQI_IME_Vietnamese
      {0,                                          0                          },     //IQQI_IME_Zulu
      {BENGALI_BD_LAYOUT_LETTER_NUM_QWERTY,        Bengali_BD::VK_Layout_Qwerty},       //IQQI_IME_Bengali_BD
      {BULGARIAN_LAYOUT_LETTER_NUM_QWERTY,         Bulgarian::VK_Layout_Qwerty},        //IQQI_IME_Bulgarian
      {CROATIAN_LAYOUT_LETTER_NUM_QWERTY,          Croatian::VK_Layout_Qwerty},         //IQQI_IME_Croatian
      {CZECH_LAYOUT_LETTER_NUM_QWERTY,             Czech::VK_Layout_Qwerty},            //IQQI_IME_Czech
      {FINNISH_LAYOUT_LETTER_NUM_QWERTY,           Finnish::VK_Layout_Qwerty},          //IQQI_IME_Finnish
      {GREEK_LAYOUT_LETTER_NUM_QWERTY,             Greek::VK_Layout_Qwerty},            //IQQI_IME_Greek
      {KAZAKH_LAYOUT_LETTER_NUM_QWERTY,            Kazakh::VK_Layout_Qwerty},           //IQQI_IME_Kazakh
      {KHMER_LAYOUT_LETTER_NUM_QWERTY,             Khmer::VK_Layout_Qwerty},            //IQQI_IME_Khmer
      {MACEDONIAN_LAYOUT_LETTER_NUM_QWERTY,        Macedonian::VK_Layout_Qwerty},       //IQQI_IME_Macedonian
      {SERBIAN_LAYOUT_LETTER_NUM_QWERTY,           Serbian::VK_Layout_Qwerty},          //IQQI_IME_Serbian
      {SINHALA_LAYOUT_LETTER_NUM_QWERTY,           Sinhala::VK_Layout_Qwerty},          //IQQI_IME_Sinhala
      {SLOVAK_LAYOUT_LETTER_NUM_QWERTY,            Slovak::VK_Layout_Qwerty},           //IQQI_IME_Slovak
      {SLOVENIAN_LAYOUT_LETTER_NUM_QWERTY,         Slovenian::VK_Layout_Qwerty},        //IQQI_IME_Slovenian
      {SWEDISH_LAYOUT_LETTER_NUM_QWERTY,           Swedish::VK_Layout_Qwerty},          //IQQI_IME_Swedish
      {TAGALOG_LAYOUT_LETTER_NUM_QWERTY,           Tagalog::VK_Layout_Qwerty},          //IQQI_IME_Tagalog
      {UKRAINIAN_LAYOUT_LETTER_NUM_QWERTY,         Ukrainian::VK_Layout_Qwerty},        //IQQI_IME_Ukrainian
      {0,                                          0                          },     //IQQI_IME_Xhosa
      {0,                                          0                          },     //IQQI_IME_Albanian
      {0,                                          0                          },     //IQQI_IME_Armenian
      {AZERBAIJANI_LAYOUT_LETTER_NUM_QWERTY,       Azerbaijani::VK_Layout_Qwerty},      //IQQI_IME_Azerbaijani
      {BELARUSIAN_LAYOUT_LETTER_NUM_QWERTY,        Belarusian::VK_Layout_Qwerty},       //IQQI_IME_Belarusian
      {BOSNIAN_LAYOUT_LETTER_NUM_QWERTY,           Bosnian::VK_Layout_Qwerty},          //IQQI_IME_Bosnian
      {0,                                          0                          },     //IQQI_IME_Chinese_HK
      {CHINESE_TW_LAYOUT_LETTER_NUM_QWERTY,        Chinese_TW::VK_Layout_Qwerty},       //IQQI_IME_Chinese_TW
      {DANISH_LAYOUT_LETTER_NUM_QWERTY,            Danish::VK_Layout_Qwerty},           //IQQI_IME_Danish
      {ESTONIAN_LAYOUT_LETTER_NUM_QWERTY,          Estonian::VK_Layout_Qwerty},         //IQQI_IME_Estonian
      {GEORGIAN_LAYOUT_LETTER_NUM_QWERTY,          Georgian::VK_Layout_Qwerty},         //IQQI_IME_Georgian
      {HEBREW_LAYOUT_LETTER_NUM_QWERTY,            Hebrew::VK_Layout_Qwerty},           //IQQI_IME_Hebrew
      {0,                                          0                          },     //IQQI_IME_Icelandic
      {LAO_LAYOUT_LETTER_NUM_QWERTY,               Lao::VK_Layout_Qwerty},              //IQQI_IME_Lao
      {LATVIAN_LAYOUT_LETTER_NUM_QWERTY,           Latvian::VK_Layout_Qwerty},          //IQQI_IME_Latvian
      {LITHUANIAN_LAYOUT_LETTER_NUM_QWERTY,        Lithuanian::VK_Layout_Qwerty},       //IQQI_IME_Lithuanian
      {NORWEGIAN_LAYOUT_LETTER_NUM_QWERTY,         Norwegian::VK_Layout_Qwerty},        //IQQI_IME_Norwegian
      {UZBEK_CYRILLIC_LAYOUT_LETTER_NUM_QWERTY,    Uzbek_Cyrillic::VK_Layout_Qwerty},   //IQQI_IME_Uzbek_Cyrillic
      {BASQUE_LAYOUT_LETTER_NUM_QWERTY,            Basque::VK_Layout_Qwerty},           //IQQI_IME_Basque
      {GALICIAN_LAYOUT_LETTER_NUM_QWERTY,          Galician::VK_Layout_Qwerty},         //IQQI_IME_Galician
      {0,                                          0                          },     //IQQI_IME_Malagasy
      {0,                                          0                          },     //IQQI_IME_Yiddish
      #if !defined(__NO_KOREAN)                                                         
      {KOREAN_LAYOUT_LETTER_NUM_QWERTY,            Korean::VK_Layout_Qwerty},           //IQQI_IME_Korean
      #endif                                                                            
      {CATALAN_LAYOUT_LETTER_NUM_QWERTY,           Catalan::VK_Layout_Qwerty},          //IQQI_IME_Catalan
      {0,                                          0                          },     //IQQI_IME_Japanese_KN
      {0,                                          0                          },     //IQQI_IME_Japanese_RM
      {CHANGJEI_LAYOUT_LETTER_NUM_QWERTY,          Changjie::VK_Layout_Qwerty},         //IQQI_IME_Changjie
      {0,                                          0                          },     //IQQI_IME_Quick_CJ
      {FARSI_LAYOUT_LETTER_NUM_QWERTY,             Farsi::VK_Layout_Qwerty},            //IQQI_IME_Farsi
      {MAITHILI_LAYOUT_LETTER_NUM_QWERTY,          Maithili::VK_Layout_Qwerty},         //IQQI_IME_Maithil
      {BURMESE_Z_LAYOUT_LETTER_NUM_QWERTY,         Burmese_Z::VK_Layout_Qwerty},        //IQQI_IME_Burmese
      {BURMESE_U_LAYOUT_LETTER_NUM_QWERTY,         Burmese_U::VK_Layout_Qwerty},        //IQQI_IME_Burmese
      {TIBETAN_LAYOUT_LETTER_NUM_QWERTY,           Tibetan::VK_Layout_Qwerty},          //IQQI_IME_Tibetan
      {UYGHUR_LAYOUT_LETTER_NUM_QWERTY,            Uyghur::VK_Layout_Qwerty},           //IQQI_IME_Uyghur
      {JAWA_LAYOUT_LETTER_NUM_QWERTY,              Jawa::VK_Layout_Qwerty},             //IQQI_IME_Jawa
      {MAORI_LAYOUT_LETTER_NUM_QWERTY,             Maori::VK_Layout_Qwerty},            //IQQI_IME_Maori
      {MONGOLIAN_LAYOUT_LETTER_NUM_QWERTY,         Mongolian::VK_Layout_Qwerty},        //IQQI_IME_Mongolian
      {AMHARIC_LAYOUT_LETTER_NUM_QWERTY,           Amharic::VK_Layout_Qwerty},          //IQQI_IME_Amharic
    };

     //------------------------------------------------------------------------
     if ( imeID >=IQQI_IME_English_US && imeID <=IQQI_IME_Amharic )
     {
          int
          mapID =imeID -1;
          
          if ( mode ==for_T9 )
          {
               #if defined(__DEBUG_TBL_MSG)
               if ( MultiType[mapID].table ==0 )
                    KIKA_LOGD("Kika-ENGINE, T9 MapVkTableEx not found! mapID =%d, is null\r\n", mapID);
               else
                    KIKA_LOGD("Kika-ENGINE, T9 MapVkTableEx found! mapID =%d, OK\r\n", mapID);
               #endif

               #if defined(__DEBUG_MSG)
               KIKA_LOGD("MapVkTable T9\r\n");
               #endif

               total  =T9[mapID].total;
               *table =T9[mapID].table;
          }
          else
          {
               #if defined(__DEBUG_TBL_MSG)
               if ( MultiType[mapID].table ==0 )
                    KIKA_LOGD("Kika-ENGINE, Qwerty MapVkTableEx not found! mapID =%d, is null\r\n", mapID);
               else
                    KIKA_LOGD("Kika-ENGINE, Qwerty MapVkTableEx found! mapID =%d, OK\r\n", mapID);
               #endif

               #if defined(__DEBUG_MSG)
               KIKA_LOGD("MapVkTable Qwerty\r\n");
               #endif

               total  =Qwerty[mapID].total;
               *table =Qwerty[mapID].table;
          }
     }
     else
     {
          total  =0;
          *table =nullptr;
     }

     #if defined(__DEBUG_MSG) && defined(__TEST)
     printf("maptable_MapVkTable::table =%x, _IME =%d\r\n", *table, imeID);
     #endif

     mBTotal =total;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void maptable::MapVkTable(int imeID, unsigned int &total, VirtualKeybdMapping **table)
{
     MapVkTableEx(imeID, total, table, mKbMode);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void maptable::MapMultiType(int imeID, unsigned int &total, MultiTypeConfig **table)
{
     //------------------------------------------------------------------------
     struct _MultiType
     {
          int                total;
          MultiTypeConfig*   table;
     } MultiType[] =
     {
      {ENGLISH_US_MULTI_TYPE_SET_NUM,      English_US::MultiType},      //IQQI_IME_English_US
      {FRENCH_CA_MULTI_TYPE_SET_NUM,       French_CA::MultiType},       //IQQI_IME_French_CA
      {PORTUGUESE_BR_MULTI_TYPE_SET_NUM,   Portuguese_BR::MultiType},   //IQQI_IME_Portuguese_BR
      {SPANISH_US_MULTI_TYPE_SET_NUM,      Spanish_US::MultiType},      //IQQI_IME_Spanish_US
      {ASSAMESE_MULTI_TYPE_SET_NUM,        Assamese::MultiType},        //IQQI_IME_Assamese
      {BENGALI_IN_MULTI_TYPE_SET_NUM,      Bengali_IN::MultiType},      //IQQI_IME_Bengali_IN
      {0,                                  0                     },     //IQQI_IME_Bodo
      {0,                                  0                     },     //IQQI_IME_Dogri
      {GUJARATI_MULTI_TYPE_SET_NUM,        Gujarati::MultiType},        //IQQI_IME_Gujarati
      {HINDI_MULTI_TYPE_SET_NUM,           Hindi::MultiType},           //IQQI_IME_Hindi
      {KANNADA_MULTI_TYPE_SET_NUM,         Kannada::MultiType},         //IQQI_IME_Kannada
      {0,                                  0                     },     //IQQI_IME_Kashmiri
      {0,                                  0                     },     //IQQI_IME_Konkani
      {MALAYALAM_MULTI_TYPE_SET_NUM,       Malayalam::MultiType},       //IQQI_IME_Malayalam
      {0,                                  0                     },     //IQQI_IME_Manipuri
      {MARATHI_MULTI_TYPE_SET_NUM,         Marathi::MultiType},         //IQQI_IME_Marathi
      {NEPALI_MULTI_TYPE_SET_NUM,          Nepali::MultiType},          //IQQI_IME_Nepali
      {ORIYA_MULTI_TYPE_SET_NUM,           Oriya::MultiType},           //IQQI_IME_Oriya
      {PUNJABI_MULTI_TYPE_SET_NUM,         Punjabi::MultiType},         //IQQI_IME_Punjabi
      {0,                                  0                     },     //IQQI_IME_Sanskrit
      {0,                                  0                     },     //IQQI_IME_Santhali
      {0,                                  0                     },     //IQQI_IME_Sindhi
      {TAMIL_MULTI_TYPE_SET_NUM,           Tamil::MultiType},           //IQQI_IME_Tamil
      {TELUGU_MULTI_TYPE_SET_NUM,          Telugu::MultiType},          //IQQI_IME_Telugu
      {URDU_MULTI_TYPE_SET_NUM,            Urdu::MultiType},            //IQQI_IME_Urdu
      {AFRIKAANS_MULTI_TYPE_SET_NUM,       Afrikaans::MultiType},       //IQQI_IME_Afrikaans
      {ARABIC_MULTI_TYPE_SET_NUM,          Arabic::MultiType},          //IQQI_IME_Arabic
      {CHINESE_CN_MULTI_TYPE_SET_NUM,      Chinese_CN::MultiType},      //IQQI_IME_Chinese_CN
      {DUTCH_MULTI_TYPE_SET_NUM,           Dutch::MultiType},           //IQQI_IME_Dutch
      {ENGLISH_GB_MULTI_TYPE_SET_NUM,      English_GB::MultiType},      //IQQI_IME_English_GB
      {FRENCH_FR_MULTI_TYPE_SET_NUM,       French_FR::MultiType},       //IQQI_IME_French_FR
      {GERMAN_MULTI_TYPE_SET_NUM,          German::MultiType},          //IQQI_IME_German
      {HUNGARIAN_MULTI_TYPE_SET_NUM,       Hungarian::MultiType},       //IQQI_IME_Hungarian
      {INDONESIAN_MULTI_TYPE_SET_NUM,      Indonesian::MultiType},      //IQQI_IME_Indonesian
      {ITALIAN_MULTI_TYPE_SET_NUM,         Italian::MultiType},         //IQQI_IME_Italian
      {MALAY_MULTI_TYPE_SET_NUM,           Malay::MultiType},           //IQQI_IME_Malay
      {PERSIAN_MULTI_TYPE_SET_NUM,         Persian::MultiType},         //IQQI_IME_Persian
      {POLISH_MULTI_TYPE_SET_NUM,          Polish::MultiType},          //IQQI_IME_Polish
      {PORTUGUESE_PT_MULTI_TYPE_SET_NUM,   Portuguese_PT::MultiType},   //IQQI_IME_Portuguese_PT
      {ROMANIAN_MULTI_TYPE_SET_NUM,        Romanian::MultiType},        //IQQI_IME_Romanian
      {RUSSIAN_MULTI_TYPE_SET_NUM,         Russian::MultiType},         //IQQI_IME_Russian
      {SPANISH_ES_MULTI_TYPE_SET_NUM,      Spanish_ES::MultiType},      //IQQI_IME_Spanish_ES
      {SWAHILI_MULTI_TYPE_SET_NUM,         Swahili::MultiType},         //IQQI_IME_Swahili
      {THAI_MULTI_TYPE_SET_NUM,            Thai::MultiType},            //IQQI_IME_Thai
      {TURKISH_MULTI_TYPE_SET_NUM,         Turkish::MultiType},         //IQQI_IME_Turkish
      {VIETNAMESE_MULTI_TYPE_SET_NUM,      Vietnamese::MultiType},      //IQQI_IME_Vietnamese
      {ZULU_MULTI_TYPE_SET_NUM,            Zulu::MultiType},            //IQQI_IME_Zulu
      {BENGALI_BD_MULTI_TYPE_SET_NUM,      Bengali_BD::MultiType},      //IQQI_IME_Bengali_BD
      {BULGARIAN_MULTI_TYPE_SET_NUM,       Bulgarian::MultiType},       //IQQI_IME_Bulgarian
      {CROATIAN_MULTI_TYPE_SET_NUM,        Croatian::MultiType},        //IQQI_IME_Croatian
      {CZECH_MULTI_TYPE_SET_NUM,           Czech::MultiType},           //IQQI_IME_Czech
      {FINNISH_MULTI_TYPE_SET_NUM,         Finnish::MultiType},         //IQQI_IME_Finnish
      {GREEK_MULTI_TYPE_SET_NUM,           Greek::MultiType},           //IQQI_IME_Greek
      {KAZAKH_MULTI_TYPE_SET_NUM,          Kazakh::MultiType},          //IQQI_IME_Kazakh
      {KHMER_MULTI_TYPE_SET_NUM,           Khmer::MultiType},           //IQQI_IME_Khmer
      {MACEDONIAN_MULTI_TYPE_SET_NUM,      Macedonian::MultiType},      //IQQI_IME_Macedonian
      {SERBIAN_MULTI_TYPE_SET_NUM,         Serbian::MultiType},         //IQQI_IME_Serbian
      {SINHALA_MULTI_TYPE_SET_NUM,         Sinhala::MultiType},         //IQQI_IME_Sinhala
      {SLOVAK_MULTI_TYPE_SET_NUM,          Slovak::MultiType},          //IQQI_IME_Slovak
      {SLOVENIAN_MULTI_TYPE_SET_NUM,       Slovenian::MultiType},       //IQQI_IME_Slovenian
      {SWEDISH_MULTI_TYPE_SET_NUM,         Swedish::MultiType},         //IQQI_IME_Swedish
      {TAGALOG_MULTI_TYPE_SET_NUM,         Tagalog::MultiType},         //IQQI_IME_Tagalog
      {UKRAINIAN_MULTI_TYPE_SET_NUM,       Ukrainian::MultiType},       //IQQI_IME_Ukrainian
      {XHOSA_MULTI_TYPE_SET_NUM,           Xhosa::MultiType},           //IQQI_IME_Xhosa
      {ALBANIAN_MULTI_TYPE_SET_NUM,        Albanian::MultiType},        //IQQI_IME_Albanian
      {ARMENIAN_MULTI_TYPE_SET_NUM,        Armenian::MultiType},        //IQQI_IME_Armenian
      {AZERBAIJANI_MULTI_TYPE_SET_NUM,     Azerbaijani::MultiType},     //IQQI_IME_Azerbaijani
      {BELARUSIAN_MULTI_TYPE_SET_NUM,      Belarusian::MultiType},      //IQQI_IME_Belarusian
      {BOSNIAN_MULTI_TYPE_SET_NUM,         Bosnian::MultiType},         //IQQI_IME_Bosnian
      {CHINESE_HK_MULTI_TYPE_SET_NUM,      Chinese_HK::MultiType},      //IQQI_IME_Chinese_HK
      {CHINESE_TW_MULTI_TYPE_SET_NUM,      Chinese_TW::MultiType},      //IQQI_IME_Chinese_TW
      {DANISH_MULTI_TYPE_SET_NUM,          Danish::MultiType},          //IQQI_IME_Danish
      {ESTONIAN_MULTI_TYPE_SET_NUM,        Estonian::MultiType},        //IQQI_IME_Estonian
      {GEORGIAN_MULTI_TYPE_SET_NUM,        Georgian::MultiType},        //IQQI_IME_Georgian
      {HEBREW_MULTI_TYPE_SET_NUM,          Hebrew::MultiType},          //IQQI_IME_Hebrew
      {ICELANDIC_MULTI_TYPE_SET_NUM,       Icelandic::MultiType},       //IQQI_IME_Icelandic
      {LAO_MULTI_TYPE_SET_NUM,             Lao::MultiType},             //IQQI_IME_Lao
      {LATVIAN_MULTI_TYPE_SET_NUM,         Latvian::MultiType},         //IQQI_IME_Latvian
      {LITHUANIAN_MULTI_TYPE_SET_NUM,      Lithuanian::MultiType},      //IQQI_IME_Lithuanian
      {NORWEGIAN_MULTI_TYPE_SET_NUM,       Norwegian::MultiType},       //IQQI_IME_Norwegian
      {UZBEK_CYRILLIC_MULTI_TYPE_SET_NUM,  Uzbek_Cyrillic::MultiType},  //IQQI_IME_Uzbek_Cyrillic
      {BASQUE_MULTI_TYPE_SET_NUM,          Basque::MultiType},          //IQQI_IME_Basque
      {GALICIAN_MULTI_TYPE_SET_NUM,        Galician::MultiType},        //IQQI_IME_Galician
      {0,                                  0                     },     //IQQI_IME_Malagasy
      {0,                                  0                     },     //IQQI_IME_Yiddish
      #if !defined(__NO_KOREAN)
      {KOREAN_MULTI_TYPE_SET_NUM,          Korean::MultiType},          //IQQI_IME_Korean
      #endif
      {CATALAN_MULTI_TYPE_SET_NUM,         Catalan::MultiType},         //IQQI_IME_Catalan
      {0,                                  0                     },     //IQQI_IME_Japanese_KN
      {0,                                  0                     },     //IQQI_IME_Japanese_RM
      {CHANGJEI_MULTI_TYPE_SET_NUM,        Changjie::MultiType},        //IQQI_IME_Changjie
      {0,                                  0                     },     //IQQI_IME_Quick_CJ
      {FARSI_MULTI_TYPE_SET_NUM,           Farsi::MultiType},           //IQQI_IME_Farsi
      {MAITHILI_MULTI_TYPE_SET_NUM,        Maithili::MultiType},        //IQQI_IME_Maithil
      {BURMESE_Z_MULTI_TYPE_SET_NUM,       Burmese_Z::MultiType},       //IQQI_IME_Burmese
      {BURMESE_U_MULTI_TYPE_SET_NUM,       Burmese_U::MultiType},       //IQQI_IME_Burmese
      {TIBETAN_MULTI_TYPE_SET_NUM,         Tibetan::MultiType},         //IQQI_IME_Tibetan
      {0,                                  0                     },     //IQQI_IME_Uyghur
      {JAWA_MULTI_TYPE_SET_NUM,            Jawa::MultiType},            //IQQI_IME_Jawa
      {MAORI_MULTI_TYPE_SET_NUM,           Maori::MultiType},           //IQQI_IME_Maori
      {MONGOLIAN_MULTI_TYPE_SET_NUM,       Mongolian::MultiType},       //IQQI_IME_Mongolian
      {0,                                  0                     },     //IQQI_IME_Amharic
    };

     if ( imeID >=IQQI_IME_English_US && imeID <=IQQI_IME_Amharic )
     {
          int
          mapID =imeID -1;

          #if defined(__DEBUG_TBL_MSG)
          if ( MultiType[mapID].table ==0 )
               KIKA_LOGD("Kika-ENGINE, MapMultiType not found! mapID =%d, is null\r\n", mapID);
          else
               KIKA_LOGD("Kika-ENGINE, MapMultiType found! mapID =%d, OK\r\n", mapID);
          #endif

          total  =MultiType[mapID].total;
          *table =MultiType[mapID].table;
     }
     else
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("Kika-ENGINE, MapMultiType not found! \r\n");
          #endif

          total  =0;
          *table =nullptr;
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
key* maptable::GetLayout(int _IME)
{
     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTable(_IME, total, &table);

     if ( table ==nullptr )
          return nullptr;

     for ( unsigned int i = 0; i <total; i++ )
     {
           mBKeys[i].x      =table[i].x;
           mBKeys[i].y      =table[i].y;
           mBKeys[i].width  =table[i].width;
           mBKeys[i].height =table[i].height;
           mBKeys[i].code   =table[i].kcode;
     }

     return mBKeys;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::KeyCode2BtnXY(int _IME, unsigned int kcode, unsigned int &x, unsigned int &y)
{
     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTable(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("KeyCode2BtnXY::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("KeyCode2BtnXY::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          return false;
     }

     for ( unsigned int i = 0; i <total; i++ )
     {
           if ( table[i].kcode ==kcode )
           {
                x =table[i].x;
                y =table[i].y;
                return true;
           }
     }
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
bool maptable::keyCode2Numb(int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize)
{
     ncode[0] =0;

     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTable(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("keyCode2Numb::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("keyCode2Numb::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          return false;
     }

     unsigned int cn =wcslen(kcode);

     for ( unsigned int c = 0; c <cn && c <buffSize; c++ )
     {
           for ( unsigned int i = 0; i <total; i++ )
           {
                 if ( table[i].kcode ==kcode[c] )
                 {
                      wchar_t
                      ws[2] ={0, 0};
                      ws[0] =table[i].kcode_L;
                      wcscat(ncode, ws);
                      break;
                 }
           }
     }

     return wcslen(ncode) ==cn;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : return capitalize character
//
//-----------------------------------------------------------------------------
wchar_t maptable::keyCode2Shift(int _IME, const wchar_t kcode)
{
     if ( kcode ==0 )
     {
          return kcode;
     }

     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTable(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("keyCode2Shift::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("keyCode2Shift::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          return false;
     }

     for ( unsigned int i = 0; i <total; i++ )
     {
           if ( table[i].kcode ==kcode )
           {
                return table[i].kcode_L;
                break;
           }
     }
     return kcode;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::keyCode2NumbEx(int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize)
{
     //ncode[0] =0;

     if ( wcscmp(kcode, L"[[begin]]") ==0 )
     {
          #if defined(__DEBUG_KEYCODE_MSG)
          KIKA_LOGD("Kika-ENGINE, keyCode2NumbEx, is [[begin]]\r\n");
          #endif
          return true;
     }
     else if ( wcscmp(kcode, L"[[end]]") ==0 )
     {
          #if defined(__DEBUG_KEYCODE_MSG)
          KIKA_LOGD("Kika-ENGINE, keyCode2NumbEx, is [[end]]\r\n");
          #endif
          return true;
     }

     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTable(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("keyCode2NumbEx::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("keyCode2NumbEx::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          return false;
     }

     unsigned int cn =wcslen(kcode);
     unsigned int c_cut =0;

     for ( unsigned int c = 0; c <cn && c <buffSize; c++ )
     {
           //------------------------------------------------------------------
           // don't 另外處理
           //------------------------------------------------------------------
           #if defined(__DEBUG_KEYCODE_MSG)
           KIKA_LOGD("Kika-ENGINE, keyCode2NumbEx, c =%c\r\n", kcode[c]);
           #endif

           switch ( kcode[c] )
           {
                    case KEY_EXTRA_ALLOW_CH_QUOTATION:
                    //case KEY_EXTRA_ALLOW_CH_MINUS:
                    #if !defined(__SONIM)
                    case KEY_EXTRA_ALLOW_CH_0:
                    case KEY_EXTRA_ALLOW_CH_2:
                    #endif
                         c_cut++;
                         #if defined(__DEBUG_KEYCODE_MSG)
                         KIKA_LOGD("Kika-ENGINE, keyCode2NumbEx, c =%c, skip 1\r\n", kcode[c]);
                         #endif
                         continue;
                    default:
                         break;
           }

           if ( ncode[c] !=0 )
           {
                #if defined(__DEBUG_KEYCODE_MSG)
                KIKA_LOGD("Kika-ENGINE, keyCode2NumbEx, c =%c, skip 2\r\n", kcode[c]);
                #endif
                continue;
           }
           for ( unsigned int i = 0; i <total; i++ )
           {
                 if ( table[i].kcode ==kcode[c] )
                 {
                      if ( ncode[c] ==0 )
                      {
                           ncode[c -c_cut] =table[i].kcode_L;
                      }
                      break;
                 }
           }
     }
     {
         #if defined(__DEBUG_MSG)
         string
         sVoca =WstringToString(ncode);
         KIKA_LOGD("Kika-ENGINE, keyCode2NumbEx, done, key =%s\r\n", sVoca.c_str());
         #endif
     }
     return wcslen(ncode) ==(cn -c_cut);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::Numb2keyCode(int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize)
{
     ncode[0] =0;

     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTableEx(_IME, total, &table, for_T9);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("Numb2keyCode::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("Numb2keyCode::curr table ==null,  _IME =%d\r\n", _IME);
          #endif
          return false;
     }

     ncode[0] =0;

     unsigned int
     cn =wcslen(kcode);

     for ( unsigned int c = 0; c <cn && c <buffSize; c++ )
     {
           for ( unsigned int i = 0; i <total; i++ )
           {
                 if ( table[i].kcode_L ==kcode[c] )
                 {
                      wchar_t
                      ws[2] ={0, 0};
                      ws[0] =table[i].kcode;
                      wcscat(ncode, ws);
                      break;
                 }
           }
     }
     return wcslen(ncode) ==cn;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::Numb2EngCode(int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize)
{
     ncode[0] =0;

     unsigned int
     cn =wcslen(kcode);

     for ( unsigned int c = 0; c <cn && c <buffSize; c++ )
     {
           wchar_t
           kCH[2] ={kcode[c], 0};

           if ( _IME ==IQQI_IME_Chinese_TW )
           {
                map<wstring, wstring>::iterator
                find =mZhStrokes.find(kCH);

                if ( find ==mZhStrokes.end() )
                     wcscat(ncode, kCH);
                else
                     wcscat(ncode, find->second.c_str());
           }
           else if ( _IME ==IQQI_IME_Changjie )
           {
                map<wstring, wstring>::iterator
                find =mCjStrokes.find(kCH);

                if ( find ==mCjStrokes.end() )
                     wcscat(ncode, kCH);
                else
                     wcscat(ncode, find->second.c_str());
           }
     }
     return wcslen(ncode) ==cn;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::EngCodeToZHKey(int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize)
{
     ncode[0] =0;

     unsigned int
     cn =wcslen(kcode);

     for ( unsigned int c = 0; c <cn && c <buffSize; c++ )
     {
           wchar_t
           kCH[2] ={kcode[c], 0};

           if ( _IME ==IQQI_IME_Chinese_TW )
           {
                map<wstring, wstring>::iterator
                find =mEngZhStrokes.find(kCH);

                if ( find ==mEngZhStrokes.end() )
                     wcscat(ncode, kCH);
                else
                     wcscat(ncode, find->second.c_str());
           }
     }
     return wcslen(ncode) ==cn;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::Numb2keyCodeAll(int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize)
{
     ncode[0] =0;

     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTableEx(_IME, total, &table, for_T9);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("Numb2keyCodeAll::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG_MAPTABLE_EX) && defined(__TEST)
          printf("Numb2keyCodeAll::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          return false;
     }

     ncode[0] =0;

     #if defined(__DEBUG_MSG_MAPTABLE_EX)
     KIKA_LOGD("maptabl_Numb2keyCode::_IME =%d, total =%d, buffSize =%d\r\n", _IME, total, buffSize);
     #endif
     #if defined(__DEBUG_MSG_MAPTABLE_EX) && defined(__TEST)
     printf("maptabl_Numb2keyCode::_IME =%d, total =%d, buffSize =%d\r\n", _IME, total, buffSize);
     #endif

     unsigned int cn =wcslen(kcode);

     for ( unsigned int c = 0; c <cn && c <buffSize; c++ )
     {
           for ( unsigned int i = 0; i <total; i++ )
           {
                 #if defined(__DEBUG_MSG_MAPTABLE_EX)
                 KIKA_LOGD("maptabl_Numb2keyCode::%d, table[i].kcode_L =%d(%x)(%c)\r\n", i, table[i].kcode_L, table[i].kcode_L, table[i].kcode_L);
                 #endif
                 #if defined(__DEBUG_MSG_MAPTABLE_EX) && defined(__TEST)
                 printf("maptabl_Numb2keyCode::%d, table[i].kcode_L =%d(%x)(%c)\r\n", i, table[i].kcode_L, table[i].kcode_L, table[i].kcode_L);
                 #endif

                 if ( table[i].kcode_L ==kcode[c] )
                 {
                      #if defined(__DEBUG_MSG)
                      KIKA_LOGD("maptabl_Numb2keyCode::table[i].kcode_L =%x\r\n", table[i].kcode_L);
                      #endif
                      #if defined(__DEBUG_MSG_MAPTABLE_EX) && defined(__TEST)
                      printf("maptabl_Numb2keyCode::table[i].kcode_L =%x\r\n", table[i].kcode_L);
                      #endif

                      wchar_t
                      ws[2] ={0, 0};
                      ws[0] =table[i].kcode;
                      wcscat(ncode, ws);
                 }
           }
     }

     #if defined(__DEBUG_MSG)
     KIKA_LOGD("maptabl_Numb2keyCode::wcslen(ncode) =%d\r\n", wcslen(ncode));
     #endif
     #if defined(__DEBUG_MSG_MAPTABLE_EX) && defined(__TEST)
     printf("maptabl_Numb2keyCode::wcslen(ncode) =%d\r\n", wcslen(ncode));
     #endif

     return wcslen(ncode) ==cn;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::KeyCode2HitXY(int _IME, unsigned int kcode, unsigned int &x, unsigned int &y)
{
     unsigned int        total;
     VirtualKeybdMapping *table =NULL;

     MapVkTable(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("KeyCode2HitXY::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("KeyCode2HitXY::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          return false;
     }

     for ( unsigned int i = 0; i <total; i++ )
     {
           if ( mKbMode ==for_T9 )
           {
                if ( table[i].kcode_L ==kcode )
                {
                     x =table[i].hit_x;
                     y =table[i].hit_y;

                     #if defined(__DEBUG_MSG) && defined(__TEST)
                     printf("  OK Hit =0x%x, x =%d, y =%d\r\n", kcode, x, y);
                     #endif
                     return true;
                }
           }
           else
           {
                if ( table[i].kcode ==kcode )
                {
                     x =table[i].x +(table[i].width  /2);
                     y =table[i].y +(table[i].height /2);

                     #if defined(__DEBUG_MSG) && defined(__TEST)
                     printf("  OK Hit =0x%x, x =%d, y =%d\r\n", kcode, x, y);
                     #endif
                     return true;
                }
           }
     }
     return false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
wchar_t* maptable::MultiType_Input(int _IME, int keyCode, int count)
{
     wchar_t seps[]  = L"\t";
     wchar_t *token  = NULL;
     wchar_t *buffer = NULL;

     unsigned int       total;
     MultiTypeConfig    *table =NULL;

     MapMultiType(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("MultiType_Input::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("MultiType_Input::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          return (wchar_t *)L"";
     }

     MultiType_KeyCount = count;

     if ( table[keyCode].code == keyCode )
     {
          wcscpy(MultiType_Def_Tmp, table[keyCode].def);

          #if defined(__LINUX_BASE)
          token = wcstok(MultiType_Def_Tmp, seps, &buffer);
          #else
          token = wcstok(MultiType_Def_Tmp, seps);
          #endif

          int
          index = MultiType_KeyCount % table[keyCode].num;

          if ( index != 1 )
          {
               if ( index == 0 )
               {
                    index = table[keyCode].num;
               }
               for ( int i = 1; i < index; i++ )
               {
                     #if defined(__LINUX_BASE)
                     token = wcstok(NULL, seps, &buffer);
                     #else
                     token = wcstok(NULL, seps);
                     #endif
               }
          }
     }

     return token;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool maptable::MultiType_IsTimeOut()
{
     bool onTime = false;

     #if defined(__LINUX_BASE)

     //Åªšúµ²§ô®É¶¡
     struct timeval EndTime, Result;
     MultiType_GetLocalTime(&EndTime);

     //®É¶¡®t
     timersub(&EndTime, &MultiType_StartTime, &Result);

     //ÂàŠš¬íŒÆ
     float Secs= MultiType_Time / 1000.0;

     if ( Result.tv_sec <= Secs )
     {
          //³Ì«á¥þÂàŠš·L¬íŒÆ€ñžû
          unsigned long SpendTime = Result.tv_sec * 1000000 + Result.tv_usec;
          unsigned long Stop      = MultiType_Time * 1000;

          //€p©ó³]©w®É¶¡œd³ò€º
          if ( SpendTime <= Stop )
               onTime = true;
     }

     //±NEndTimeŠs€JStartTime,·í°µ€U€@Šžªº°_©l®É¶¡
     MultiType_StartTime = EndTime;

     #endif

     return onTime;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void maptable::MultiType_GetLocalTime(void* LocalTime)
{
     struct timeval*
     time = (struct timeval*) LocalTime;

     #if defined(__LINUX_BASE)
     gettimeofday(time, NULL);
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void maptable::MultiType_ResetState()
{
     MultiType_KeyCount    =0;
     MultiType_LastKeyCode =-1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : for T9
//
//-----------------------------------------------------------------------------
void maptable::MultiType_ToBuffer(int _IME, unsigned int kcode, wchar_t *keyBuffer)
{
     wchar_t def[1024] ={0};
     wchar_t seps[] = L"\t";
     wchar_t *token = NULL;
     wchar_t *buffer = NULL;

     unsigned int       total;
     MultiTypeConfig    *table =NULL;

     mMultiSz[0] =0;

     Hindi::MultiType  =Hindi::MultiType_T0;
     Tamil::MultiType  =Tamil::MultiType_T0;
     Telugu::MultiType =Telugu::MultiType_T0;

     #if !defined(__HUAWEI) && defined(__SONIM)
     Bengali_BD::MultiType =Bengali_BD::MultiType_T0;
     Bengali_IN::MultiType =Bengali_IN::MultiType_T0;
     #endif

     MapMultiType(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("MultiType_ToBuffer::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("MultiType_ToBuffer::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          return;
     }

     if ( kcode >total -1 )
     {
          #if defined(__DEBUG_MSG) && defined(__TEST)
          printf("maptable_ToBuffer::curr table =%x, _IME =%d\r\n", table, _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("err!!, Out of range!! total MultiTap =%d, curr =%d\r\n\r\n", total -1, kcode);
          #endif
          return;
     }

     if ( table[kcode].code ==(char)kcode )
     {
          wcscpy(def, table[kcode].def);

          #if !defined(__BY_USE_TOKEN )

          wchar_t*
          pDef =def;

          for ( int i =0, nxt =0; pDef[i]; i++ )
          {
                if ( pDef[i] =='\t' )
                {
                     continue;
                }
                keyBuffer[nxt++] =pDef[i];
          }

          #else

          #if defined(__LINUX_BASE)
          token = wcstok(def, seps, &buffer);
          #else
          token = wcstok(def, seps);
          #endif

          while ( token !=0 )
          {
                  wcscat(keyBuffer, token);

                  #if defined(__LINUX_BASE)
                  token = wcstok(NULL, seps, &buffer);
                  #else
                  token = wcstok(NULL, seps);
                  #endif
          }
          #endif
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : for T9
//
//-----------------------------------------------------------------------------
void maptable::MultiType_ToBufferV(int _IME, unsigned int kcode, vector<wstring> &v)
{
     wchar_t def[1024] ={0};
     wchar_t seps[] = L"\t";
     wchar_t *token = NULL;
     wchar_t *buffer = NULL;

     unsigned int       total;
     MultiTypeConfig    *table =NULL;

     mMultiSz[0] =0;

     Hindi::MultiType  =Hindi::MultiType_T0;
     Tamil::MultiType  =Tamil::MultiType_T0;
     Telugu::MultiType =Telugu::MultiType_T0;

     #if !defined(__HUAWEI) && defined(__SONIM)
     Bengali_BD::MultiType =Bengali_BD::MultiType_T0;
     Bengali_IN::MultiType =Bengali_IN::MultiType_T0;
     #endif

     MapMultiType(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("MultiType_ToBufferV::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("MultiType_ToBufferV::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          return;
     }

     if ( kcode >total -1 )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("MultiType_ToBufferV::err!!, Out of range!! total MultiTap =%d, curr =%d\r\n\r\n", total -1, kcode);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("MultiType_ToBufferV::err!!, Out of range!! total MultiTap =%d, curr =%d\r\n\r\n", total -1, kcode);
          #endif
          return;
     }

     if ( table[kcode].code ==(char)kcode )
     {
          wcscpy(def, table[kcode].def);

          wchar_t*
          pDef =def;

          int nxt =0;
          for ( int i =0; pDef[i]; i++ )
          {
                if ( pDef[i] =='\t' )
                {
                     pDef[i] =0;
                     v.push_back(pDef +nxt);

                     #if defined(__DEBUG_MSG)
                     string
                     svoca =WstringToString(pDef +nxt);
                     KIKA_LOGD("MultiType_ToBufferV::add sz =%s\r\n", svoca.c_str());
                     #endif

                     nxt =i +1;
                     continue;
                }
          }
          if ( pDef[nxt] !=0 )
          {
               v.push_back(pDef +nxt);
          }
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : for T9
//
//-----------------------------------------------------------------------------
void maptable::MultiType_ToBufferEx(int _IME, unsigned int kcode, const wchar_t *wsSplitChar, int count)
{
     wchar_t def[1024] ={0};
     wchar_t seps[] = L"\t";
     wchar_t *token = NULL;
     wchar_t *buffer = NULL;

     unsigned int       total;
     MultiTypeConfig    *table =NULL;

     Hindi::MultiType  =Hindi::MultiType_T0;
     Tamil::MultiType  =Tamil::MultiType_T0;
     Telugu::MultiType =Telugu::MultiType_T0;

     #if !defined(__HUAWEI) && defined(__SONIM)
     Bengali_BD::MultiType =Bengali_BD::MultiType_T0;
     Bengali_IN::MultiType =Bengali_IN::MultiType_T0;
     #endif

     mMultiSz[0] =0;

     MapMultiType(_IME, total, &table);

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("MultiType_ToBufferEx::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("MultiType_ToBufferEx::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          return;
     }

     if ( kcode >total -1 )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("MultiType_ToBufferEx::err!!, Out of range!! total MultiTap =%d, curr =%d\r\n\r\n", total -1, kcode);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("MultiType_ToBufferEx::err!!, Out of range!! total MultiTap =%d, curr =%d\r\n\r\n", total -1, kcode);
          #endif
          return;
     }

     if ( table[kcode].code ==(char)kcode )
     {
          wcscpy(def, table[kcode].def);

          #if defined(__LINUX_BASE)
          token = wcstok(def, seps, &buffer);
          #else
          token = wcstok(def, seps);
          #endif

          int MaxCount =0;

          while ( token !=0 )
          {
                  ++MaxCount;

                  #if defined(__LINUX_BASE)
                  token = wcstok(NULL, seps, &buffer);
                  #else
                  token = wcstok(NULL, seps);
                  #endif
          }

          wcscpy(def, table[kcode].def);

          #if defined(__LINUX_BASE)
          token = wcstok(def, seps, &buffer);
          #else
          token = wcstok(def, seps);
          #endif

          int index   =0;
          int countID =count ==0 ? MaxCount :count;

          if ( MaxCount >0 )
          {
               while ( countID >MaxCount )
                       countID -=MaxCount;

               if ( token !=0 )
               {
                    while ( token !=0 )
                    {
                            if ( ++index ==countID )
                                 wcscat(mMultiSz, L">");

                            wcscat(mMultiSz, token);
                            wcscat(mMultiSz, wsSplitChar);

                            #if defined(__LINUX_BASE)
                            token = wcstok(NULL, seps, &buffer);
                            #else
                            token = wcstok(NULL, seps);
                            #endif
                    }
               }
               else if ( count ==1 )
               {
                    wcscat(mMultiSz, L">");
                    wcscat(mMultiSz, def);
               }
          }
     }
     #if defined(__DEBUG_MSG)
     KIKA_LOGD("MultiType_ToBufferEx done!");
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void maptable::MultiType_InputFmtSz(int _IME, unsigned int kcode, unsigned int prev_kcode, wchar_t *wsSplitChar, int count)
{
     wchar_t def[1024] ={0};
     wchar_t seps[] = L"\t";
     wchar_t *token = NULL;
     wchar_t *buffer = NULL;

     unsigned int       total;
     MultiTypeConfig    *table =NULL;

     switch ( _IME )
     {
              case IQQI_IME_Hindi:
                   {
                       wchar_t
                       wkcode[2] ={0, 0};
                       wkcode[0] =prev_kcode;

                       std::vector<wstring>::iterator
                       itr =std::find( Hindi_wData1_T2.begin(), Hindi_wData1_T2.end(), wstring(wkcode) );

                       if ( itr !=Hindi_wData1_T2.end() )
                       {
                            #if defined(__DEBUG_MSG) && defined(__TEST)
                            printf("Hindi use T2\r\n");
                            #endif
                            Hindi::MultiType =Hindi::MultiType_T2;
                       }
                       else
                       {
                            itr =std::find( Hindi_wData2_T3.begin(), Hindi_wData2_T3.end(), wstring(wkcode) );

                            if ( itr !=Hindi_wData2_T3.end() )
                            {
                                 #if defined(__DEBUG_MSG) && defined(__TEST)
                                 printf("Hindi use T3\r\n");
                                 #endif
                                 Hindi::MultiType =Hindi::MultiType_T3;
                            }
                            else
                            {
                                 itr =std::find( Hindi_wData3_T4.begin(), Hindi_wData3_T4.end(), wstring(wkcode) );

                                 if ( itr !=Hindi_wData3_T4.end() )
                                 {
                                      #if defined(__DEBUG_MSG) && defined(__TEST)
                                      printf("Hindi use T4\r\n");
                                      #endif
                                      Hindi::MultiType =Hindi::MultiType_T4;
                                 }
                                 else
                                 {
                                      #if defined(__DEBUG_MSG) && defined(__TEST)
                                      printf("Hindi use T1\r\n");
                                      #endif
                                      Hindi::MultiType =Hindi::MultiType_T1;
                                 }
                            }
                       }
                   }
                   break;

              case IQQI_IME_Tamil:
                   {
                       wchar_t
                       wkcode[2] ={0, 0};
                       wkcode[0] =prev_kcode;

                       std::vector<wstring>::iterator
                       itr =std::find( Tamil_wData1_T2.begin(), Tamil_wData1_T2.end(), wstring(wkcode) );

                       if ( itr !=Tamil_wData1_T2.end() )
                       {
                            #if defined(__DEBUG_MSG) && defined(__TEST)
                            printf("Tamil use T2\r\n");
                            #endif
                            Tamil::MultiType =Tamil::MultiType_T2;
                       }
                       else
                       {
                            itr =std::find( Tamil_wData2_T3.begin(), Tamil_wData2_T3.end(), wstring(wkcode) );

                            if ( itr !=Tamil_wData2_T3.end() )
                            {
                                 #if defined(__DEBUG_MSG) && defined(__TEST)
                                 printf("Tamil use T3\r\n");
                                 #endif
                                 Tamil::MultiType =Tamil::MultiType_T3;
                            }
                            else
                            {
                                 #if defined(__DEBUG_MSG) && defined(__TEST)
                                 printf("Tamil use T1\r\n");
                                 #endif
                                 Tamil::MultiType =Tamil::MultiType_T1;
                            }
                       }
                   }
                   break;

              case IQQI_IME_Telugu:
                   {
                       wchar_t
                       wkcode[2] ={0, 0};
                       wkcode[0] =prev_kcode;

                       std::vector<wstring>::iterator
                       itr =std::find( Telugu_wData1_T2.begin(), Telugu_wData1_T2.end(), wstring(wkcode) );

                       if ( itr !=Telugu_wData1_T2.end() )
                       {
                            #if defined(__DEBUG_MSG) && defined(__TEST)
                            printf("Telugu use T2\r\n");
                            #endif
                            Telugu::MultiType =Telugu::MultiType_T2;
                       }
                       else
                       {
                            itr =std::find( Telugu_wData1_T3.begin(), Telugu_wData1_T3.end(), wstring(wkcode) );

                            if ( itr !=Telugu_wData1_T3.end() )
                            {
                                 #if defined(__DEBUG_MSG) && defined(__TEST)
                                 printf("Telugu use T3\r\n");
                                 #endif
                                 Telugu::MultiType =Telugu::MultiType_T3;
                            }
                            else
                            {
                                 #if defined(__DEBUG_MSG) && defined(__TEST)
                                 printf("Telugu use T1\r\n");
                                 #endif
                                 Telugu::MultiType =Telugu::MultiType_T1;
                            }
                       }
                   }
                   break;

              #if !defined(__HUAWEI) && defined(__SONIM)

              case IQQI_IME_Bengali_BD:
                   {
                       wchar_t
                       wkcode[2] ={0, 0};
                       wkcode[0] =prev_kcode;

                       std::vector<wstring>::iterator
                       itr =std::find( Bengali_BD_wData1_T2.begin(), Bengali_BD_wData1_T2.end(), wstring(wkcode) );

                       if ( itr !=Bengali_BD_wData1_T2.end() )
                       {
                            #if defined(__TEST)
                            printf("Bengali_BD use T2\r\n");
                            #endif
                            Bengali_BD::MultiType =Bengali_BD::MultiType_T2;
                       }
                       else
                       {
                            itr =std::find( Bengali_BD_wData2_T3.begin(), Bengali_BD_wData2_T3.end(), wstring(wkcode) );

                            if ( itr !=Bengali_BD_wData2_T3.end() )
                            {
                                 #if defined(__TEST)
                                 printf("Bengali_BD use T3\r\n");
                                 #endif
                                 Bengali_BD::MultiType =Bengali_BD::MultiType_T3;
                            }
                            else
                            {
                                 #if defined(__TEST)
                                 printf("Bengali_BD use T1\r\n");
                                 #endif
                                 Bengali_BD::MultiType =Bengali_BD::MultiType_T1;
                            }
                       }
                   }
                   break;

              case IQQI_IME_Bengali_IN:
                   {
                       wchar_t
                       wkcode[2] ={0, 0};
                       wkcode[0] =prev_kcode;

                       std::vector<wstring>::iterator
                       itr =std::find( Bengali_IN_wData1_T2.begin(), Bengali_IN_wData1_T2.end(), wstring(wkcode) );

                       if ( itr !=Bengali_IN_wData1_T2.end() )
                       {
                            #if defined(__TEST)
                            printf("Bengali_IN use T2\r\n");
                            #endif
                            Bengali_IN::MultiType =Bengali_IN::MultiType_T2;
                       }
                       else
                       {
                            itr =std::find( Bengali_IN_wData2_T3.begin(), Bengali_IN_wData2_T3.end(), wstring(wkcode) );

                            if ( itr !=Bengali_IN_wData2_T3.end() )
                            {
                                 #if defined(__TEST)
                                 printf("Bengali_IN use T3\r\n");
                                 #endif
                                 Bengali_IN::MultiType =Bengali_IN::MultiType_T3;
                            }
                            else
                            {
                                 #if defined(__TEST)
                                 printf("Bengali_IN use T1\r\n");
                                 #endif
                                 Bengali_IN::MultiType =Bengali_IN::MultiType_T1;
                            }
                       }
                   }
                   break;

              #endif
     }
     MapMultiType(_IME, total, &table);

     mMultiSz[0] ='\0';

     if ( table ==nullptr )
     {
          #if defined(__DEBUG_MSG)
          KIKA_LOGD("MultiType_InputFmtSz::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("MultiType_InputFmtSz::curr table ==null, _IME =%d\r\n", _IME);
          #endif
          return;
     }

     if ( kcode >total -1 )
     {
          #if defined(__DEBUG_MSG) && defined(__TEST)
          printf("MultiType_InputFmtSz::curr table =%x, _IME =%d\r\n", table, _IME);
          #endif
          #if defined(__DEBUG_MSG) || defined(__TEST)
          printf("err!!, Out of range!! total MultiTap =%d, curr =%d\r\n\r\n", total -1, kcode);
          #endif
          return;
     }

     if ( table[kcode].code ==(char)kcode )
     {
          wcscpy(def, table[kcode].def);

          #if defined(__LINUX_BASE)
          token = wcstok(def, seps, &buffer);
          #else
          token = wcstok(def, seps);
          #endif

          int MaxCount =0;

          while ( token !=0 )
          {
                  ++MaxCount;

                  #if defined(__LINUX_BASE)
                  token = wcstok(NULL, seps, &buffer);
                  #else
                  token = wcstok(NULL, seps);
                  #endif
          }

          wcscpy(def, table[kcode].def);

          #if defined(__LINUX_BASE)
          token = wcstok(def, seps, &buffer);
          #else
          token = wcstok(def, seps);
          #endif

          int index   =0;
          int countID =count ==0 ? MaxCount :count;

          if ( MaxCount >0 )
          {
               while ( countID >MaxCount )
                       countID -=MaxCount;

               if ( token !=0 )
               {
                    while ( token !=0 )
                    {
                            if ( ++index ==countID )
                                 wcscat(mMultiSz, L">");

                            wcscat(mMultiSz, token);
                            wcscat(mMultiSz, wsSplitChar);

                            #if defined(__LINUX_BASE)
                            token = wcstok(NULL, seps, &buffer);
                            #else
                            token = wcstok(NULL, seps);
                            #endif
                    }
               }
               else if ( count ==1 )
               {
                    wcscat(mMultiSz, L">");
                    wcscat(mMultiSz, def);
               }
          }
     }
}
