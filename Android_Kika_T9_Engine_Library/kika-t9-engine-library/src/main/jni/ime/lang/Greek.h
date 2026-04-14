#ifndef GREEK_H_
#define GREEK_H_

#include "maptable.h"

#define GREEK_MULTI_TYPE_SET_NUM       11
#define GREEK_LAYOUT_LETTER_NUM_T9     36
#define GREEK_LAYOUT_LETTER_NUM_QWERTY 25

class Greek
{
public:
    Greek();

    static _MultiTypeConfig     MultiType[GREEK_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[GREEK_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[GREEK_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* GREEK_H_ */
