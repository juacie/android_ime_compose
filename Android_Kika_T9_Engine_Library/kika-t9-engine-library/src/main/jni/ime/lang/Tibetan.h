#ifndef TIBETAN_H_
#define TIBETAN_H_

#include "maptable.h"

#define TIBETAN_MULTI_TYPE_SET_NUM       11
#define TIBETAN_LAYOUT_LETTER_NUM_T9     0
#define TIBETAN_LAYOUT_LETTER_NUM_QWERTY 62

class Tibetan
{
public:
    Tibetan();

    static _MultiTypeConfig     MultiType[TIBETAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[TIBETAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[TIBETAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* TIBETAN_H_ */