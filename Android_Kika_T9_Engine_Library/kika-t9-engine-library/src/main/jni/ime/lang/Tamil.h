#ifndef TAMIL_H_
#define TAMIL_H_

#include "maptable.h"

#define TAMIL_MULTI_TYPE_SET_NUM       11
#define TAMIL_LAYOUT_LETTER_NUM_T9     46
#define TAMIL_LAYOUT_LETTER_NUM_QWERTY 82

class Tamil
{
public:
    Tamil();

    static wchar_t              wData1[100];
    static wchar_t              wData2[100];
    static _MultiTypeConfig*    MultiType;

    static _MultiTypeConfig     MultiType_T0[TAMIL_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T1[TAMIL_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T2[TAMIL_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T3[TAMIL_MULTI_TYPE_SET_NUM];

    static _VirtualKeybdMapping VK_Layout_T9[TAMIL_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[TAMIL_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* TAMIL_H_ */