#ifndef SINHALA_H_
#define SINHALA_H_

#include "maptable.h"

#define SINHALA_MULTI_TYPE_SET_NUM       11
#define SINHALA_LAYOUT_LETTER_NUM_T9     51
#define SINHALA_LAYOUT_LETTER_NUM_QWERTY 62

class Sinhala
{
public:
    Sinhala();

    static _MultiTypeConfig     MultiType[SINHALA_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[SINHALA_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[SINHALA_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* SINHALA_H_ */
