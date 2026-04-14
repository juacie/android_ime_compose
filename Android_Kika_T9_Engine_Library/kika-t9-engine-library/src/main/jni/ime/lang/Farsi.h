#ifndef FARSI_H_
#define FARSI_H_

#include "maptable.h"

#define FARSI_MULTI_TYPE_SET_NUM       11
#define FARSI_LAYOUT_LETTER_NUM_T9     44
#define FARSI_LAYOUT_LETTER_NUM_QWERTY 61

class Farsi
{
public:
    Farsi();

    static _MultiTypeConfig     MultiType[FARSI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[FARSI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[FARSI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* FARSI_H_ */