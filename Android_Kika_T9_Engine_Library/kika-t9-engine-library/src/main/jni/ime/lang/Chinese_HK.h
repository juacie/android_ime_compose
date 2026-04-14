#ifndef CHINESE_HK_H_
#define CHINESE_HK_H_

#include "maptable.h"

#define CHINESE_HK_MULTI_TYPE_SET_NUM       11
#define CHINESE_HK_LAYOUT_LETTER_NUM_T9     6
#define CHINESE_HK_LAYOUT_LETTER_NUM_QWERTY 27

class Chinese_HK
{
public:
    Chinese_HK();

    static _MultiTypeConfig     MultiType[CHINESE_HK_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[CHINESE_HK_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[CHINESE_HK_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* CHINESE_HK_H_ */
