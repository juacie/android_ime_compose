#include "pinyinime.h"


namespace pinyinimehandler {
    
class PinyinIMEHandler {

public:
    static PinyinIMEHandler* getHandler() { return &handler; }
    bool init(const char *dictPath, const char* userDictPath);
    bool isInit() { return isInited; }
    
private:
    PinyinIMEHandler() { isInited = false; }
    
    static PinyinIMEHandler handler;
    static bool isInited;
};
    
}

