#ifndef DUTCH_H_
#define DUTCH_H_

#include "maptable.h"

#define DUTCH_MULTI_TYPE_SET_NUM       11
#define DUTCH_LAYOUT_LETTER_NUM_T9     35
#define DUTCH_LAYOUT_LETTER_NUM_QWERTY 26

class Dutch
{
public:
    Dutch();

    static _MultiTypeConfig     MultiType[DUTCH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[DUTCH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[DUTCH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* DUTCH_H_ */
