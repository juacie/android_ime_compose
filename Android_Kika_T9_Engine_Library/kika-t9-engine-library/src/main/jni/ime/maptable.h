/*
* maptable.h
*
*  Created on: 2017/10/12      Author: seckychen
*/

#ifndef MAPTABLE_H_
#define MAPTABLE_H_

#include <wchar.h>
#include "string_util.h"

#if !defined(__NO_KIKA_IME)
#include "kikaime.h"
#endif

enum KEYBD_MODE
{
     for_T9     =1,
     for_Qwerty =2
};

enum IQQI_IME_ID
{
     IQQI_IME_English_US =1,
     IQQI_IME_French_CA,
     IQQI_IME_Portuguese_BR,
     IQQI_IME_Spanish_US,
     IQQI_IME_Assamese,
     IQQI_IME_Bengali_IN,
     IQQI_IME_Bodo,
     IQQI_IME_Dogri,
     IQQI_IME_Gujarati,
     IQQI_IME_Hindi,
     IQQI_IME_Kannada,
     IQQI_IME_Kashmiri,
     IQQI_IME_Konkani,
     IQQI_IME_Malayalam,
     IQQI_IME_Manipuri,
     IQQI_IME_Marathi,
     IQQI_IME_Nepali,
     IQQI_IME_Oriya,
     IQQI_IME_Punjabi,
     IQQI_IME_Sanskrit,
     IQQI_IME_Santhali,
     IQQI_IME_Sindhi,
     IQQI_IME_Tamil,
     IQQI_IME_Telugu,
     IQQI_IME_Urdu,
     IQQI_IME_Afrikaans,
     IQQI_IME_Arabic,
     IQQI_IME_Chinese_CN,
     IQQI_IME_Dutch,
     IQQI_IME_English_GB,
     IQQI_IME_French_FR,
     IQQI_IME_German,
     IQQI_IME_Hungarian,
     IQQI_IME_Indonesian,
     IQQI_IME_Italian,
     IQQI_IME_Malay,
     IQQI_IME_Persian,
     IQQI_IME_Polish,
     IQQI_IME_Portuguese_PT,
     IQQI_IME_Romanian,
     IQQI_IME_Russian,
     IQQI_IME_Spanish_ES,
     IQQI_IME_Swahili,
     IQQI_IME_Thai,
     IQQI_IME_Turkish,
     IQQI_IME_Vietnamese,
     IQQI_IME_Zulu,
     IQQI_IME_Bengali_BD,
     IQQI_IME_Bulgarian,
     IQQI_IME_Croatian,
     IQQI_IME_Czech,
     IQQI_IME_Finnish,
     IQQI_IME_Greek,
     IQQI_IME_Kazakh,
     IQQI_IME_Khmer,
     IQQI_IME_Macedonian,
     IQQI_IME_Serbian,
     IQQI_IME_Sinhala,
     IQQI_IME_Slovak,
     IQQI_IME_Slovenian,
     IQQI_IME_Swedish,
     IQQI_IME_Tagalog,
     IQQI_IME_Ukrainian,
     IQQI_IME_Xhosa,
     IQQI_IME_Albanian,
     IQQI_IME_Armenian,
     IQQI_IME_Azerbaijani,
     IQQI_IME_Belarusian,
     IQQI_IME_Bosnian,
     IQQI_IME_Chinese_HK,
     IQQI_IME_Chinese_TW,
     IQQI_IME_Danish,
     IQQI_IME_Estonian,
     IQQI_IME_Georgian,
     IQQI_IME_Hebrew,
     IQQI_IME_Icelandic,
     IQQI_IME_Lao,
     IQQI_IME_Latvian,
     IQQI_IME_Lithuanian,
     IQQI_IME_Norwegian,
     IQQI_IME_Uzbek_Cyrillic,
     IQQI_IME_Basque,
     IQQI_IME_Galician,
     IQQI_IME_Malagasy,
     IQQI_IME_Yiddish,
     IQQI_IME_Korean,
     IQQI_IME_Catalan,
     IQQI_IME_Japanese_KN,
     IQQI_IME_Japanese_RM,
     IQQI_IME_Changjie,
     IQQI_IME_Quick_CJ,
     IQQI_IME_Farsi,
     IQQI_IME_Maithili,
     IQQI_IME_Burmese_Z,
     IQQI_IME_Burmese_U,
     IQQI_IME_Tibetan,
     IQQI_IME_Uyghur,
     IQQI_IME_Jawa,
     IQQI_IME_Maori,
     IQQI_IME_Mongolian,
     IQQI_IME_Amharic,
     #if defined(__HINDI_BASE)
     IQQI_IME_Hindi_01,
     IQQI_IME_Hindi_02,
     IQQI_IME_Hindi_03,
     IQQI_IME_Hindi_04,
     IQQI_IME_Hindi_05,
     IQQI_IME_Hindi_06,
     IQQI_IME_Hindi_07,
     IQQI_IME_Hindi_08,
     IQQI_IME_Hindi_09,
     IQQI_IME_Hindi_10,
     IQQI_IME_Hindi_11,
     IQQI_IME_Hindi_12,
     IQQI_IME_Hindi_13,
     IQQI_IME_Hindi_14,
     IQQI_IME_Hindi_15,
     IQQI_IME_Hindi_16,
     IQQI_IME_Hindi_17,
     IQQI_IME_Hindi_18,
     IQQI_IME_Hindi_19,
     IQQI_IME_Hindi_20,
     IQQI_IME_Hindi_21,
     IQQI_IME_Hindi_22,
     #endif
};

