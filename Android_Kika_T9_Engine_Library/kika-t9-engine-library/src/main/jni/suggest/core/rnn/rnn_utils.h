//
// Created by msj on 2017/5/11.
//

#ifndef RNN_UTILS_H
#define RNN_UTILS_H

#include "suggest/core/session/dic_traverse_session.h"
#include "suggest/core/result/suggestion_results.h"
#include "suggest/core/policy/scoring.h"

namespace latinime {

class RNNUtils {
 public:
    static void outputSuggestionsOfRNN(const Scoring *const scoringPolicy,
                                       DicTraverseSession * session,
                                       const float weightOfLangModelVsSpatialModel,
                                       SuggestionResults *const outSuggestionResults);

    static void constructRNNContext(DicTraverseSession *session, const int * const *wordCodePointArrays,
                                    const int *wordCodePointLengths, const int *wordScoreArray,
                                    size_t wordCount);

    static float getRNNNodeImprobability(const DicTraverseSession *const session,
            const DicNode *const dicNode, MultiBigramMap *const multiBigramMap);

 private:
    static float getCompoundCost(DicTraverseSession * session,
                                 const float weightOfLangModelVsSpatialModel,
                                 const std::pair<int, int> &it);
    static float getSpatialCost(DicTraverseSession * session, int index);
    static float getLanguageCost(DicTraverseSession * session, int wordId);
    static float getMatchCost(DicTraverseSession * session, int inputIndex, int* word, int wordIndex, ProximityType &errorType);
    static float getInsertCost(DicTraverseSession * session, int inputIndex, int* word, int wordIndex, int wordSize);
    static float getOmissionCost(DicTraverseSession * session, int inputIndex, int* word, int wordIndex, int wordSize, bool &isCompletion, bool isPerfectMatch);
};

} // namespace latinime

#endif //RNN_UTILS_H
