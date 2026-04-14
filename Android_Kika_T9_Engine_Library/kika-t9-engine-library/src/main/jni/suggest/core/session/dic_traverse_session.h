/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LATINIME_DIC_TRAVERSE_SESSION_H
#define LATINIME_DIC_TRAVERSE_SESSION_H

#include <vector>

#include "defines.h"
#include "dictionary/utils/multi_bigram_map.h"
#include "suggest/core/dicnode/dic_nodes_cache.h"
#include "suggest/core/layout/proximity_info_state.h"
#include "utils/int_array_view.h"
#include "suggest/core/rnn/rnn_context.h"

namespace latinime {

class Dictionary;
class DictionaryStructureWithBufferPolicy;
class NgramContext;
class ProximityInfo;
class SuggestOptions;
class SuggestionResults;

class DicTraverseSession {
 public:

    // A factory method for DicTraverseSession
    static AK_FORCE_INLINE void *getSessionInstance(int dictSize) {
        // To deal with the trade-off between accuracy and memory space, large cache is used for
        // dictionaries larger that the threshold
        return new DicTraverseSession(
                dictSize >= DICTIONARY_SIZE_THRESHOLD_TO_USE_LARGE_CACHE_FOR_SUGGESTION);
    }

    static AK_FORCE_INLINE void releaseSessionInstance(DicTraverseSession *traverseSession) {
        delete traverseSession;
    }

    DicTraverseSession(bool usesLargeCache);

    // Non virtual inline destructor -- never inherit this class
    AK_FORCE_INLINE ~DicTraverseSession() {}

    void getSuggestions(ProximityInfo *proximityInfo, const Dictionary *dictionary,
            int *xcoordinates, int *ycoordinates, int *times, int *pointerIds, int *inputCodePoints,
            int inputSize, const NgramContext *const ngramContext,
            const SuggestOptions *const suggestOptions, const float weightOfLangModelVsSpatialModel,
            SuggestionResults *const outSuggestionResults);

    void init(const Dictionary *dictionary, const NgramContext *const ngramContext,
            const SuggestOptions *const suggestOptions);
    // TODO: Remove and merge into init
    void setupForGetSuggestions(const ProximityInfo *pInfo, const int *inputCodePoints,
            const int inputSize, const int *const inputXs, const int *const inputYs,
            const int *const times, const int *const pointerIds, const float maxSpatialDistance,
            const int maxPointerCount);
    void resetCache(const int thresholdForNextActiveDicNodes, const int maxWords);
    void resetCacheG(const int thresholdForNextActiveDicNodes, const int maxWords);

    const DictionaryStructureWithBufferPolicy *getDictionaryStructurePolicy() const;

    //--------------------
    // getters and setters
    //--------------------
    const ProximityInfo *getProximityInfo() const { return mProximityInfo; }
    const SuggestOptions *getSuggestOptions() const { return mSuggestOptions; }
    const WordIdArrayView getPrevWordIds() const {
        return WordIdArrayView::fromArray(mPrevWordIdArray).limit(mPrevWordIdCount);
    }
    DicNodesCache *getDicTraverseCache() { return &mDicNodesCache; }
    DicNodesCacheG *getDicTraverseCacheG() { return &mDicNodesCacheG; }
    MultiBigramMap *getMultiBigramMap() { return &mMultiBigramMap; }
    const ProximityInfoState *getProximityInfoState(int id) const {
        return &mProximityInfoStates[id];
    }
    int getInputSize() const { return mInputSize; }

    bool isOnlyOnePointerUsed(int *pointerId) const {
        // Not in the dictionary word
        int usedPointerCount = 0;
        int usedPointerId = 0;
        for (int i = 0; i < mMaxPointerCount; ++i) {
            if (mProximityInfoStates[i].isUsed()) {
                ++usedPointerCount;
                usedPointerId = i;
            }
        }
        if (usedPointerCount != 1) {
            return false;
        }
        if (pointerId) {
            *pointerId = usedPointerId;
        }
        return true;
    }

