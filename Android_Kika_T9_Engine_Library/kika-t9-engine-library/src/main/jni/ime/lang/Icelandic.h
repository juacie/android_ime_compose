#ifndef ICELANDIC_H_
#define ICELANDIC_H_

#include "maptable.h"

#define ICELANDIC_MULTI_TYPE_SET_NUM       11
#define ICELANDIC_LAYOUT_LETTER_NUM_T9     54
#define ICELANDIC_LAYOUT_LETTER_NUM_QWERTY 27

class Icelandic
{
public:
    Icelandic();

    static _MultiTypeConfig     MultiType[ICELANDIC_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ICELANDIC_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ICELANDIC_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ICELANDIC_H_ */
