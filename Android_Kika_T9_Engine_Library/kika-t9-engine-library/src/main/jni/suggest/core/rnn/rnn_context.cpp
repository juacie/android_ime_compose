//
// Created by msj on 2017/5/24.
//
#include "suggest/core/rnn/rnn_context.h"

namespace latinime {

int RNNContext::getWordId(
        const DictionaryStructureWithBufferPolicy *const dictStructurePolicy,
        const int *const wordCodePoints, const int wordCodePointCount) {
    if (!dictStructurePolicy || !wordCodePoints || wordCodePointCount > MAX_WORD_LENGTH) {
        return NOT_A_WORD_ID;
    }
    int codePoints[MAX_WORD_LENGTH];
    int codePointCount = wordCodePointCount;
    memmove(codePoints, wordCodePoints, sizeof(int) * codePointCount);

    const CodePointArrayView codePointArrayView(codePoints, codePointCount);
    const int wordId = dictStructurePolicy->getWordId(codePointArrayView,
                                                      false /* forceLowerCaseSearch */);
    if (wordId != NOT_A_WORD_ID) {
        // Return the id when when the word was found or doesn't try lower case search.
        return wordId;
    }
    // Check bigrams for lower-cased previous word if original was not found. Useful for
    // auto-capitalized words like "The [current_word]".
    return dictStructurePolicy->getWordId(codePointArrayView, true /* forceLowerCaseSearch */);
}

void RNNContext::init(const DictionaryStructureWithBufferPolicy *const dictStructurePolicy) {
    mId2Index.clear();
    for(int i = 0; i < mRNNWordCount; i++) {
        int id = getWordId(dictStructurePolicy, mRNNWordCodePoints[i], mRNNWordCodePointCount[i]);
        if(id == NOT_A_WORD_ID) {
            continue;
        }
        mId2Index[id] = i;
        if(DEBUG_RNN && false){
            AKLOGI("Native rnn %d id %d, socre %d", i, id, mRNNWordScore[i]);
        }
    }
}

int RNNContext::calculateRNNScore (int wordId) const {
    auto it = mId2Index.find(wordId);
    if(it == mId2Index.end()) {
        return NOT_A_PROBABILITY;
    }
    return mRNNWordScore[it->second];
}

} // namespace latinime

