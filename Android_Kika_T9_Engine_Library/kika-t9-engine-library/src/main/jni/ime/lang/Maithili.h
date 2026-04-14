#ifndef MAITHILI_H_
#define MAITHILI_H_

#include "maptable.h"

#define MAITHILI_MULTI_TYPE_SET_NUM       11
#define MAITHILI_LAYOUT_LETTER_NUM_T9     82
#define MAITHILI_LAYOUT_LETTER_NUM_QWERTY 71

class Maithili
{
public:
    Maithili();

    static _MultiTypeConfig     MultiType[MAITHILI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[MAITHILI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[MAITHILI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* MAITHILI_H_ */