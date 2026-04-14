#ifndef BOSNIAN_H_
#define BOSNIAN_H_

#include "maptable.h"

#define BOSNIAN_MULTI_TYPE_SET_NUM       11
#define BOSNIAN_LAYOUT_LETTER_NUM_T9     36
#define BOSNIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Bosnian
{
public:
    Bosnian();

    static _MultiTypeConfig     MultiType[BOSNIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[BOSNIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[BOSNIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* BOSNIAN_H_ */
