#ifndef SWEDISH_H_
#define SWEDISH_H_

#include "maptable.h"

#define SWEDISH_MULTI_TYPE_SET_NUM       11
#define SWEDISH_LAYOUT_LETTER_NUM_T9     29
#define SWEDISH_LAYOUT_LETTER_NUM_QWERTY 29

class Swedish
{
public:
    Swedish();

    static _MultiTypeConfig     MultiType[SWEDISH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SWEDISH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SWEDISH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SWEDISH_H_ */
