#ifndef ARABIC_H_
#define ARABIC_H_

#include "maptable.h"

#define ARABIC_MULTI_TYPE_SET_NUM       11
#define ARABIC_LAYOUT_LETTER_NUM_T9     36
#define ARABIC_LAYOUT_LETTER_NUM_QWERTY 27

class Arabic
{
public:
    Arabic();

    static _MultiTypeConfig     MultiType[ARABIC_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ARABIC_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ARABIC_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ARABIC_H_ */
