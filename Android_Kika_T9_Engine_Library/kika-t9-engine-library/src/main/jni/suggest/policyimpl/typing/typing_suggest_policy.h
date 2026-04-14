/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef LATINIME_TYPING_SUGGEST_POLICY_H
#define LATINIME_TYPING_SUGGEST_POLICY_H

#include "defines.h"
#include "suggest/core/policy/suggest_policy.h"
#include "suggest/policyimpl/typing/typing_scoring.h"
#include "suggest/policyimpl/typing/typing_traversal.h"
#include "suggest/policyimpl/typing/typing_weighting.h"

namespace latinime {

class TypingSuggestPolicy : public SuggestPolicy {
public:
    TypingSuggestPolicy() = default;
    virtual ~TypingSuggestPolicy() = default;

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
    DISALLOW_COPY_AND_ASSIGN(TypingSuggestPolicy);

    TypingTraversal traversal;
    TypingScoring scoring;
    TypingWeighting weighting;
};
} // namespace latinime
#endif // LATINIME_TYPING_SUGGEST_POLICY_H
