//
// Created by msj on 2017/5/11.
//
#include "suggest/core/rnn/rnn_utils.h"

#include <sstream>

#include "suggest/core/dictionary/error_type_utils.h"
#include "suggest/core/layout/touch_position_correction_utils.h"
#include "suggest/policyimpl/typing/scoring_params.h"
#include "dictionary/utils/probability_utils.h"
#include "dictionary/interface/dictionary_structure_with_buffer_policy.h"

namespace latinime {

bool isMaxCost(float cost) {
    return std::abs(cost - MAX_VALUE_FOR_WEIGHTING) < 0.0001;
}

/* static */
void RNNUtils::constructRNNContext(DicTraverseSession *session,
        const int * const *wordCodePointArrays, const int *wordCodePointLengths,
        const int *wordScoreArray, size_t wordCount) {

    int *wordCodePoints = session->getRNNContext()->getWordCodePoints();
    int *wordCodePointCount = session->getRNNContext()->getWordCodePointCount();
    int *wordScore = session->getRNNContext()->getWordScore();

    session->getRNNContext()->clear();
    for (size_t i = 0; i < wordCount; ++i) {
        wordCodePointCount[i] = 0;
        wordScore[i] = 0;
        const int *word = wordCodePointArrays[i];
        int wordLen = wordCodePointLengths[i];
        if (wordLen > MAX_WORD_LENGTH) {
            continue;
        }
        memcpy(wordCodePoints + (i * MAX_WORD_LENGTH), word, sizeof(int) * wordLen);
        wordCodePointCount[i] = wordLen;
        wordScore[i] = wordScoreArray[i];
    }
    session->getRNNContext()->setWordCount(wordCount);
    return;
}

/* static */ float RNNUtils::getRNNNodeImprobability(const DicTraverseSession *const session,
                                     const DicNode *const dicNode, MultiBigramMap *const multiBigramMap) {
    int rnnProbability = session->getRNNContext()->calculateRNNScore(dicNode->getWordId());
    const DictionaryStructureWithBufferPolicy *const dictionaryStructurePolicy = session->getDictionaryStructurePolicy();
    if (dicNode->hasMultipleWords() && !dicNode->isValidMultipleWordSuggestion()) {
        return static_cast<float>(MAX_VALUE_FOR_WEIGHTING);
    }
    const WordAttributes wordAttributes = dictionaryStructurePolicy->getWordAttributesInContext(
            WordIdArrayView(), dicNode->getWordId(), multiBigramMap);
    if (wordAttributes.getProbability() == NOT_A_PROBABILITY
        || (dicNode->hasMultipleWords()
            && (wordAttributes.isBlacklisted() || wordAttributes.isNotAWord()))) {
        return static_cast<float>(MAX_VALUE_FOR_WEIGHTING);
    }
    int probability = wordAttributes.getProbability();
    if(rnnProbability != NOT_A_PROBABILITY) {
        probability = ProbabilityUtils::computeProbabilityForBigram(probability,
                                                                    rnnProbability);
    }

    if(DEBUG_RNN && false) {
        RNNLOGI("mix probability: id %d unigram %d, rnn %d", dicNode->getWordId(), wordAttributes.getProbability(), rnnProbability);
    }

    const float cost = static_cast<float>(MAX_PROBABILITY - probability)
                       / static_cast<float>(MAX_PROBABILITY);
    return cost;
}

/* static */ void RNNUtils::outputSuggestionsOfRNN(const Scoring *const scoringPolicy,
                                                   DicTraverseSession * session,
                                                   const float weightOfLangModelVsSpatialModel,
                                                   SuggestionResults *const outSuggestionResults) {
    const RNNContext * rnnContext = session->getRNNContext();
    for(auto it = rnnContext->cbegin(); it != rnnContext->cend(); it++) {
        int index = it->second;
        float compoundDistance = getCompoundCost(session, weightOfLangModelVsSpatialModel, *it);
        if(!isMaxCost(compoundDistance)) {
            int finalScore = scoringPolicy->calculateFinalScore(
                    compoundDistance, session->getInputSize(),
                    ErrorTypeUtils::NOT_AN_ERROR,
                    false /*forceCommit*/, false /*boostExactMatches*/, false /*hasProbabilityZero*/);

            outSuggestionResults->addSuggestion(rnnContext->getNthWordCodePoints(index),
                                                rnnContext->getNthWordCodePointCount(index),
                                                finalScore, Dictionary::KIND_CORRECTION,
                                                NOT_AN_INDEX /*indexToPartialCommit*/, NOT_A_FIRST_WORD_CONFIDENCE/*autocimmitFirstWordConfindence*/);
        }
    }
}

/* static */ float RNNUtils::getCompoundCost(DicTraverseSession * session,
                                             const float weightOfLangModelVsSpatialModel,
                                             const std::pair<int, int> &it) {
    float spatialCost = getSpatialCost(session, it.second);
    if(isMaxCost(spatialCost)) {
        return MAX_VALUE_FOR_WEIGHTING;
    }
    float languageCost = getLanguageCost(session, it.first);

    if(DEBUG_RNN && true){
        auto tmpWord = session->getRNNContext()->getNthWordCodePoints(it.second);
        auto tmpCount = session->getRNNContext()->getNthWordCodePointCount(it.second);
        std::string word(tmpWord, tmpWord + tmpCount);
        RNNLOGI("merge %s spatial:%f, language:%f, compound: %f",
                word.c_str(),
                spatialCost,
                languageCost,
                spatialCost + languageCost * weightOfLangModelVsSpatialModel);
    }

    return spatialCost + languageCost * weightOfLangModelVsSpatialModel;
}

/* static */ float RNNUtils::getLanguageCost(DicTraverseSession * session, int wordId) {
    int rnnProbability = session->getRNNContext()->calculateRNNScore(wordId);
    const WordAttributes wordAttributes = session->getDictionaryStructurePolicy()->getWordAttributesInContext(
            WordIdArrayView(), wordId, 0/*multiBigramMap*/);
    if (wordAttributes.getProbability() == NOT_A_PROBABILITY) {
        return static_cast<float>(MAX_VALUE_FOR_WEIGHTING) * ScoringParams::DISTANCE_WEIGHT_LANGUAGE;
    }

    int probability = wordAttributes.getProbability();
    if(rnnProbability != NOT_A_PROBABILITY) {
        probability = ProbabilityUtils::computeProbabilityForBigram(probability,
                                                                    rnnProbability);
    }

    return static_cast<float>(MAX_PROBABILITY - probability)
           / static_cast<float>(MAX_PROBABILITY)
           * ScoringParams::DISTANCE_WEIGHT_LANGUAGE;
}

/* static */ float RNNUtils::getSpatialCost(DicTraverseSession * session, int index) {
    int *word = session->getRNNContext()->getNthWordCodePoints(index);
    int wordSize = session->getRNNContext()->getNthWordCodePointCount(index);
    int inputSize = session->getInputSize();

    float dpMatrix[inputSize + 1][wordSize + 1];
    int editCorrectionMatrix[inputSize + 1][wordSize + 1];
    int proximityMatrix[inputSize + 1][wordSize + 1];

    for(int i = 0; i < wordSize + 1; i++) {
        dpMatrix[0][i] = ScoringParams::OMISSION_COST * i;
        editCorrectionMatrix[0][i] = i;
        proximityMatrix[0][i] = 0;
    }


    for(int i = 1; i < inputSize + 1; i++) {
        dpMatrix[i][0] = MAX_VALUE_FOR_WEIGHTING;
        bool isCompletion = false;
        for(int j = 1; j < wordSize + 1; j++) {
            ProximityType matchType = MATCH_CHAR;
            float matchCost = dpMatrix[i - 1][j - 1];
            if(!isMaxCost(matchCost)) {
                matchCost += getMatchCost(session, i - 1, word, j - 1, matchType);
            }
            float insertCost = dpMatrix[i - 1][j];
            if(!isMaxCost(insertCost)) {
                insertCost += getInsertCost(session, i - 1, word, j - 1, wordSize);
            }
            float omissionCost = dpMatrix[i][j - 1];
            if(!isMaxCost(omissionCost)) {
                bool isPerfectMatch = proximityMatrix[i][j - 1] == 0 && editCorrectionMatrix[i][j - 1] == 0;
                omissionCost += getOmissionCost(session, i - 1, word, j - 1, wordSize, isCompletion, isPerfectMatch);
            }

            if(matchCost < insertCost && matchCost < omissionCost) {
                dpMatrix[i][j] = matchCost;
                proximityMatrix[i][j] = proximityMatrix[i - 1][j - 1];
                editCorrectionMatrix[i][j] = editCorrectionMatrix[i - 1][j - 1];
                switch (matchType) {
                    case PROXIMITY_CHAR:
                    case ADDITIONAL_PROXIMITY_CHAR:
                        proximityMatrix[i][j] = proximityMatrix[i][j] + 1;
                        break;
                    case SUBSTITUTION_CHAR:
                        editCorrectionMatrix[i][j] = editCorrectionMatrix[i][j] + 1;
                    default:
                        ;
                }
            } else if(insertCost < omissionCost) {
                dpMatrix[i][j] = insertCost;
                proximityMatrix[i][j] = proximityMatrix[i - 1][j];
                editCorrectionMatrix[i][j] = editCorrectionMatrix[i - 1][j] + 1;
            } else {
                dpMatrix[i][j] = omissionCost;
                proximityMatrix[i][j] = proximityMatrix[i][j - 1];
                editCorrectionMatrix[i][j] = editCorrectionMatrix[i][j - 1] + (isCompletion ? 0 : 1);
            }

            if(i == 1 && j == 1 && isMaxCost(matchCost)) {
                return MAX_VALUE_FOR_WEIGHTING;
            }
        }
    }

    if(DEBUG_RNN && false){
        for(int i = 0; i < inputSize + 1; i++) {
            std::stringstream ss;
            if(i != 0)
                ss << (char)session->getProximityInfoState(0)->getPrimaryOriginalCodePointAt(i - 1) << ": ";
            for(int j = 0; j < wordSize + 1; j++) {
                ss << dpMatrix[i][j] << " ";
            }
            RNNLOGI("%s", ss.str().c_str());
        }
        RNNLOGI("cost: %f, proximity: %d, edit: %d", dpMatrix[inputSize][wordSize],
               proximityMatrix[inputSize][wordSize],
               editCorrectionMatrix[inputSize][wordSize]
        );
    }

    float cost = dpMatrix[inputSize][wordSize];
    bool isNotMaxCost = !isMaxCost(cost);
    if(isNotMaxCost && editCorrectionMatrix[inputSize][wordSize] > 0) {
        cost += ScoringParams::HAS_EDIT_CORRECTION_TERMINAL_COST;
    }
    if(isNotMaxCost && proximityMatrix[inputSize][wordSize] > 0) {
        cost += ScoringParams::HAS_PROXIMITY_TERMINAL_COST;
        cost += ScoringParams::FIRST_PROXIMITY_COST;
    }
    return cost;
}

/* static */ float RNNUtils::getMatchCost(DicTraverseSession * session, int inputIndex, int* word, int wordIndex, ProximityType &errorType) {
    const ProximityType proximityType = session->getProximityInfoState(0)->getProximityType(
            inputIndex, word[wordIndex], true /* checkProximityChars */);
    errorType = proximityType;
    if(inputIndex == 0 && wordIndex == 0 && proximityType != PROXIMITY_CHAR && proximityType != MATCH_CHAR) {
        return MAX_VALUE_FOR_WEIGHTING;
    }
    float cost = 0.0;
    switch(proximityType) {
        case PROXIMITY_CHAR:
            cost += (inputIndex == 0 ? ScoringParams::FIRST_CHAR_PROXIMITY_COST : ScoringParams::PROXIMITY_COST);
            break;
        case SUBSTITUTION_CHAR:
            cost += ScoringParams::SUBSTITUTION_COST;
            break;
        case ADDITIONAL_PROXIMITY_CHAR:
            cost += ScoringParams::ADDITIONAL_PROXIMITY_COST;
            break;
        default:
            ;
    }

    const float normalizedSquaredLength = session->getProximityInfoState(0)->getPointToKeyLength(
            inputIndex, CharUtils::toBaseLowerCase(word[wordIndex]));
    const float normalizedDistance = TouchPositionCorrectionUtils::getSweetSpotFactor(
            session->isTouchPositionCorrectionEnabled(), normalizedSquaredLength);
    cost += ScoringParams::DISTANCE_WEIGHT_LENGTH * normalizedDistance;

    // todo: ScoringParams::FIRST_PROXIMITY_COST
    return cost;
}

/* static */ float RNNUtils::getInsertCost(DicTraverseSession * session, int inputIndex, int* word, int wordIndex, int wordSize) {

    if(inputIndex == 0 && wordIndex == 0) {
        return MAX_VALUE_FOR_WEIGHTING;
    }

    if(inputIndex == session->getInputSize() - 1 || wordIndex == wordSize - 1) {
        return MAX_VALUE_FOR_WEIGHTING;
    }

    const int currentCodePoint = session->getProximityInfoState(0)->getPrimaryCodePointAt(inputIndex);
    const int nextCodePoint = session->getProximityInfoState(0)->getPrimaryCodePointAt(inputIndex + 1);
    if(nextCodePoint != word[wordIndex + 1]) {
        return MAX_VALUE_FOR_WEIGHTING;
    }

    const bool sameCodePoint = currentCodePoint == nextCodePoint;
    const bool existsAdjacentProximityChars = session->getProximityInfoState(0)
            ->existsAdjacentProximityChars(inputIndex);

    const bool singleChar = wordSize == 1;
    float cost = (singleChar ? ScoringParams::INSERTION_COST_FIRST_CHAR : 0.0f);
    if (sameCodePoint) {
        cost += ScoringParams::INSERTION_COST_SAME_CHAR;
    } else if (existsAdjacentProximityChars) {
        cost += ScoringParams::INSERTION_COST_PROXIMITY_CHAR;
    } else {
        cost += ScoringParams::INSERTION_COST;
    }
    return cost;
}

/* static */ float RNNUtils::getOmissionCost(DicTraverseSession * session, int inputIndex, int* word, int wordIndex, int wordSize, bool &isCompletion, bool isPerfectMatch) {

    if(inputIndex == 0 && wordIndex == 0) {
        return MAX_VALUE_FOR_WEIGHTING;
    }
    if(inputIndex == session->getInputSize() - 1) {
        if(isCompletion) {
            //return ScoringParams::COST_COMPLETION;
            return 0;
        } else {
            isCompletion = true;
            return  isPerfectMatch ? 0 : ScoringParams::COST_FIRST_COMPLETION;
        }
    }
    if(wordIndex == wordSize - 1) {
        return MAX_VALUE_FOR_WEIGHTING;
    }
    const int codePoint = session->getProximityInfoState(0)->getPrimaryCodePointAt(inputIndex);
    const int currWordCode = word[wordIndex];
    const int nextWordCode = word[wordIndex + 1];
    if(codePoint != nextWordCode) {
        return MAX_VALUE_FOR_WEIGHTING;
    }
    if(wordIndex == 0) {
        return ScoringParams::OMISSION_COST_FIRST_CHAR;
    } else {
        return (currWordCode == nextWordCode) ? ScoringParams::OMISSION_COST_SAME_CHAR
                                                        : ScoringParams::OMISSION_COST;
    }
}

} // namespace laitnime
