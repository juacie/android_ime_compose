#ifndef SPANISH_US_H_
#define SPANISH_US_H_

#include "maptable.h"

#define SPANISH_US_MULTI_TYPE_SET_NUM       11
#define SPANISH_US_LAYOUT_LETTER_NUM_T9     34
#define SPANISH_US_LAYOUT_LETTER_NUM_QWERTY 27

class Spanish_US
{
public:
    Spanish_US();

    static _MultiTypeConfig     MultiType[SPANISH_US_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SPANISH_US_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SPANISH_US_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SPANISH_US_H_ */