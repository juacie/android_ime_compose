#ifndef KAZAKH_H_
#define KAZAKH_H_

#include "maptable.h"

#define KAZAKH_MULTI_TYPE_SET_NUM       11
#define KAZAKH_LAYOUT_LETTER_NUM_T9     42
#define KAZAKH_LAYOUT_LETTER_NUM_QWERTY 31

class Kazakh
{
public:
    Kazakh();

    static _MultiTypeConfig     MultiType[KAZAKH_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[KAZAKH_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[KAZAKH_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* KAZAKH_H_ */
