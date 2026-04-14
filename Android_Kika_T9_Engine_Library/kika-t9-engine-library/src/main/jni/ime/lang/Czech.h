#ifndef CZECH_H_
#define CZECH_H_

#include "maptable.h"

#define CZECH_MULTI_TYPE_SET_NUM       11
#define CZECH_LAYOUT_LETTER_NUM_T9     42
#define CZECH_LAYOUT_LETTER_NUM_QWERTY 26

class Czech
{
public:
    Czech();

    static _MultiTypeConfig     MultiType[CZECH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[CZECH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[CZECH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* CZECH_H_ */