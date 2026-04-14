#ifndef CHINESE_CN_H_
#define CHINESE_CN_H_

#include "maptable.h"

#define CHINESE_CN_MULTI_TYPE_SET_NUM       11
#define CHINESE_CN_LAYOUT_LETTER_NUM_T9     30
#define CHINESE_CN_LAYOUT_LETTER_NUM_QWERTY 27

class Chinese_CN
{
public:
    Chinese_CN();

    static _MultiTypeConfig     MultiType[CHINESE_CN_MULTI_TYPE_SET_NUM];
    static _VirtualKeybdMapping VK_Layout_T9[CHINESE_CN_LAYOUT_LETTER_NUM_T9];
    static _VirtualKeybdMapping VK_Layout_Qwerty[CHINESE_CN_LAYOUT_LETTER_NUM_QWERTY];
};

#endif /* CHINESE_CN_H_ */
