#ifndef POLISH_H_
#define POLISH_H_

#include "maptable.h"

#define POLISH_MULTI_TYPE_SET_NUM       11
#define POLISH_LAYOUT_LETTER_NUM_T9     38
#define POLISH_LAYOUT_LETTER_NUM_QWERTY 26

class Polish
{
public:
    Polish();

    static _MultiTypeConfig     MultiType[POLISH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[POLISH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[POLISH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* POLISH_H_ */