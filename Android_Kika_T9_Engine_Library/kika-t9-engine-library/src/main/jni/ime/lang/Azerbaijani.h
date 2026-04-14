#ifndef AZERBAIJANI_H_
#define AZERBAIJANI_H_

#include "maptable.h"

#define AZERBAIJANI_MULTI_TYPE_SET_NUM       11
#define AZERBAIJANI_LAYOUT_LETTER_NUM_T9     33
#define AZERBAIJANI_LAYOUT_LETTER_NUM_QWERTY 32

class Azerbaijani
{
public:
    Azerbaijani();

    static _MultiTypeConfig     MultiType[AZERBAIJANI_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[AZERBAIJANI_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[AZERBAIJANI_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* AZERBAIJANI_H_ */
