#ifndef MALAY_H_
#define MALAY_H_

#include "maptable.h"

#define MALAY_MULTI_TYPE_SET_NUM       11
#define MALAY_LAYOUT_LETTER_NUM_T9     54
#define MALAY_LAYOUT_LETTER_NUM_QWERTY 26

class Malay
{
public:
    Malay();

    static _MultiTypeConfig     MultiType[MALAY_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[MALAY_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[MALAY_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* MALAY_H_ */
