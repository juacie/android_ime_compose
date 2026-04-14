#ifndef BURMESE_Z_H_
#define BURMESE_Z_H_

#include "maptable.h"

#define BURMESE_Z_MULTI_TYPE_SET_NUM       11
#define BURMESE_Z_LAYOUT_LETTER_NUM_T9     94
#define BURMESE_Z_LAYOUT_LETTER_NUM_QWERTY 76

class Burmese_Z
{
public:
    Burmese_Z();

    static _MultiTypeConfig     MultiType[BURMESE_Z_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[BURMESE_Z_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[BURMESE_Z_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* BURMESE_Z_H_ */