#include "stdafx.h"
#include "Chinese_HK.h"

_MultiTypeConfig Chinese_HK::MultiType[] =
{
    {0, 1, L"",  true},
    {1, 1, L"",  true},
    {2, 1, L"",  true},
    {3, 1, L"",  true},
    {4, 1, L"",  true},
    {5, 1, L"",  true},
    {6, 1, L"",  true},
    {7, 2, L"，\t：", true},
    {8, 2, L"。\t…", true},
    {9, 2, L"？\t！", true},
    {10, 42, L"，\t。\t？\t！\t：\t；\t、\t…\t“\t”\t‘\t’\t（\t）\t＠\t《\t》\t￥\t＄\t￡\t€\t＃\t％\t＆\t＊\t＋\t－\t＼\t＜\t＝\t＞\t＿\t／\t～\t＾\t｀\t｛\t｝\t【\t】\t「\t」", true}
};

_VirtualKeybdMapping Chinese_HK::VK_Layout_T9[] =
{
    { 60,  60, 100,  10, 0x4E00, 0x31, true, 110, 110},  //'1=一'

    {370,  60, 100,  10, 0x4E28, 0x32, true, 420, 110},  //'2=丨'

    {670,  60, 100,  10, 0x4E3F, 0x33, true, 720, 110},  //'3=丿'

    {190, 300, 100,  10, 0x4E36, 0x34, true, 240, 350},  //'4=丶'

    {550, 300, 100,  10, 0x4E5B, 0x35, true, 600, 350},  //'5=乛'

    {370,  60, 100,  10, 0x2A, 0x36, true, 420, 110}  //'6=*'

};

_VirtualKeybdMapping Chinese_HK::VK_Layout_Qwerty[] =
{
};
