#ifndef BASQUE_H_
#define BASQUE_H_

#include "maptable.h"

#define BASQUE_MULTI_TYPE_SET_NUM                 10
#define BASQUE_LAYOUT_LETTER_NUM_T9     28
#define BASQUE_LAYOUT_LETTER_NUM_QWERTY 26

class Basque
{
public:
    Basque();

    static _MultiTypeConfig     MultiType[BASQUE_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[BASQUE_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[BASQUE_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* BASQUE_H_ */