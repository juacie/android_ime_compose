#ifndef ARMENIAN_H_
#define ARMENIAN_H_

#include "maptable.h"

#define ARMENIAN_MULTI_TYPE_SET_NUM       11
#define ARMENIAN_LAYOUT_LETTER_NUM_T9     65
#define ARMENIAN_LAYOUT_LETTER_NUM_QWERTY 27

class Armenian
{
public:
    Armenian();

    static _MultiTypeConfig     MultiType[ARMENIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ARMENIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ARMENIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ARMENIAN_H_ */
