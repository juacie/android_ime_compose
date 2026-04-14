#ifndef CHANGJEI_H_
#define CHANGJEI_H_

#include "maptable.h"

#define CHANGJEI_MULTI_TYPE_SET_NUM       11
#define CHANGJEI_LAYOUT_LETTER_NUM_T9     26
#define CHANGJEI_LAYOUT_LETTER_NUM_QWERTY 0

class Changjie
{
public:
    Changjie();

    static _MultiTypeConfig     MultiType[CHANGJEI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[CHANGJEI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[CHANGJEI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* CHANGJEI_H_ */
