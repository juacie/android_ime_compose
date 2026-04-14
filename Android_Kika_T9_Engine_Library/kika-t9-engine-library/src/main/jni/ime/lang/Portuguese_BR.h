#ifndef PORTUGUESE_BR_H_
#define PORTUGUESE_BR_H_

#include "maptable.h"

#define PORTUGUESE_BR_MULTI_TYPE_SET_NUM       11
#define PORTUGUESE_BR_LAYOUT_LETTER_NUM_T9     40
#define PORTUGUESE_BR_LAYOUT_LETTER_NUM_QWERTY 26

class Portuguese_BR
{
public:
    Portuguese_BR();

    static _MultiTypeConfig     MultiType[PORTUGUESE_BR_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[PORTUGUESE_BR_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[PORTUGUESE_BR_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* PORTUGUESE_BR_H_ */
