#ifndef FINNISH_H_
#define FINNISH_H_

#include "maptable.h"

#define FINNISH_MULTI_TYPE_SET_NUM       11
#define FINNISH_LAYOUT_LETTER_NUM_T9     55
#define FINNISH_LAYOUT_LETTER_NUM_QWERTY 29

class Finnish
{
public:
    Finnish();

    static _MultiTypeConfig     MultiType[FINNISH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[FINNISH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[FINNISH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* FINNISH_H_ */
