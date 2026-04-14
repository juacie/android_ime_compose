#ifndef BULGARIAN_H_
#define BULGARIAN_H_

#include "maptable.h"

#define BULGARIAN_MULTI_TYPE_SET_NUM       11
#define BULGARIAN_LAYOUT_LETTER_NUM_T9     32
#define BULGARIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Bulgarian
{
public:
    Bulgarian();

    static _MultiTypeConfig     MultiType[BULGARIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[BULGARIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[BULGARIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* BULGARIAN_H_ */