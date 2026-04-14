#ifndef VIETNAMESE_H_
#define VIETNAMESE_H_

#include "maptable.h"

#define VIETNAMESE_MULTI_TYPE_SET_NUM       11
#define VIETNAMESE_LAYOUT_LETTER_NUM_T9     93
#define VIETNAMESE_LAYOUT_LETTER_NUM_QWERTY 26

class Vietnamese
{
public:
    Vietnamese();

    static _MultiTypeConfig     MultiType[VIETNAMESE_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[VIETNAMESE_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[VIETNAMESE_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* VIETNAMESE_H_ */
