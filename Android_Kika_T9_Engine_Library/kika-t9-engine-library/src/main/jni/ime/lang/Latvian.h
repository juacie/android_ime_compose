#ifndef LATVIAN_H_
#define LATVIAN_H_

#include "maptable.h"

#define LATVIAN_MULTI_TYPE_SET_NUM       11
#define LATVIAN_LAYOUT_LETTER_NUM_T9     37
#define LATVIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Latvian
{
public:
    Latvian();

    static _MultiTypeConfig     MultiType[LATVIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[LATVIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[LATVIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* LATVIAN_H_ */
