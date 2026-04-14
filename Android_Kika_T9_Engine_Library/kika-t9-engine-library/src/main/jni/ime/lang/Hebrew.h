#ifndef HEBREW_H_
#define HEBREW_H_

#include "maptable.h"

#define HEBREW_MULTI_TYPE_SET_NUM       11
#define HEBREW_LAYOUT_LETTER_NUM_T9     27
#define HEBREW_LAYOUT_LETTER_NUM_QWERTY 28

class Hebrew
{
public:
    Hebrew();

    static _MultiTypeConfig     MultiType[HEBREW_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[HEBREW_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[HEBREW_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* HEBREW_H_ */
