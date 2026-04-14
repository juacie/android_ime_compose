#ifndef HUNGARIAN_H_
#define HUNGARIAN_H_

#include "maptable.h"

#define HUNGARIAN_MULTI_TYPE_SET_NUM       11
#define HUNGARIAN_LAYOUT_LETTER_NUM_T9     35
#define HUNGARIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Hungarian
{
public:
    Hungarian();

    static _MultiTypeConfig     MultiType[HUNGARIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[HUNGARIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[HUNGARIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* HUNGARIAN_H_ */
