#ifndef GALICIAN_H_
#define GALICIAN_H_

#include "maptable.h"

#define GALICIAN_MULTI_TYPE_SET_NUM                 10
#define GALICIAN_LAYOUT_LETTER_NUM_T9     38
#define GALICIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Galician
{
public:
    Galician();

    static _MultiTypeConfig     MultiType[GALICIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[GALICIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[GALICIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* GALICIAN_H_ */