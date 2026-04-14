#ifndef DANISH_H_
#define DANISH_H_

#include "maptable.h"

#define DANISH_MULTI_TYPE_SET_NUM       11
#define DANISH_LAYOUT_LETTER_NUM_T9     60
#define DANISH_LAYOUT_LETTER_NUM_QWERTY 29

class Danish
{
public:
    Danish();

    static _MultiTypeConfig     MultiType[DANISH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[DANISH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[DANISH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* DANISH_H_ */