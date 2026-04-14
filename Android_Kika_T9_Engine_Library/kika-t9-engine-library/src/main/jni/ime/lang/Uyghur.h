#ifndef UYGHUR_H_
#define UYGHUR_H_

#include "maptable.h"

#define UYGHUR_MULTI_TYPE_SET_NUM       11
#define UYGHUR_LAYOUT_LETTER_NUM_T9     0
#define UYGHUR_LAYOUT_LETTER_NUM_QWERTY 33

class Uyghur
{
public:
    Uyghur();

    static _MultiTypeConfig     MultiType[UYGHUR_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[UYGHUR_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[UYGHUR_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* UYGHUR_H_ */