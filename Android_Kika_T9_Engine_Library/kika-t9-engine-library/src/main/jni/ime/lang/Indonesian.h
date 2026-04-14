#ifndef INDONESIAN_H_
#define INDONESIAN_H_

#include "maptable.h"

#define INDONESIAN_MULTI_TYPE_SET_NUM       11
#define INDONESIAN_LAYOUT_LETTER_NUM_T9     26
#define INDONESIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Indonesian
{
public:
    Indonesian();

    static _MultiTypeConfig     MultiType[INDONESIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[INDONESIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[INDONESIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* INDONESIAN_H_ */
