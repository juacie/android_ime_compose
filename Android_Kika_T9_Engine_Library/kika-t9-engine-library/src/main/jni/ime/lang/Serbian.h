#ifndef SERBIAN_H_
#define SERBIAN_H_

#include "maptable.h"

#define SERBIAN_MULTI_TYPE_SET_NUM       11
#define SERBIAN_LAYOUT_LETTER_NUM_T9     36
#define SERBIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Serbian
{
public:
    Serbian();

    static _MultiTypeConfig     MultiType[SERBIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SERBIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SERBIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SERBIAN_H_ */
