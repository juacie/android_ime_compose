#ifndef TURKISH_H_
#define TURKISH_H_

#include "maptable.h"

#define TURKISH_MULTI_TYPE_SET_NUM       11
#define TURKISH_LAYOUT_LETTER_NUM_T9     37
#define TURKISH_LAYOUT_LETTER_NUM_QWERTY 26

class Turkish
{
public:
    Turkish();

    static _MultiTypeConfig     MultiType[TURKISH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[TURKISH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[TURKISH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* TURKISH_H_ */
