#ifndef ZULU_H_
#define ZULU_H_

#include "maptable.h"

#define ZULU_MULTI_TYPE_SET_NUM       11
#define ZULU_LAYOUT_LETTER_NUM_T9     54
#define ZULU_LAYOUT_LETTER_NUM_QWERTY 27

class Zulu
{
public:
    Zulu();

    static _MultiTypeConfig     MultiType[ZULU_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ZULU_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ZULU_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ZULU_H_ */
