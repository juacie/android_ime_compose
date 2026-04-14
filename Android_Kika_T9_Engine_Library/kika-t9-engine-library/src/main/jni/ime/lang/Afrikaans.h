#ifndef AFRIKAANS_H_
#define AFRIKAANS_H_

#include "maptable.h"

#define AFRIKAANS_MULTI_TYPE_SET_NUM       11
#define AFRIKAANS_LAYOUT_LETTER_NUM_T9     47
#define AFRIKAANS_LAYOUT_LETTER_NUM_QWERTY 0

class Afrikaans
{
public:
    Afrikaans();

    static _MultiTypeConfig     MultiType[AFRIKAANS_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[AFRIKAANS_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[AFRIKAANS_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* AFRIKAANS_H_ */
