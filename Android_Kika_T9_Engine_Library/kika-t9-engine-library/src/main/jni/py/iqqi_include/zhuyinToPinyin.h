#include <cstdlib>

extern "C" {
    int zhuyinToPinyin(char* buf, size_t* splStart, size_t bufSize, const wchar_t* zhuyinStr);
}
