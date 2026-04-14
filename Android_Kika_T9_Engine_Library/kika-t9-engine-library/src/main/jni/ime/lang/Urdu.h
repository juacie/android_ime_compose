#ifndef URDU_H_
#define URDU_H_

#include "maptable.h"

#define URDU_MULTI_TYPE_SET_NUM       11
#define URDU_LAYOUT_LETTER_NUM_T9     54
#define URDU_LAYOUT_LETTER_NUM_QWERTY 52

class Urdu
{
public:
    Urdu();

    static _MultiTypeConfig     MultiType[URDU_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[URDU_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[URDU_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* URDU_H_ */
