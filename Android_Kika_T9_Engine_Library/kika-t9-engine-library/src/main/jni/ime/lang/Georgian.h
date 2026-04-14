#ifndef GEORGIAN_H_
#define GEORGIAN_H_

#include "maptable.h"

#define GEORGIAN_MULTI_TYPE_SET_NUM       11
#define GEORGIAN_LAYOUT_LETTER_NUM_T9     33
#define GEORGIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Georgian
{
public:
    Georgian();

    static _MultiTypeConfig     MultiType[GEORGIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[GEORGIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[GEORGIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* GEORGIAN_H_ */
