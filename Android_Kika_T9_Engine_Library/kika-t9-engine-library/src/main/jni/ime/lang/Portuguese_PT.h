#ifndef PORTUGUESE_PT_H_
#define PORTUGUESE_PT_H_

#include "maptable.h"

#define PORTUGUESE_PT_MULTI_TYPE_SET_NUM       11
#define PORTUGUESE_PT_LAYOUT_LETTER_NUM_T9     40
#define PORTUGUESE_PT_LAYOUT_LETTER_NUM_QWERTY 26

class Portuguese_PT
{
public:
    Portuguese_PT();

    static _MultiTypeConfig     MultiType[PORTUGUESE_PT_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[PORTUGUESE_PT_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[PORTUGUESE_PT_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* PORTUGUESE_PT_H_ */
