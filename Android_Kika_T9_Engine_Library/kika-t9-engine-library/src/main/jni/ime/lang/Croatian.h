#ifndef CROATIAN_H_
#define CROATIAN_H_

#include "maptable.h"

#define CROATIAN_MULTI_TYPE_SET_NUM       11
#define CROATIAN_LAYOUT_LETTER_NUM_T9     36
#define CROATIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Croatian
{
public:
    Croatian();

    static _MultiTypeConfig     MultiType[CROATIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[CROATIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[CROATIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* CROATIAN_H_ */
