#ifndef ITALIAN_H_
#define ITALIAN_H_

#include "maptable.h"

#define ITALIAN_MULTI_TYPE_SET_NUM       11
#define ITALIAN_LAYOUT_LETTER_NUM_T9     32
#define ITALIAN_LAYOUT_LETTER_NUM_QWERTY 27

class Italian
{
public:
    Italian();

    static _MultiTypeConfig     MultiType[ITALIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ITALIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ITALIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ITALIAN_H_ */
