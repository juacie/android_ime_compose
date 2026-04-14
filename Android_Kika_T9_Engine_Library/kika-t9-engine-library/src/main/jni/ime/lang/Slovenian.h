#ifndef SLOVENIAN_H_
#define SLOVENIAN_H_

#include "maptable.h"

#define SLOVENIAN_MULTI_TYPE_SET_NUM       11
#define SLOVENIAN_LAYOUT_LETTER_NUM_T9     29
#define SLOVENIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Slovenian
{
public:
    Slovenian();

    static _MultiTypeConfig     MultiType[SLOVENIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SLOVENIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SLOVENIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SLOVENIAN_H_ */
