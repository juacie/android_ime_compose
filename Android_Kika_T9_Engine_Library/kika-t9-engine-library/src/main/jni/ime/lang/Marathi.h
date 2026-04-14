#ifndef MARATHI_H_
#define MARATHI_H_

#include "maptable.h"

#define MARATHI_MULTI_TYPE_SET_NUM       11
#define MARATHI_LAYOUT_LETTER_NUM_T9     95
#define MARATHI_LAYOUT_LETTER_NUM_QWERTY 60

class Marathi
{
public:
    Marathi();

    static _MultiTypeConfig     MultiType[MARATHI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[MARATHI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[MARATHI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* MARATHI_H_ */