#ifndef GUJARATI_H_
#define GUJARATI_H_

#include "maptable.h"

#define GUJARATI_MULTI_TYPE_SET_NUM       11
#define GUJARATI_LAYOUT_LETTER_NUM_T9     73
#define GUJARATI_LAYOUT_LETTER_NUM_QWERTY 71

class Gujarati
{
public:
    Gujarati();

    static _MultiTypeConfig     MultiType[GUJARATI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[GUJARATI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[GUJARATI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* GUJARATI_H_ */