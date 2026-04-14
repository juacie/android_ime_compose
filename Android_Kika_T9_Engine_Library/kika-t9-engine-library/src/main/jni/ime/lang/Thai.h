#ifndef THAI_H_
#define THAI_H_

#include "maptable.h"

#define THAI_MULTI_TYPE_SET_NUM       11
#define THAI_LAYOUT_LETTER_NUM_T9     70
#define THAI_LAYOUT_LETTER_NUM_QWERTY 83

class Thai
{
public:
    Thai();

    static _MultiTypeConfig     MultiType[THAI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[THAI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[THAI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* THAI_H_ */