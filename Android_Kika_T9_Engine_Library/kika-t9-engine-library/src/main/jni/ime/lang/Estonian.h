#ifndef ESTONIAN_H_
#define ESTONIAN_H_

#include "maptable.h"

#define ESTONIAN_MULTI_TYPE_SET_NUM       11
#define ESTONIAN_LAYOUT_LETTER_NUM_T9     26
#define ESTONIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Estonian
{
public:
    Estonian();

    static _MultiTypeConfig     MultiType[ESTONIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ESTONIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ESTONIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ESTONIAN_H_ */
