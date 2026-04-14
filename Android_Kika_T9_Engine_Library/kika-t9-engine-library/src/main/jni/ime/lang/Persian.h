#ifndef PERSIAN_H_
#define PERSIAN_H_

#include "maptable.h"

#define PERSIAN_MULTI_TYPE_SET_NUM       11
#define PERSIAN_LAYOUT_LETTER_NUM_T9     36
#define PERSIAN_LAYOUT_LETTER_NUM_QWERTY 27

class Persian
{
public:
    Persian();

    static _MultiTypeConfig     MultiType[PERSIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[PERSIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[PERSIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* PERSIAN_H_ */
