#ifndef SLOVAK_H_
#define SLOVAK_H_

#include "maptable.h"

#define SLOVAK_MULTI_TYPE_SET_NUM       11
#define SLOVAK_LAYOUT_LETTER_NUM_T9     43
#define SLOVAK_LAYOUT_LETTER_NUM_QWERTY 26

class Slovak
{
public:
    Slovak();

    static _MultiTypeConfig     MultiType[SLOVAK_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SLOVAK_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SLOVAK_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SLOVAK_H_ */