    ProximityType getProximityTypeG(const DicNode *const dicNode, const int childCodePoint) const {
        ProximityType proximityType = UNRELATED_CHAR;
        for (int i = 0; i < MAX_POINTER_COUNT_G; ++i) {
            if (!mProximityInfoStates[i].isUsed()) {
                continue;
            }
            const int pointerId = dicNode->getInputIndex(i);
            proximityType = mProximityInfoStates[i].getProximityTypeG(pointerId, childCodePoint);
            ASSERT(proximityType == UNRELATED_CHAR || proximityType == MATCH_CHAR);
            // TODO: Make this more generic
            // Currently we assume there are only two types here -- UNRELATED_CHAR
            // and MATCH_CHAR
            if (proximityType != UNRELATED_CHAR) {
                return proximityType;
            }
        }
        return proximityType;
    }

    AK_FORCE_INLINE bool isCacheBorderForTyping(const int inputSize) const {
        return mDicNodesCache.isCacheBorderForTyping(inputSize);
    }
    
    AK_FORCE_INLINE bool isCacheBorderForTypingG(const int inputSize) const {
        return mDicNodesCacheG.isCacheBorderForTyping(inputSize);
    }

    /**
     * Returns whether or not it is possible to continue suggestion from the previous search.
     */
    // TODO: Remove. No need to check once the session is fully implemented.
    bool isContinuousSuggestionPossible() const {
        if (!mDicNodesCache.hasCachedDicNodesForContinuousSuggestion()) {
            return false;
        }
        ASSERT(mMaxPointerCount <= MAX_POINTER_COUNT_G);
        for (int i = 0; i < mMaxPointerCount; ++i) {
            const ProximityInfoState *const pInfoState = getProximityInfoState(i);
            // If a proximity info state is not continuous suggestion possible,
            // do not continue searching.
            if (pInfoState->isUsed() && !pInfoState->isContinuousSuggestionPossible()) {
                return false;
            }
        }
        return true;
    }
    
    bool isContinuousSuggestionPossibleG() const {
        if (!mDicNodesCacheG.hasCachedDicNodesForContinuousSuggestion()) {
            return false;
        }
        ASSERT(mMaxPointerCount <= MAX_POINTER_COUNT_G);
        for (int i = 0; i < mMaxPointerCount; ++i) {
            const ProximityInfoState *const pInfoState = getProximityInfoState(i);
            // If a proximity info state is not continuous suggestion possible,
            // do not continue searching.
            if (pInfoState->isUsed() && !pInfoState->isContinuousSuggestionPossible()) {
                return false;
            }
        }
        return true;
    }

    bool isTouchPositionCorrectionEnabled() const {
        return mProximityInfoStates[0].touchPositionCorrectionEnabled();
    }

    float getMultiWordCostMultiplier() const {
        return mMultiWordCostMultiplier;
    }

    void setUseRNN(bool b) {
        mUseRNN = b;
    }

    bool useRNN() const {
        return mUseRNN;
    }

    RNNContext *getRNNContext() {
        return &mRnnContext;
    }

    const RNNContext *getRNNContext() const {
        return &mRnnContext;
    }

 private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(DicTraverseSession);

    // threshold to start caching
    static const int CACHE_START_INPUT_LENGTH_THRESHOLD;
    static const int DICTIONARY_SIZE_THRESHOLD_TO_USE_LARGE_CACHE_FOR_SUGGESTION;
    void initializeProximityInfoStates(const int *const inputCodePoints, const int *const inputXs,
            const int *const inputYs, const int *const times, const int *const pointerIds,
            const int inputSize, const float maxSpatialDistance, const int maxPointerCount);

    WordIdArray<MAX_PREV_WORD_COUNT_FOR_N_GRAM> mPrevWordIdArray;
    size_t mPrevWordIdCount;
    const ProximityInfo *mProximityInfo;
    const Dictionary *mDictionary;
    const SuggestOptions *mSuggestOptions;

    DicNodesCache mDicNodesCache;
    DicNodesCacheG mDicNodesCacheG;
    // Temporary cache for bigram frequencies
    MultiBigramMap mMultiBigramMap;
    ProximityInfoState mProximityInfoStates[MAX_POINTER_COUNT_G];

    int mInputSize;
    int mMaxPointerCount;

    /////////////////////////////////
    // Configuration per dictionary
    float mMultiWordCostMultiplier;

    RNNContext mRnnContext;

    bool mUseRNN = false;

};
} // namespace latinime
#endif // LATINIME_DIC_TRAVERSE_SESSION_H
