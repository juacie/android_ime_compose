#include "kt9ToPinyin.h"
#include "pinyinime.h"


// TODO: check the following logic is correct...
bool isValid(const std::string& s, const char c) {
    switch(c) {
        case 'u':
            if (s.size() == 0)
                return false;
            switch(s.back()) {
                case 'e':
                case 'u':
                case 'a':
                case 'v':
                case '\'':
                    return false;
                case 'i': // qiu diu jiu liu xiu niu miu
                    if (s.size() < 2)
                        return false;
                    switch (s.at(s.size()-2)) {
                        case 'q':
                        case 'd':
                        case 'j':
                        case 'l':
                        case 'x':
                        case 'n':
                        case 'm':
                            break;
                        default:
                            return false;
                    }
                default:
                    break;
            }
            break;
        case 'i':
            if (s.size() == 0)
                return false;
            switch(s.back()) {
                case 'w':
                case 'i':
                case 'o':
                case 'f':
                case 'g':
                case 'k':
                case 'v':
                case '\'':
                    return false;
                case 'h': // zhi chi shi
                    if (s.size() < 2)
                        return false;
                    switch (s.at(s.size()-2)) {
                        case 's':
                        case 'z':
                        case 'c':
                            break;
                        default:
                            return false;
                    }
                    break;
                case 'u': // rui tui sui dui gui hui kui zui cui
                    if (s.size() < 2)
                        return false;
                    switch (s.at(s.size()-2)) {
                        case 'r':
                        case 't':
                        case 's':
                        case 'd':
                        case 'g':
                        case 'h':
                        case 'k':
                        case 'z':
                        case 'c':
                            break;
                        default:
                            return false;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 'v': // lv nv
            if (s.size() == 0)
                return false;
            switch(s.back()) {
                case 'l':
                case 'n':
                    break;
                default:
                    return false;
            }
            break;
        default:
            break;

    }

    return true;
}


bool isValid2(const std::string& s, size_t splnum, size_t chLimit)
{
    // must keep limit+2 to catch h o n(3) -> hong(1) cases
    // check: rong tong yong song dong gong hong kong long zong cong nong
    if (splnum == chLimit + 2 && s.length() >= 3 && s.at(s.length()-2) == 'o' && s.at(s.length()-1) == 'n') {
        switch(s.at(s.length()-3)) {
            case 'r':
            case 't':
            case 'y':
            case 's':
            case 'd':
            case 'g':
            case 'h':
            case 'k':
            case 'l':
            case 'z':
            case 'c':
            case 'n':
                return true;
            default:
                return false;
        }
    } else if (splnum == chLimit + 1) {
        return true;
    }

    return splnum <= chLimit;
}


size_t composingAdd(const std::vector<std::pair<std::string, int>>& composings,
                     std::vector<std::pair<std::string, int>>& nextComposings,
                     std::string& adds, size_t chLimit) {

    nextComposings.clear();

    if (composings.empty()) {
        for (const char c: adds) {
            if (!isValid("", c))
                continue;

            nextComposings.push_back(std::make_pair(std::string(1, c), 1));
        }
    } else {
        for(const std::pair<std::string, int>& composing: composings) {
            const std::string& s = composing.first;

            for (const char c: adds) {
                if (!isValid(s, c))
                    continue;

                // h o n(3) -> hong(1) cases
                // check: rong tong yong song dong gong hong kong long zong cong nong
                if (composing.second > chLimit + 1 && c != 'g')
                    continue;

                const std::string newComposing = s + c;
                size_t splnum = ime_pinyin::im_get_spl_id_num(newComposing.c_str());

                if (splnum == 0)
                    continue;

                const size_t newlength =  newComposing.size();

                if (!isValid2(newComposing, splnum, chLimit))
                    continue;
                nextComposings.push_back(std::make_pair(newComposing, splnum));
            }
        }
    }

    return nextComposings.size();
}

static std::vector<std::vector<std::pair<std::string, int>>> cachedT9Composings;
static size_t cachedT9ComposingsValidLength = 0;

static wchar_t cachedT9KeyCode[ime_pinyin::kMaxLemmaSize+1] = {0};
static size_t cachedT9KeyCodeLength = 0;

void resetT9Cache() {
    cachedT9Composings.clear();
    cachedT9Composings.resize(T9MaxInputSize+1);
    cachedT9ComposingsValidLength = 0;

    cachedT9KeyCode[0] = 0;
    cachedT9KeyCodeLength = 0;
}

/*
void collectPinyinComposings(std::vector<std::pair<std::string, int>>& pinyinComposings, size_t index) {
    // try to find valid Composings with the longest KeyCode
    // TODO: check the following approach is good enough to find candidates, or we need to rewrite it
    for(size_t i = index; i > 0; i--) {
        for(const std::pair<std::string, int>& composing: cachedT9Composings[i]) {

            if (composing.second > T9MaxChLimit)
                continue;

            pinyinComposings.push_back(composing);
        }

        if (pinyinComposings.size() > 0)
            break;
    }
}
 */

void collectPinyinComposings(std::vector<std::pair<std::string, int>>& pinyinComposings, size_t index) {
    for(const std::pair<std::string, int>& composing: cachedT9Composings[index]) {

        if (composing.second > T9MaxChLimit)
            continue;

        pinyinComposings.push_back(composing);
    }
}


int kt9ToPinyin(const wchar_t* keyCode, std::vector<std::pair<std::string, int>>& pinyinComposings) {
    pinyinComposings.clear();

    size_t length = wcslen(keyCode);

    if (length == 0 || length > T9MaxInputSize)
        return -1;

    if (length == cachedT9KeyCodeLength && wcsncmp(keyCode, cachedT9KeyCode, length) == 0) {
        collectPinyinComposings(pinyinComposings, cachedT9ComposingsValidLength);
        return 0;
    }

    size_t begin;

    if (cachedT9KeyCodeLength > 0 &&
        length > cachedT9KeyCodeLength && wcsncmp(keyCode, cachedT9KeyCode, cachedT9KeyCodeLength) == 0) {

        if (cachedT9ComposingsValidLength > 0 && cachedT9ComposingsValidLength < cachedT9KeyCodeLength) {
            // no more valid composing
            //collectPinyinComposings(pinyinComposings, cachedT9ComposingsValidLength);
            return 0;
        }

        begin = cachedT9KeyCodeLength;
    } else if (cachedT9KeyCodeLength > 0 &&
               length < cachedT9KeyCodeLength && wcsncmp(keyCode, cachedT9KeyCode, length) == 0) {

        if (cachedT9ComposingsValidLength > length) {
            collectPinyinComposings(pinyinComposings, length);
            return 0;
        }

        begin = length;
    } else {
        begin = 0;
        resetT9Cache();
    }

    // update cache
    wcsncpy(cachedT9KeyCode, keyCode, length);
    cachedT9KeyCode[length] = 0;
    cachedT9KeyCodeLength = length;

    for (size_t i = begin; i < length; i++) {
        std::string s;
        const wchar_t code = keyCode[i];
        switch(code) {
            case 49: // 1
            case 39: /* ' */
                if (i == 0 || (i > 0 && (keyCode[i-1] == 49 || keyCode[i-1] == 39))) {
                    resetT9Cache();
                    return -1;
                }

                s = std::string("'");
                break;
            case 50: // 2
                s = std::string("abc");
                break;
            case 51: // 3
                s = std::string("def");
                break;
            case 52: // 4
                s = std::string("ghi");
                break;
            case 53: // 5
                s = std::string("jkl");
                break;
            case 54: // 6
                s = std::string("mno");
                break;
            case 55: // 7
                s = std::string("pqrs");
                break;
            case 56: // 8
                s = std::string("tuv");
                break;
            case 57: // 9
                s = std::string("wxyz");
                break;
            default:
                if ((code >= 97 /* a */ && code <= 122 /* z */)) {
                    s = std::string(1, (char)code);
                } else {
                    resetT9Cache();
                    return -1;
                }
        }
        if (composingAdd(cachedT9Composings[i], cachedT9Composings[i+1], s, T9MaxChLimit) == 0)
            break;

        cachedT9ComposingsValidLength = i+1;
    }

    collectPinyinComposings(pinyinComposings, cachedT9ComposingsValidLength);

    return 0;
}

int kt9ToComposingDisplay(const wchar_t* keyCode, std::string& composing) {
    composing.clear();
    size_t length = wcslen(keyCode);
    
    for (size_t i = 0; i < length; i++) {
        std::string s;
        const wchar_t code = keyCode[i];
        switch(code) {
            case 49: // 1
            case 39: /* ' */
                if (composing.length() == 0 || (composing.back() == 39)) {
                    return -1;
                }
                composing.append(1, '\'');
                break;
            case 50: // 2
                composing.append(1, 'a');
                break;
            case 51: // 3
                composing.append(1, 'd');
                break;
            case 52: // 4
                composing.append(1, 'g');
                break;
            case 53: // 5
                composing.append(1, 'j');
                break;
            case 54: // 6
                composing.append(1, 'm');
                break;
            case 55: // 7
                composing.append(1, 'p');
                break;
            case 56: // 8
                composing.append(1, 't');
                break;
            case 57: // 9
                composing.append(1, 'w');
                break;
            default:
                if ((code >= 97 /* a */ && code <= 122 /* z */)) {
                    composing.append(1, (char)code);
                } else {
                    return -1;
                }
        }
    }
    
    return composing.length();
}


bool Kt9CandidateCompareMix(Kt9Candidate& a, Kt9Candidate& b) {
    if (a.mIsComplete != b.mIsComplete)
        return a.mIsComplete;
    if (a.mTotalScore != b.mTotalScore)
        return a.mTotalScore < b.mTotalScore;
    return a.mLength < b.mLength;
}

bool Kt9CandidateCompareNoComplete(Kt9Candidate& a, Kt9Candidate& b) {
    if (a.mIsComplete != b.mIsComplete)
        return a.mIsComplete;
    if (a.mTotalScore != b.mTotalScore)
        return a.mTotalScore < b.mTotalScore;
    return a.mLength > b.mLength;
}
