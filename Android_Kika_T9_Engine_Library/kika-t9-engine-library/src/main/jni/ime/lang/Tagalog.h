#ifndef TAGALOG_H_
#define TAGALOG_H_

#include "maptable.h"

#define TAGALOG_MULTI_TYPE_SET_NUM       11
#define TAGALOG_LAYOUT_LETTER_NUM_T9     54
#define TAGALOG_LAYOUT_LETTER_NUM_QWERTY 26

class Tagalog
{
public:
    Tagalog();

    static _MultiTypeConfig     MultiType[TAGALOG_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[TAGALOG_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[TAGALOG_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* TAGALOG_H_ */
