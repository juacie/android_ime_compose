#ifndef BURMESE_U_H_
#define BURMESE_U_H_

#include "maptable.h"

#define BURMESE_U_MULTI_TYPE_SET_NUM       11
#define BURMESE_U_LAYOUT_LETTER_NUM_T9     94
#define BURMESE_U_LAYOUT_LETTER_NUM_QWERTY 73

class Burmese_U
{
public:
    Burmese_U();

    static _MultiTypeConfig     MultiType[BURMESE_U_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[BURMESE_U_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[BURMESE_U_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* BURMESE_U_H_ */