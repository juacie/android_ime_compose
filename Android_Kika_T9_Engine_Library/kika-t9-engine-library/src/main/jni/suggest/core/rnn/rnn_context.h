//
// Created by msj on 2017/5/10.
//

#ifndef RNN_CONTEXT_H
#define RNN_CONTEXT_H

#include "defines.h"

#include <unordered_map>

#include "dictionary/interface/dictionary_structure_with_buffer_policy.h"

namespace latinime {
class RNNContext {
 public:
    RNNContext(): mRNNWordCount(0) {}

    void setWordCount(int wordCount) {
        mRNNWordCount = wordCount;
    }

    int wordCount() const {
        return mRNNWordCount;
    }

    int *getWordCodePoints() {
        return (int *)mRNNWordCodePoints;
    }

    int *getNthWordCodePoints(int i) const {
        return (int *)(mRNNWordCodePoints[i]);
    }

    int *getWordCodePointCount() {
        return mRNNWordCodePointCount;
    }

    int getNthWordCodePointCount(int i) const {
        if(i >= mRNNWordCount) {
            return 0;
        }
        return mRNNWordCodePointCount[i];
    }

    int *getWordScore() {
        return mRNNWordScore;
    }

    int getNthWordScore(int i) const {
        if(i >= mRNNWordCount) {
            return NOT_A_PROBABILITY;
        }
        return mRNNWordScore[i];
    }

    void clear() {
        mRNNWordCount = 0;
    }

    void init(const DictionaryStructureWithBufferPolicy *const dictStructurePolicy);

    int calculateRNNScore (int wordId) const;

    std::unordered_map<int, int>::const_iterator cbegin() const {
        return mId2Index.begin();
    };

    std::unordered_map<int, int>::const_iterator cend() const {
        return mId2Index.end();
    };

 private:
    int mRNNWordCount;
    int mRNNWordCodePoints[MAX_WORD_COUNT_FOR_RNN][MAX_WORD_LENGTH];
    int mRNNWordCodePointCount[MAX_WORD_COUNT_FOR_RNN];
    int mRNNWordScore[MAX_WORD_COUNT_FOR_RNN];
    std::unordered_map<int, int> mId2Index;

    int getWordId(const DictionaryStructureWithBufferPolicy *const dictStructurePolicy,
                  const int *const wordCodePoints, const int wordCodePointCount);
};
} // namespace latinime

#endif //RNN_CONTEXT_H
