#ifndef RUSSIAN_H_
#define RUSSIAN_H_

#include "maptable.h"

#define RUSSIAN_MULTI_TYPE_SET_NUM       11
#define RUSSIAN_LAYOUT_LETTER_NUM_T9     42
#define RUSSIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Russian
{
public:
    Russian();

    static _MultiTypeConfig     MultiType[RUSSIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[RUSSIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[RUSSIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* RUSSIAN_H_ */