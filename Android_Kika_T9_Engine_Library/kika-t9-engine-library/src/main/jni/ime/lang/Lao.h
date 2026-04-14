#ifndef LAO_H_
#define LAO_H_

#include "maptable.h"

#define LAO_MULTI_TYPE_SET_NUM       11
#define LAO_LAYOUT_LETTER_NUM_T9     55
#define LAO_LAYOUT_LETTER_NUM_QWERTY 89

class Lao
{
public:
    Lao();

    static _MultiTypeConfig     MultiType[LAO_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[LAO_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[LAO_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* LAO_H_ */
