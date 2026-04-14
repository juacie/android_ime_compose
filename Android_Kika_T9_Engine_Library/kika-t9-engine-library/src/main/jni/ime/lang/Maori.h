#ifndef MAORI_H_
#define MAORI_H_

#include "maptable.h"

#define MAORI_MULTI_TYPE_SET_NUM       11
#define MAORI_LAYOUT_LETTER_NUM_T9     26
#define MAORI_LAYOUT_LETTER_NUM_QWERTY 26

class Maori
{
public:
    Maori();

    static _MultiTypeConfig     MultiType[MAORI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[MAORI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[MAORI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* MAORI_H_ */