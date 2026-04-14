#ifndef BELARUSIAN_H_
#define BELARUSIAN_H_

#include "maptable.h"

#define BELARUSIAN_MULTI_TYPE_SET_NUM       11
#define BELARUSIAN_LAYOUT_LETTER_NUM_T9     32
#define BELARUSIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Belarusian
{
public:
    Belarusian();

    static _MultiTypeConfig     MultiType[BELARUSIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[BELARUSIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[BELARUSIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* BELARUSIAN_H_ */
