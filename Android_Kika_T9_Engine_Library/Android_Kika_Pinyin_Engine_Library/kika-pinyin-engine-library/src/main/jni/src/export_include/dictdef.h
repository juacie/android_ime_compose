/*
 * Copyright (C) 2009 The Android Open Source Project
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

#ifndef PINYINIME_INCLUDE_DICTDEF_H__
#define PINYINIME_INCLUDE_DICTDEF_H__

#include <stdlib.h>

namespace ime_pinyin {

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;

typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned long long uint64;

typedef unsigned short char16;

// The max length of a lemma.
const size_t kMaxLemmaSize = 96;

// The max length of a Pinyin (spelling).
const size_t kMaxPinyinSize = 7;
    
const size_t kMaxSentenceLength = 16;

}  //  namespace ime_pinyin

#endif  // PINYINIME_INCLUDE_DICTDEF_H__
