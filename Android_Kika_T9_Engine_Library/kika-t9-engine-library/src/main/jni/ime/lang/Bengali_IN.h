#ifndef BENGALI_IN_H_
#define BENGALI_IN_H_

#include "maptable.h"

#define BENGALI_IN_MULTI_TYPE_SET_NUM       11
#define BENGALI_IN_LAYOUT_LETTER_NUM_T9     73
#define BENGALI_IN_LAYOUT_LETTER_NUM_QWERTY 73

class Bengali_IN
{
public:
    Bengali_IN();

    #if !defined(__HUAWEI) && defined(__SONIM)
    static wchar_t              wData1[100];
    static wchar_t              wData2[100];
    static _MultiTypeConfig*    MultiType;

    static _MultiTypeConfig     MultiType_T0[BENGALI_IN_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T1[BENGALI_IN_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T2[BENGALI_IN_MULTI_TYPE_SET_NUM];
    static _MultiTypeConfig     MultiType_T3[BENGALI_IN_MULTI_TYPE_SET_NUM];
    #else
    static _MultiTypeConfig     MultiType[BENGALI_IN_MULTI_TYPE_SET_NUM];
    #endif

    static _VirtualKeybdMapping VK_Layout_T9[BENGALI_IN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[BENGALI_IN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* BENGALI_IN_H_ */