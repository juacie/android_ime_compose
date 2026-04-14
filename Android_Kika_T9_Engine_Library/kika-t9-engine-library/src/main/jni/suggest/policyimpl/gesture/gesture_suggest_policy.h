//
// Created by msj on 2017/3/9.
//

#ifndef GOOGLEKEYBOARDV7_GESTURE_SUGGEST_POLICY_H
#define GOOGLEKEYBOARDV7_GESTURE_SUGGEST_POLICY_H

#include "defines.h"
#include "suggest/core/policy/suggest_policy.h"
#include "suggest/policyimpl/gesture/gesture_scoring.h"
#include "suggest/policyimpl/gesture/gesture_traversal.h"
#include "suggest/policyimpl/gesture/gesture_weighting.h"

namespace latinime {

class GestureSuggestPolicy : public SuggestPolicy {
public:
    GestureSuggestPolicy() = default;
    virtual ~GestureSuggestPolicy() = default;

    AK_FORCE_INLINE const Traversal *getTraversal() const {
        return &traversal;
    }

    AK_FORCE_INLINE const Scoring *getScoring() const {
        return &scoring;
    }

    AK_FORCE_INLINE const Weighting *getWeighting() const {
        return &weighting;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(GestureSuggestPolicy);

    GestureTraversal traversal;
    GestureScoring scoring;
    GestureWeighting weighting;
};

} // namespace latinime

#endif //GOOGLEKEYBOARDV7_GESTURE_SUGGEST_POLICY_H
