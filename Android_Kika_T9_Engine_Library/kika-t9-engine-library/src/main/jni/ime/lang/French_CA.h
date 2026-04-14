#ifndef FRENCH_CA_H_
#define FRENCH_CA_H_

#include "maptable.h"

#define FRENCH_CA_MULTI_TYPE_SET_NUM       11
#define FRENCH_CA_LAYOUT_LETTER_NUM_T9     43
#define FRENCH_CA_LAYOUT_LETTER_NUM_QWERTY 27

class French_CA
{
public:
    French_CA();

    static _MultiTypeConfig     MultiType[FRENCH_CA_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[FRENCH_CA_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[FRENCH_CA_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* FRENCH_CA_H_ */