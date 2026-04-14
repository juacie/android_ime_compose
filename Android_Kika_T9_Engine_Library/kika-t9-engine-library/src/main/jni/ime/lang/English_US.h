#ifndef ENGLISH_US_H_
#define ENGLISH_US_H_

#include "maptable.h"

#define ENGLISH_US_MULTI_TYPE_SET_NUM       11
#define ENGLISH_US_LAYOUT_LETTER_NUM_T9     26
#define ENGLISH_US_LAYOUT_LETTER_NUM_QWERTY 27

class English_US
{
public:
    English_US();

    static _MultiTypeConfig     MultiType[ENGLISH_US_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ENGLISH_US_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ENGLISH_US_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ENGLISH_US_H_ */