#ifndef CATALAN_H_
#define CATALAN_H_

#include "maptable.h"

#define CATALAN_MULTI_TYPE_SET_NUM       10
#define CATALAN_LAYOUT_LETTER_NUM_T9     38
#define CATALAN_LAYOUT_LETTER_NUM_QWERTY 27

class Catalan
{
public:
    Catalan();

    static _MultiTypeConfig     MultiType[CATALAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[CATALAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[CATALAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* CATALAN_H_ */