#ifndef UZBEK_CYRILLIC_H_
#define UZBEK_CYRILLIC_H_

#include "maptable.h"

#define UZBEK_CYRILLIC_MULTI_TYPE_SET_NUM       11
#define UZBEK_CYRILLIC_LAYOUT_LETTER_NUM_T9     26
#define UZBEK_CYRILLIC_LAYOUT_LETTER_NUM_QWERTY 26

class Uzbek_Cyrillic
{
public:
    Uzbek_Cyrillic();

    static _MultiTypeConfig     MultiType[UZBEK_CYRILLIC_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[UZBEK_CYRILLIC_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[UZBEK_CYRILLIC_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* UZBEK_CYRILLIC_H_ */