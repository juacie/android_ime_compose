#ifndef KHMER_H_
#define KHMER_H_

#include "maptable.h"

#define KHMER_MULTI_TYPE_SET_NUM       11
#define KHMER_LAYOUT_LETTER_NUM_T9     81
#define KHMER_LAYOUT_LETTER_NUM_QWERTY 88

class Khmer
{
public:
    Khmer();

    static _MultiTypeConfig     MultiType[KHMER_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[KHMER_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[KHMER_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* KHMER_H_ */
