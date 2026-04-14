#ifndef PHILIPPINES_H_
#define PHILIPPINES_H_

#include "maptable.h"

#define PHILIPPINES_MULTI_TYPE_SET_NUM       11
#define PHILIPPINES_LAYOUT_LETTER_NUM_T9     55
#define PHILIPPINES_LAYOUT_LETTER_NUM_QWERTY 55

class Philippines
{
public:
    Philippines();

    static _MultiTypeConfig     MultiType[PHILIPPINES_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[PHILIPPINES_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[PHILIPPINES_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* PHILIPPINES_H_ */

