#ifndef JAWA_H_
#define JAWA_H_

#include "maptable.h"

#define JAWA_MULTI_TYPE_SET_NUM       11
#define JAWA_LAYOUT_LETTER_NUM_T9     26
#define JAWA_LAYOUT_LETTER_NUM_QWERTY 26

class Jawa
{
public:
    Jawa();

    static _MultiTypeConfig     MultiType[JAWA_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[JAWA_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[JAWA_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* JAWA_H_ */