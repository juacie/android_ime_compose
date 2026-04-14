#ifndef XHOSA_H_
#define XHOSA_H_

#include "maptable.h"

#define XHOSA_MULTI_TYPE_SET_NUM       11
#define XHOSA_LAYOUT_LETTER_NUM_T9     54
#define XHOSA_LAYOUT_LETTER_NUM_QWERTY 27

class Xhosa
{
public:
    Xhosa();

    static _MultiTypeConfig     MultiType[XHOSA_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[XHOSA_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[XHOSA_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* XHOSA_H_ */
