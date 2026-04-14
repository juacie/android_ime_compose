//
// Created by msj on 2017/3/9.
//

#include "suggest/policyimpl/gesture/gesture_suggest_policy.h"
#include "suggest/policyimpl/gesture/gesture_suggest_policy_factory.h"

namespace latinime {

const SuggestPolicy* GestureSuggestPolicyFactory::getGestureSuggestPolicy() {
    static GestureSuggestPolicy instance;
    return &instance;
}

} // namespace latinime

