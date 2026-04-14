#ifndef LITHUANIAN_H_
#define LITHUANIAN_H_

#include "maptable.h"

#define LITHUANIAN_MULTI_TYPE_SET_NUM       11
#define LITHUANIAN_LAYOUT_LETTER_NUM_T9     35
#define LITHUANIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Lithuanian
{
public:
    Lithuanian();

    static _MultiTypeConfig     MultiType[LITHUANIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[LITHUANIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[LITHUANIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* LITHUANIAN_H_ */
