#ifndef SWAHILI_H_
#define SWAHILI_H_

#include "maptable.h"

#define SWAHILI_MULTI_TYPE_SET_NUM       11
#define SWAHILI_LAYOUT_LETTER_NUM_T9     54
#define SWAHILI_LAYOUT_LETTER_NUM_QWERTY 26

class Swahili
{
public:
    Swahili();

    static _MultiTypeConfig     MultiType[SWAHILI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SWAHILI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SWAHILI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SWAHILI_H_ */
