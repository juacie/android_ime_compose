#ifndef MACEDONIAN_H_
#define MACEDONIAN_H_

#include "maptable.h"

#define MACEDONIAN_MULTI_TYPE_SET_NUM       11
#define MACEDONIAN_LAYOUT_LETTER_NUM_T9     33
#define MACEDONIAN_LAYOUT_LETTER_NUM_QWERTY 31

class Macedonian
{
public:
    Macedonian();

    static _MultiTypeConfig     MultiType[MACEDONIAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[MACEDONIAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[MACEDONIAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* MACEDONIAN_H_ */
