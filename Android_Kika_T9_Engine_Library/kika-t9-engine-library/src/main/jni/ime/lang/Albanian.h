#ifndef ALBANIAN_H_
#define ALBANIAN_H_

#include "maptable.h"

#define ALBANIAN_MULTI_TYPE_SET_NUM       11
#define ALBANIAN_LAYOUT_LETTER_NUM_T9     28
#define ALBANIAN_LAYOUT_LETTER_NUM_QWERTY 27

class Albanian
{
public:
    Albanian();

    static _MultiTypeConfig     MultiType[ALBANIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ALBANIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ALBANIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ALBANIAN_H_ */
