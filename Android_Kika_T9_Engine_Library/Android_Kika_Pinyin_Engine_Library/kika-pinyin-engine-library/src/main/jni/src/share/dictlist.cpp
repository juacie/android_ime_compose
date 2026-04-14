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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dictlist.h"
#include "../include/mystdlib.h"
#include "../include/ngram.h"
#include "../include/searchutility.h"

namespace ime_pinyin {

bool spl_homo_enabled = false;

DictList::DictList() {
  initialized_ = false;
  scis_num_ = 0;
  scis_hz_ = NULL;
  scis_splid_ = NULL;
  scis_splid_f_ = NULL;
  buf_ = NULL;
  spl_trie_ = SpellingTrie::get_cpinstance();

  assert(kMaxLemmaSize == 96);
  cmp_func_[0] = cmp_hanzis_1;
  cmp_func_[1] = cmp_hanzis_2;
  cmp_func_[2] = cmp_hanzis_3;
  cmp_func_[3] = cmp_hanzis_4;
  cmp_func_[4] = cmp_hanzis_5;
  cmp_func_[5] = cmp_hanzis_6;
  cmp_func_[6] = cmp_hanzis_7;
  cmp_func_[7] = cmp_hanzis_8;
  cmp_func_[8] = cmp_hanzis_9;
  cmp_func_[9] = cmp_hanzis_10;
  cmp_func_[10] = cmp_hanzis_11;
  cmp_func_[11] = cmp_hanzis_12;
  cmp_func_[12] = cmp_hanzis_13;
  cmp_func_[13] = cmp_hanzis_14;
  cmp_func_[14] = cmp_hanzis_15;
  cmp_func_[15] = cmp_hanzis_16;
  cmp_func_[16] = cmp_hanzis_17;
  cmp_func_[17] = cmp_hanzis_18;
  cmp_func_[18] = cmp_hanzis_19;
  cmp_func_[19] = cmp_hanzis_20;
  cmp_func_[20] = cmp_hanzis_21;
  cmp_func_[21] = cmp_hanzis_22;
  cmp_func_[22] = cmp_hanzis_23;
  cmp_func_[23] = cmp_hanzis_24;
  cmp_func_[24] = cmp_hanzis_25;
  cmp_func_[25] = cmp_hanzis_26;
  cmp_func_[26] = cmp_hanzis_27;
  cmp_func_[27] = cmp_hanzis_28;
  cmp_func_[28] = cmp_hanzis_29;
  cmp_func_[29] = cmp_hanzis_30;
  cmp_func_[30] = cmp_hanzis_31;
  cmp_func_[31] = cmp_hanzis_32;
  cmp_func_[32] = cmp_hanzis_33;
  cmp_func_[33] = cmp_hanzis_34;
  cmp_func_[34] = cmp_hanzis_35;
  cmp_func_[35] = cmp_hanzis_36;
  cmp_func_[36] = cmp_hanzis_37;
  cmp_func_[37] = cmp_hanzis_38;
  cmp_func_[38] = cmp_hanzis_39;
  cmp_func_[39] = cmp_hanzis_40;
  cmp_func_[40] = cmp_hanzis_41;
  cmp_func_[41] = cmp_hanzis_42;
  cmp_func_[42] = cmp_hanzis_43;
  cmp_func_[43] = cmp_hanzis_44;
  cmp_func_[44] = cmp_hanzis_45;
  cmp_func_[45] = cmp_hanzis_46;
  cmp_func_[46] = cmp_hanzis_47;
  cmp_func_[47] = cmp_hanzis_48;
  cmp_func_[48] = cmp_hanzis_49;
  cmp_func_[49] = cmp_hanzis_50;
  cmp_func_[50] = cmp_hanzis_51;
  cmp_func_[51] = cmp_hanzis_52;
  cmp_func_[52] = cmp_hanzis_53;
  cmp_func_[53] = cmp_hanzis_54;
  cmp_func_[54] = cmp_hanzis_55;
  cmp_func_[55] = cmp_hanzis_56;
  cmp_func_[56] = cmp_hanzis_57;
  cmp_func_[57] = cmp_hanzis_58;
  cmp_func_[58] = cmp_hanzis_59;
  cmp_func_[59] = cmp_hanzis_60;
  cmp_func_[60] = cmp_hanzis_61;
  cmp_func_[61] = cmp_hanzis_62;
  cmp_func_[62] = cmp_hanzis_63;
  cmp_func_[63] = cmp_hanzis_64;
  cmp_func_[64] = cmp_hanzis_65;
  cmp_func_[65] = cmp_hanzis_66;
  cmp_func_[66] = cmp_hanzis_67;
  cmp_func_[67] = cmp_hanzis_68;
  cmp_func_[68] = cmp_hanzis_69;
  cmp_func_[69] = cmp_hanzis_70;
  cmp_func_[70] = cmp_hanzis_71;
  cmp_func_[71] = cmp_hanzis_72;
  cmp_func_[72] = cmp_hanzis_73;
  cmp_func_[73] = cmp_hanzis_74;
  cmp_func_[74] = cmp_hanzis_75;
  cmp_func_[75] = cmp_hanzis_76;
  cmp_func_[76] = cmp_hanzis_77;
  cmp_func_[77] = cmp_hanzis_78;
  cmp_func_[78] = cmp_hanzis_79;
  cmp_func_[79] = cmp_hanzis_80;
  cmp_func_[80] = cmp_hanzis_81;
  cmp_func_[81] = cmp_hanzis_82;
  cmp_func_[82] = cmp_hanzis_83;
  cmp_func_[83] = cmp_hanzis_84;
  cmp_func_[84] = cmp_hanzis_85;
  cmp_func_[85] = cmp_hanzis_86;
  cmp_func_[86] = cmp_hanzis_87;
  cmp_func_[87] = cmp_hanzis_88;
  cmp_func_[88] = cmp_hanzis_89;
  cmp_func_[89] = cmp_hanzis_90;
  cmp_func_[90] = cmp_hanzis_91;
  cmp_func_[91] = cmp_hanzis_92;
  cmp_func_[92] = cmp_hanzis_93;
  cmp_func_[93] = cmp_hanzis_94;
  cmp_func_[94] = cmp_hanzis_95;
  cmp_func_[95] = cmp_hanzis_96;
}

DictList::~DictList() {
  free_resource();
}

bool DictList::alloc_resource(size_t buf_size, size_t scis_num) {
  // Allocate memory
  buf_ = static_cast<char16*>(malloc(buf_size * sizeof(char16)));
  if (NULL == buf_)
    return false;

  scis_num_ = scis_num;

  scis_hz_ = static_cast<char16*>(malloc(scis_num_ * sizeof(char16)));
  if (NULL == scis_hz_)
    return false;

  scis_splid_f_ = static_cast<SpellingIdFull*>
  (malloc(scis_num_ * sizeof(SpellingIdFull)));

  if (NULL == scis_splid_f_)
    return false;

  scis_splid_ = static_cast<SpellingId*>
  (malloc(scis_num_ * sizeof(SpellingId)));

  if (NULL == scis_splid_)
    return false;

  return true;
}

void DictList::free_resource() {
  if (NULL != buf_)
    free(buf_);
  buf_ = NULL;

  if (NULL != scis_hz_)
    free(scis_hz_);
  scis_hz_ = NULL;

  if (NULL != scis_splid_f_)
    free(scis_splid_f_);
  scis_splid_f_ = NULL;

  if (NULL != scis_splid_)
    free(scis_splid_);
  scis_splid_ = NULL;
}

#ifdef ___BUILD_MODEL___
bool DictList::init_list(const SingleCharItem *scis, size_t scis_num,
                         const LemmaEntry *lemma_arr, size_t lemma_num) {
  if (NULL == scis || 0 == scis_num || NULL == lemma_arr || 0 == lemma_num)
    return false;

  initialized_ = false;

  if (NULL != buf_)
    free(buf_);

  // calculate the size
  size_t buf_size = calculate_size(lemma_arr, lemma_num);
  if (0 == buf_size)
    return false;

  if (!alloc_resource(buf_size, scis_num))
    return false;

  fill_scis(scis, scis_num);

  // Copy the related content from the array to inner buffer
  fill_list(lemma_arr, lemma_num);

  initialized_ = true;
  return true;
}

size_t DictList::calculate_size(const LemmaEntry* lemma_arr, size_t lemma_num) {
  size_t last_hz_len = 0;
  size_t list_size = 0;
  size_t id_num = 0;
  size_t last_group_hz_len = 0;
    
  for (size_t i = 0; i < lemma_num; i++) {
    if (0 == i) {
      last_hz_len = lemma_arr[i].hz_str_len;

      assert(last_hz_len > 0);
      assert(lemma_arr[0].idx_by_hz == 1);

      id_num++;
      start_pos_[0] = 0;
      start_id_[0] = id_num;

      last_hz_len = 1;
      list_size += last_hz_len;
    } else {
        size_t current_hz_len = lemma_arr[i].hz_str_len;
        
        if (current_hz_len == last_hz_len) {
            list_size += current_hz_len;
            id_num++;
            
            if (current_hz_len - last_group_hz_len > 1) {
                // need to update start_id_ table
                for (size_t len = current_hz_len - 1; len > last_group_hz_len ; len--) {
                    start_id_[len-1] = start_id_[len];
                }
            }
            
        } else {
          assert(current_hz_len >= last_hz_len);
            
            if (last_hz_len - last_group_hz_len > 1) {
                // need to update start_id_ table
                for (size_t len = last_hz_len - 1; len > last_group_hz_len ; len--) {
                    start_id_[len-1] = start_id_[len];
                }
            }

            last_group_hz_len = last_hz_len;

          start_pos_[current_hz_len - 1] = list_size;
            start_id_[current_hz_len - 1] = id_num;
          
          if (current_hz_len - 1 > last_group_hz_len) {
              for (size_t len = current_hz_len - 1; len > last_group_hz_len ; len--) {
                  start_pos_[len-1] = start_pos_[len];
                  start_id_[len-1] = start_id_[len];
              }
          }
            
            id_num++;
            start_id_[current_hz_len - 1] = id_num;

        last_hz_len = current_hz_len;
        list_size += current_hz_len;
            
        }
    }
  }
    
    if (last_hz_len - last_group_hz_len > 1) {
        // need to update start_id_ table
        for (size_t len = last_hz_len - 1; len > last_group_hz_len ; len--) {
            start_id_[len-1] = start_id_[len];
        }
    }
    
    id_num++;

  for (size_t i = last_hz_len; i <= kMaxLemmaSize; i++) {
    if (0 == i) {
      start_pos_[0] = 0;
      start_id_[0] = 1;
    } else {
      start_pos_[i] = list_size;
      start_id_[i] = id_num;
    }
  }

  return start_pos_[kMaxLemmaSize];
}

void DictList::fill_scis(const SingleCharItem *scis, size_t scis_num) {
  assert(scis_num_ == scis_num);

  for (size_t pos = 0; pos < scis_num_; pos++) {
    scis_hz_[pos] = scis[pos].hz;
    scis_splid_[pos] = scis[pos].splid;
  }
}

void DictList::fill_list(const LemmaEntry* lemma_arr, size_t lemma_num) {
  size_t current_pos = 0;

  utf16_strncpy(buf_, lemma_arr[0].hanzi_str,
                lemma_arr[0].hz_str_len);

  current_pos = lemma_arr[0].hz_str_len;

  size_t id_num = 1;

  for (size_t i = 1; i < lemma_num; i++) {
    utf16_strncpy(buf_ + current_pos, lemma_arr[i].hanzi_str,
                  lemma_arr[i].hz_str_len);

    id_num++;
    current_pos += lemma_arr[i].hz_str_len;
  }

  assert(current_pos == start_pos_[kMaxLemmaSize]);
  assert(id_num + 1 == start_id_[kMaxLemmaSize]);
}

char16* DictList::find_pos2_startedbyhz(char16 hz_char) {
  char16 *found_2w = static_cast<char16*>
                     (mybsearch(&hz_char, buf_ + start_pos_[1],
                                (start_pos_[2] - start_pos_[1]) / 2,
                                sizeof(char16) * 2, cmp_hanzis_1));
  if (NULL == found_2w)
    return NULL;

  while (found_2w > buf_ + start_pos_[1] && *found_2w == *(found_2w - 1))
    found_2w -= 2;

  return found_2w;
}
#endif  // ___BUILD_MODEL___

char16* DictList::find_pos_startedbyhzs(const char16 last_hzs[],
    size_t word_len, int (*cmp_func)(const void *, const void *)) {
  char16 *found_w = static_cast<char16*>
                    (mybsearch(last_hzs, buf_ + start_pos_[word_len - 1],
                               (start_pos_[word_len] - start_pos_[word_len - 1])
                               / word_len,
                               sizeof(char16) * word_len, cmp_func));

  if (NULL == found_w)
    return NULL;

  while (found_w > buf_ + start_pos_[word_len -1] &&
         cmp_func(found_w, found_w - word_len) == 0)
    found_w -= word_len;

  return found_w;
}

size_t DictList::predict(const char16 last_hzs[], uint16 hzs_len,
                         NPredictItem *npre_items, size_t npre_max,
                         size_t b4_used) {
  assert(hzs_len <= kMaxPredictSize && hzs_len > 0);

  // 1. Prepare work
  int (*cmp_func)(const void *, const void *) = cmp_func_[hzs_len - 1];

  NGram& ngram = NGram::get_instance();

  size_t item_num = 0;

  // 2. Do prediction
  for (uint16 pre_len = 1; pre_len <= kMaxPredictSize + 1 - hzs_len;
       pre_len++) {
    uint16 word_len = hzs_len + pre_len;
    char16 *w_buf = find_pos_startedbyhzs(last_hzs, word_len, cmp_func);
    if (NULL == w_buf)
      continue;
    while (w_buf < buf_ + start_pos_[word_len] &&
           cmp_func(w_buf, last_hzs) == 0 &&
           item_num < npre_max) {
      memset(npre_items + item_num, 0, sizeof(NPredictItem));
      utf16_strncpy(npre_items[item_num].pre_hzs, w_buf + hzs_len, pre_len);
      npre_items[item_num].pre_len = pre_len;
      npre_items[item_num].psb =
        ngram.get_uni_psb((size_t)(w_buf - buf_ - start_pos_[word_len - 1])
                          / word_len + start_id_[word_len - 1]);
      npre_items[item_num].his_len = hzs_len;
      npre_items[item_num].isUser = false;
      item_num++;
      w_buf += word_len;
    }
  }

  size_t new_num = 0;
  for (size_t i = 0; i < item_num; i++) {
    // Try to find it in the existing items
    size_t e_pos;
    for (e_pos = 1; e_pos <= b4_used; e_pos++) {
      if (utf16_strncmp((*(npre_items - e_pos)).pre_hzs, npre_items[i].pre_hzs,
                        kMaxPredictSize) == 0)
        break;
    }
    if (e_pos <= b4_used)
      continue;

    // If not found, append it to the buffer
    npre_items[new_num] = npre_items[i];
    new_num++;
  }

  return new_num;
}

uint16 DictList::get_lemma_str(LemmaIdType id_lemma, char16 *str_buf,
                               uint16 str_max) {
  if (!initialized_ || id_lemma >= start_id_[kMaxLemmaSize] || NULL == str_buf
      || str_max <= 1)
    return 0;

  // Find the range
  for (uint16 i = 0; i < kMaxLemmaSize; i++) {
    if (i + 1 > str_max - 1)
      return 0;
    if (start_id_[i] <= id_lemma && start_id_[i + 1] > id_lemma) {
      size_t id_span = id_lemma - start_id_[i];

      char16 *buf = buf_ + start_pos_[i] + id_span * (i + 1);
      for (uint16 len = 0; len <= i; len++) {
        str_buf[len] = buf[len];
      }
      str_buf[i+1] = (char16)'\0';
      return i + 1;
    }
  }
  return 0;
}

uint16 DictList::get_splids_for_hanzi(char16 hanzi, uint16 half_splid,
                                      uint16 *splids, uint16 max_splids) {
  char16 *hz_found = static_cast<char16*>
      (mybsearch(&hanzi, scis_hz_, scis_num_, sizeof(char16), cmp_hanzis_1));
  assert(NULL != hz_found && hanzi == *hz_found);

  // Move to the first one.
  while (hz_found > scis_hz_ && hanzi == *(hz_found - 1))
    hz_found--;

  // First try to found if strict comparison result is not zero.
  char16 *hz_f = hz_found;
  bool strict = false;
  while (hz_f < scis_hz_ + scis_num_ && hanzi == *hz_f) {
    uint32 pos = hz_f - scis_hz_;
      if (spl_homo_enabled) {
          if (0 == half_splid || scis_splid_f_[pos].half_splid == half_splid) {
              strict = true;
          }
      } else {
          if (0 == half_splid || scis_splid_[pos].half_splid == half_splid) {
              strict = true;
          }
      }
    hz_f++;
  }

  uint16 found_num = 0;
  while (hz_found < scis_hz_ + scis_num_ && hanzi == *hz_found) {
    uint32 pos = hz_found - scis_hz_;
      if (spl_homo_enabled) {
          if (0 == half_splid ||
              (strict && scis_splid_f_[pos].half_splid == half_splid) ||
               (!strict && spl_trie_->half_full_compatible(half_splid,
                                                scis_splid_f_[pos].full_splid))) {
                  assert(found_num + 1 < max_splids);
                  splids[found_num] = scis_splid_f_[pos].full_splid;
                  found_num++;
              }
          } else {
              if (0 == half_splid ||
                  (strict && scis_splid_[pos].half_splid == half_splid) ||
                  (!strict && spl_trie_->half_full_compatible(half_splid,
                                                scis_splid_[pos].full_splid))) {
                  assert(found_num + 1 < max_splids);
                  splids[found_num] = scis_splid_[pos].full_splid;
                  found_num++;
              }
          }
    hz_found++;
  }

  return found_num;
}

LemmaIdType DictList::get_lemma_id(const char16 *str, uint16 str_len) {
  if (NULL == str || str_len > kMaxLemmaSize)
    return 0;

  char16 *found = find_pos_startedbyhzs(str, str_len, cmp_func_[str_len - 1]);
  if (NULL == found)
    return 0;

  assert(found > buf_);
  assert(static_cast<size_t>(found - buf_) >= start_pos_[str_len - 1]);
  return static_cast<LemmaIdType>
      (start_id_[str_len - 1] +
       (found - buf_ - start_pos_[str_len - 1]) / str_len);
}

void DictList::convert_to_hanzis(char16 *str, uint16 str_len) {
  assert(NULL != str);

  for (uint16 str_pos = 0; str_pos < str_len; str_pos++) {
    str[str_pos] = scis_hz_[str[str_pos]];
  }
}

void DictList::convert_to_scis_ids(char16 *str, uint16 str_len) {
  assert(NULL != str);

  for (uint16 str_pos = 0; str_pos < str_len; str_pos++) {
    str[str_pos] = 0x100;
  }
}

bool DictList::save_list(FILE *fp) {
  if (!initialized_ || NULL == fp)
    return false;

  if (NULL == buf_ || 0 == start_pos_[kMaxLemmaSize] ||
      NULL == scis_hz_ || NULL == scis_splid_ || NULL == scis_splid_f_ || 0 == scis_num_)
    return false;

  if (fwrite(&scis_num_, sizeof(uint32), 1, fp) != 1)
    return false;

  if (fwrite(start_pos_, sizeof(uint32), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  if (fwrite(start_id_, sizeof(uint32), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  if (fwrite(scis_hz_, sizeof(char16), scis_num_, fp) != scis_num_)
    return false;

  if (fwrite(scis_splid_, sizeof(SpellingId), scis_num_, fp) != scis_num_)
    return false;

  if (fwrite(buf_, sizeof(char16), start_pos_[kMaxLemmaSize], fp) !=
      start_pos_[kMaxLemmaSize])
    return false;

  return true;
}

const unsigned char* DictList::load_list(const unsigned char* mem) {
    if (NULL == mem)
        return NULL;
    
    initialized_ = false;
    
    scis_num_ = *(uint32*)mem;
    mem += sizeof(uint32);
    memcpy(start_pos_, mem, (kMaxLemmaSize + 1)*sizeof(uint32));
    mem += (kMaxLemmaSize + 1)*sizeof(uint32);
    memcpy(start_id_, mem, (kMaxLemmaSize + 1)*sizeof(uint32));
    mem += (kMaxLemmaSize + 1)*sizeof(uint32);
    
    free_resource();
    
    if (!alloc_resource(start_pos_[kMaxLemmaSize], scis_num_))
        return NULL;
    
    memcpy(scis_hz_, mem, scis_num_*sizeof(char16));
    mem += scis_num_*sizeof(char16);
    
    memcpy(scis_splid_, mem, scis_num_*sizeof(SpellingId));
    mem += scis_num_*sizeof(SpellingId);
    
    memcpy(buf_, mem, start_pos_[kMaxLemmaSize]*sizeof(char16));
    mem += start_pos_[kMaxLemmaSize]*sizeof(SpellingId);
    
    initialized_ = true;
    return mem;
}

bool DictList::load_list(FILE *fp) {
  if (NULL == fp)
    return false;

  initialized_ = false;

  if (fread(&scis_num_, sizeof(uint32), 1, fp) != 1)
    return false;

  if (fread(start_pos_, sizeof(uint32), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  if (fread(start_id_, sizeof(uint32), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  free_resource();

  if (!alloc_resource(start_pos_[kMaxLemmaSize], scis_num_))
    return false;

  if (fread(scis_hz_, sizeof(char16), scis_num_, fp) != scis_num_)
    return false;


    if (spl_homo_enabled) {
        if (fread(scis_splid_f_, sizeof(SpellingIdFull), scis_num_, fp) != scis_num_)
            return false;
    } else {
        if (fread(scis_splid_, sizeof(SpellingId), scis_num_, fp) != scis_num_)
            return false;
    }

  if (fread(buf_, sizeof(char16), start_pos_[kMaxLemmaSize], fp) !=
      start_pos_[kMaxLemmaSize])
    return false;

  initialized_ = true;
  return true;
}
    
#ifdef IQQI_EXTEND
    /*
    uint16 DictList::_get_splids_for_hanzi(char16 hanzi, uint16 half_splid,
                                          uint16 *splids, uint16 max_splids) {
        char16 *hz_found = static_cast<char16*>
        (mybsearch(&hanzi, scis_hz_, scis_num_, sizeof(char16), cmp_hanzis_1));
        if (NULL == hz_found)
            return 0;
        
        // Move to the first one.
        while (hz_found > scis_hz_ && hanzi == *(hz_found - 1))
            hz_found--;
        
        // First try to found if strict comparison result is not zero.
        char16 *hz_f = hz_found;
        bool strict = false;
        while (hz_f < scis_hz_ + scis_num_ && hanzi == *hz_f) {
            uint16 pos = hz_f - scis_hz_;
            if (spl_homo_enabled) {
                if (0 == half_splid || scis_splid_f_[pos].half_splid == half_splid) {
                    strict = true;
                }
            } else {
                if (0 == half_splid || scis_splid_[pos].half_splid == half_splid) {
                    strict = true;
                }
            }
            hz_f++;
        }
        
        uint16 found_num = 0;
        while (hz_found < scis_hz_ + scis_num_ && hanzi == *hz_found) {
            uint16 pos = hz_found - scis_hz_;
            if (spl_homo_enabled) {
                if (0 == half_splid ||
                    (strict && scis_splid_f_[pos].half_splid == half_splid) ||
                    (!strict && spl_trie_->half_full_compatible(half_splid,
                                                                scis_splid_f_[pos].full_splid))) {
                    if (found_num + 1 < max_splids)
                        splids[found_num] = scis_splid_f_[pos].full_splid;
                    found_num++;
                }
            } else {
                if (0 == half_splid ||
                    (strict && scis_splid_[pos].half_splid == half_splid) ||
                    (!strict && spl_trie_->half_full_compatible(half_splid,
                                                                scis_splid_[pos].full_splid))) {
                    if (found_num + 1 < max_splids)
                        splids[found_num] = scis_splid_[pos].full_splid;
                    found_num++;
                }
            }
            hz_found++;
        }
        
        return found_num;
    }
     */
#endif
}  // namespace ime_pinyin
