#ifndef ROMANIAN_H_
#define ROMANIAN_H_

#include "maptable.h"

#define ROMANIAN_MULTI_TYPE_SET_NUM       11
#define ROMANIAN_LAYOUT_LETTER_NUM_T9     31
#define ROMANIAN_LAYOUT_LETTER_NUM_QWERTY 26

class Romanian
{
public:
    Romanian();

    static _MultiTypeConfig     MultiType[ROMANIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ROMANIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ROMANIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ROMANIAN_H_ */
