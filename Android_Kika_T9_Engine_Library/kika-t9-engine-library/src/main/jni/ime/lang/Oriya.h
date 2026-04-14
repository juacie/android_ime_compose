#ifndef ORIYA_H_
#define ORIYA_H_

#include "maptable.h"

#define ORIYA_MULTI_TYPE_SET_NUM       11
#define ORIYA_LAYOUT_LETTER_NUM_T9     83
#define ORIYA_LAYOUT_LETTER_NUM_QWERTY 76

class Oriya
{
public:
    Oriya();

    static _MultiTypeConfig     MultiType[ORIYA_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ORIYA_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ORIYA_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ORIYA_H_ */