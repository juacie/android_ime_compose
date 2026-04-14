#ifndef ASSAMESE_H_
#define ASSAMESE_H_

#include "maptable.h"

#define ASSAMESE_MULTI_TYPE_SET_NUM       11
#define ASSAMESE_LAYOUT_LETTER_NUM_T9     77
#define ASSAMESE_LAYOUT_LETTER_NUM_QWERTY 73

class Assamese
{
public:
    Assamese();

    static _MultiTypeConfig     MultiType[ASSAMESE_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[ASSAMESE_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[ASSAMESE_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* ASSAMESE_H_ */