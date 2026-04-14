#include <string>
#include <vector>
#include <utility>
#include <cstdlib>

const size_t T9MaxInputSize = 20;
const size_t T9MaxChLimit = 4;

int kt9ToPinyin(const wchar_t* keyCode, std::vector<std::pair<std::string, int>>& pinyinComposings);
int kt9ToComposingDisplay(const wchar_t* keyCode, std::string& composing);


class Kt9Candidate {
public:
    Kt9Candidate(const std::wstring& candidate, size_t id, size_t length, size_t score):
            mCandidate(candidate), mId(id), mLength(length), mScore(score), mIsComplete(false) {
        mTotalScore = score*length;
    }


    std::wstring mCandidate;
    std::string mComposing;
    size_t mId;
    size_t mLength;
    size_t mScore;
    size_t mTotalScore;
    bool mIsComplete;
};


bool Kt9CandidateCompareMix(Kt9Candidate& a, Kt9Candidate& b);
bool Kt9CandidateCompareNoComplete(Kt9Candidate& a, Kt9Candidate& b);
