#ifndef TELUGU_H_
#define TELUGU_H_

#include "maptable.h"

#define TELUGU_MULTI_TYPE_SET_NUM       11
#define TELUGU_LAYOUT_LETTER_NUM_T9     64
#define TELUGU_LAYOUT_LETTER_NUM_QWERTY 73

class Telugu
{
public:
    Telugu();

    static wchar_t              wData1[100];
    static wchar_t              wData2[100];
    static _MultiTypeConfig*    MultiType;

    static _MultiTypeConfig     MultiType_T0[TELUGU_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T1[TELUGU_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T2[TELUGU_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T3[TELUGU_MULTI_TYPE_SET_NUM];

    static _VirtualKeybdMapping VK_Layout_T9[TELUGU_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[TELUGU_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* TELUGU_H_ */
