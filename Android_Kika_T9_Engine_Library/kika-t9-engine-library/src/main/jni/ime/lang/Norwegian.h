#ifndef NORWEGIAN_H_
#define NORWEGIAN_H_

#include "maptable.h"

#define NORWEGIAN_MULTI_TYPE_SET_NUM       11
#define NORWEGIAN_LAYOUT_LETTER_NUM_T9     54
#define NORWEGIAN_LAYOUT_LETTER_NUM_QWERTY 29

class Norwegian
{
public:
    Norwegian();

    static _MultiTypeConfig     MultiType[NORWEGIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[NORWEGIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[NORWEGIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* NORWEGIAN_H_ */