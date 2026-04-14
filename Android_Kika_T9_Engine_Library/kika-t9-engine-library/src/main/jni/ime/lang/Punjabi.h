#ifndef PUNJABI_H_
#define PUNJABI_H_

#include "maptable.h"

#define PUNJABI_MULTI_TYPE_SET_NUM       11
#define PUNJABI_LAYOUT_LETTER_NUM_T9     74
#define PUNJABI_LAYOUT_LETTER_NUM_QWERTY 71

class Punjabi
{
public:
    Punjabi();

    static _MultiTypeConfig     MultiType[PUNJABI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[PUNJABI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[PUNJABI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* PUNJABI_H_ */