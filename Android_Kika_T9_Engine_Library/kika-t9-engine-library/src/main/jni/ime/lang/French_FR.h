#ifndef FRENCH_FR_H_
#define FRENCH_FR_H_

#include "maptable.h"

#define FRENCH_FR_MULTI_TYPE_SET_NUM       11
#define FRENCH_FR_LAYOUT_LETTER_NUM_T9     43
#define FRENCH_FR_LAYOUT_LETTER_NUM_QWERTY 27

class French_FR
{
public:
    French_FR();

    static _MultiTypeConfig     MultiType[FRENCH_FR_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[FRENCH_FR_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[FRENCH_FR_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* FRENCH_FR_H_ */