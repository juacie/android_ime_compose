#ifndef GERMAN_H_
#define GERMAN_H_

#include "maptable.h"

#define GERMAN_MULTI_TYPE_SET_NUM       11
#define GERMAN_LAYOUT_LETTER_NUM_T9     30
#define GERMAN_LAYOUT_LETTER_NUM_QWERTY 27

class German
{
public:
    German();

    static _MultiTypeConfig     MultiType[GERMAN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[GERMAN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[GERMAN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* GERMAN_H_ */
