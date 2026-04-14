#ifndef UKRAINIAN_H_
#define UKRAINIAN_H_

#include "maptable.h"

#define UKRAINIAN_MULTI_TYPE_SET_NUM       11
#define UKRAINIAN_LAYOUT_LETTER_NUM_T9     33
#define UKRAINIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Ukrainian
{
public:
    Ukrainian();

    static _MultiTypeConfig     MultiType[UKRAINIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[UKRAINIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[UKRAINIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* UKRAINIAN_H_ */
