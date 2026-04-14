#ifndef CHINESE_TW_H_
#define CHINESE_TW_H_

#include "maptable.h"

#define CHINESE_TW_MULTI_TYPE_SET_NUM       11
#define CHINESE_TW_LAYOUT_LETTER_NUM_T9     37
#define CHINESE_TW_LAYOUT_LETTER_NUM_QWERTY 37

class Chinese_TW
{
public:
    Chinese_TW();

    static _MultiTypeConfig     MultiType[CHINESE_TW_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[CHINESE_TW_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[CHINESE_TW_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* CHINESE_TW_H_ */
