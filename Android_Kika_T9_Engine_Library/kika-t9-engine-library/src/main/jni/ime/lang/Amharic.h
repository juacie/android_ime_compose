#ifndef AMHARIC_H_
#define AMHARIC_H_

#include "maptable.h"

#define AMHARIC_MULTI_TYPE_SET_NUM       11
#define AMHARIC_LAYOUT_LETTER_NUM_T9     0
#define AMHARIC_LAYOUT_LETTER_NUM_QWERTY 35

class Amharic
{
public:
    Amharic();

    static _MultiTypeConfig     MultiType[AMHARIC_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[AMHARIC_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[AMHARIC_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* AMHARIC_H_ */