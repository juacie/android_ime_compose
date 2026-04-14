#ifndef NEPALI_H_
#define NEPALI_H_

#include "maptable.h"

#define NEPALI_MULTI_TYPE_SET_NUM       11
#define NEPALI_LAYOUT_LETTER_NUM_T9     95
#define NEPALI_LAYOUT_LETTER_NUM_QWERTY 70

class Nepali
{
public:
    Nepali();

    static _MultiTypeConfig     MultiType[NEPALI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[NEPALI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[NEPALI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* NEPALI_H_ */