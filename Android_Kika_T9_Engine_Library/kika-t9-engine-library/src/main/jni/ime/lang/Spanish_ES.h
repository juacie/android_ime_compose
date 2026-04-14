#ifndef SPANISH_ES_H_
#define SPANISH_ES_H_

#include "maptable.h"

#define SPANISH_ES_MULTI_TYPE_SET_NUM       11
#define SPANISH_ES_LAYOUT_LETTER_NUM_T9     34
#define SPANISH_ES_LAYOUT_LETTER_NUM_QWERTY 26

class Spanish_ES
{
public:
    Spanish_ES();

    static _MultiTypeConfig     MultiType[SPANISH_ES_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SPANISH_ES_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SPANISH_ES_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SPANISH_ES_H_ */

