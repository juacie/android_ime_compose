#ifndef ENGLISH_GB_H_
#define ENGLISH_GB_H_

#include "maptable.h"

#define ENGLISH_GB_MULTI_TYPE_SET_NUM       11
#define ENGLISH_GB_LAYOUT_LETTER_NUM_T9     26
#define ENGLISH_GB_LAYOUT_LETTER_NUM_QWERTY 27

class English_GB
{
public:
    English_GB();

    static _MultiTypeConfig     MultiType[ENGLISH_GB_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ENGLISH_GB_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ENGLISH_GB_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ENGLISH_GB_H_ */