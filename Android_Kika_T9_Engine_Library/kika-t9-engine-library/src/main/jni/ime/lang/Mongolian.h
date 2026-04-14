#ifndef MONGOLIAN_H_
#define MONGOLIAN_H_

#include "maptable.h"

#define MONGOLIAN_MULTI_TYPE_SET_NUM       11
#define MONGOLIAN_LAYOUT_LETTER_NUM_T9     0
#define MONGOLIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Mongolian
{
public:
    Mongolian();

    static _MultiTypeConfig     MultiType[MONGOLIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[MONGOLIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[MONGOLIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* MONGOLIAN_H_ */