#if !defined(__NO_KIKA_IME)

//struct Table_Map
//{
//     int                     imeId;
//     _VirtualKeybdMapping*   table;
//};

typedef struct MultiTypeConfig
{
     char               code;
     char               num;
     wchar_t            def[128];
     bool               timeout_ctrl;
}
_MultiTypeConfig;

typedef struct VirtualKeybdConfig
{
     unsigned int       x;
     unsigned int       y;
     unsigned int       width;
     unsigned int       height;
     unsigned int       kcode;
}
_VirtualKeybdConfig;


typedef struct VirtualKeybdMapping
{
     unsigned int       x;
     unsigned int       y;
     unsigned int       width;
     unsigned int       height;
     unsigned int       kcode;
     unsigned int       kcode_L;
     bool               e_index;
     unsigned int       hit_x;
     unsigned int       hit_y;

}
_VirtualKeybdMapping;

#endif

#define MAX_TMP_BUFF         64
#define MAX_BKEY_BTN         120

class maptable
{
private:
      KEYBD_MODE             mKbMode;
      int                    mBTotal;
      wchar_t                mMultiSz[1024];
      map<wstring, wstring>  mZhStrokes;
      map<wstring, wstring>  mCjStrokes;
      map<wstring, wstring>  mEngZhStrokes;

      #if !defined(__NO_KIKA_IME)
      key                    mBKeys[MAX_BKEY_BTN];
      #endif

public:
      maptable();

      wchar_t MultiType_Def_Tmp[MAX_TMP_BUFF];

      void setKbMode(KEYBD_MODE mode);
      
      KEYBD_MODE getKbMode()
      {
           return mKbMode;
      }

      wchar_t* GetMultiSz()
      {
           return mMultiSz;
      }

      int  GetLayoutTotalBtn()
      {
           return mBTotal;
      }

      #if !defined(__NO_KIKA_IME)

      key* GetLayout()
      {
           return mBKeys;
      }

      key* GetLayout(int _IME);

      void MapMultiType(int _IME, unsigned int &total, MultiTypeConfig **table);
      void MapVkTable(int _IME, unsigned int &total, VirtualKeybdMapping **table);
      void MapVkTableEx(int _IME, unsigned int &total, VirtualKeybdMapping **table, KEYBD_MODE mode);

      #endif

      wchar_t* MultiType_Input(int _IME, int keyCode, int count);
      void MultiType_ToBuffer(int _IME, unsigned int kcode, wchar_t *keyBuffer);
      void MultiType_ToBufferV(int _IME, unsigned int kcode, vector<wstring> &v);
      void MultiType_ToBufferEx(int _IME, unsigned int kcode, const wchar_t *wsSplitChar, int count);
      void MultiType_InputFmtSz(int _IME, unsigned int kcode, unsigned int prev_kcode, wchar_t *wsSplitChar, int count);

      bool MultiType_IsTimeOut();
      void MultiType_GetLocalTime(void* localTime);
      void MultiType_ResetState();

      wchar_t keyCode2Shift(int _IME, const wchar_t kcode);

      bool keyCode2Numb   (int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize);
      bool keyCode2NumbEx (int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize);

      bool Numb2keyCode    (int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize);
      bool Numb2keyCodeAll (int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize);

      bool Numb2EngCode    (int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize);
      bool EngCodeToZHKey  (int _IME, const wchar_t* kcode, wchar_t* ncode, unsigned int buffSize);

      bool KeyCode2BtnXY(int _IME, unsigned int kcode, unsigned int &x, unsigned int &y);
      bool KeyCode2HitXY(int _IME, unsigned int kcode, unsigned int &x, unsigned int &y);

      static int MultiType_Time;
      static int MultiType_LastKeyCode;
      static int MultiType_KeyCount;
      static struct timeval MultiType_StartTime;
};

#endif /* MAPTABLE_H_ */

