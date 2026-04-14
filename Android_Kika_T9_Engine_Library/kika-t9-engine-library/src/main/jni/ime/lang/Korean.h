#ifndef KOREAN_H_
#define KOREAN_H_

#include "maptable.h"

#define KOREAN_MULTI_TYPE_SET_NUM       11
#define KOREAN_LAYOUT_LETTER_NUM_T9     0
#define KOREAN_LAYOUT_LETTER_NUM_QWERTY 31

class Korean
{
public:
    Korean();

    static _MultiTypeConfig     MultiType[KOREAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[KOREAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[KOREAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* KOREAN_H_ */