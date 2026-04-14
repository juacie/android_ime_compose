# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
IQQIPY_PATH := $(LOCAL_PATH)/../../../../Android_Kika_Pinyin_Engine_Library/kika-pinyin-engine-library/src/main/jni

include $(CLEAR_VARS)

LOCAL_MODULE := iqqipy
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDES := $(IQQIPY_PATH)/src/include $(IQQIPY_PATH)/src/export_include
LOCAL_EXPORT_C_INCLUDES := $(IQQIPY_PATH)/src/export_include
LOCAL_CFLAGS += -DIQQI_EXTEND
LOCAL_CPPFLAGS += -std=c++11 -fexceptions -DNDEBUG -D__NO_KOREAN -D__LINUX -D__GPP -D__FPHONE -D__CH_KAIOS -D__LINUX_BASE -D__KAIOS -D__FOR_ANDROID -D__I_WITH -D__FOR_HUAWEI -DIQQI_EXTEND -D__IQQI_PY -Diqqi_EXPORTS
LOCAL_SRC_FILES := $(IQQIPY_PATH)/android/com_android_inputmethod_pinyin_PinyinDecoderService.cpp \
 	$(IQQIPY_PATH)/src/share/dictbuilder.cpp \
	$(IQQIPY_PATH)/src/share/dictlist.cpp \
	$(IQQIPY_PATH)/src/share/dicttrie.cpp \
	$(IQQIPY_PATH)/src/share/lpicache.cpp \
	$(IQQIPY_PATH)/src/share/matrixsearch.cpp \
	$(IQQIPY_PATH)/src/share/mystdlib.cpp \
	$(IQQIPY_PATH)/src/share/ngram.cpp \
	$(IQQIPY_PATH)/src/share/pinyinime.cpp \
	$(IQQIPY_PATH)/src/share/searchutility.cpp \
	$(IQQIPY_PATH)/src/share/spellingtable.cpp \
	$(IQQIPY_PATH)/src/share/spellingtrie.cpp \
	$(IQQIPY_PATH)/src/share/splparser.cpp \
	$(IQQIPY_PATH)/src/share/userdict.cpp \
	$(IQQIPY_PATH)/src/share/utf16char.cpp \
	$(IQQIPY_PATH)/src/share/utf16reader.cpp \
	$(IQQIPY_PATH)/src/share/sync.cpp

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libIQQILib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/capi $(LOCAL_PATH)/ime $(LOCAL_PATH)/ime/lang  $(LOCAL_PATH)/japi $(IQQIPY_PATH)/src/export_include $(LOCAL_PATH)/py/iqqi_include
#LOCAL_STRIP_MODULE :=true
LOCAL_CFLAGS += -std=c++11 -fexceptions -DNDEBUG -D__NO_KOREAN -D__LINUX -D__GPP -D__FPHONE -D__CH_KAIOS -D__LINUX_BASE -D__KAIOS -D__FOR_ANDROID -D__I_WITH -D__FOR_HUAWEI -DIQQI_EXTEND -D__IQQI_PY -Diqqi_EXPORTS
LOCAL_LDLIBS := -llog -Wl
LOCAL_SHARED_LIBRARIES := iqqipy
#APP_ALLOW_MISSING_DEPS := true

LOCAL_SRC_FILES := \
    japi/kika_qwt9_inputmethod_Resource_qwt9ini.cpp \
    japi/jni_data_utils.cpp \
    py/iqqi/iqqilib.cpp \
    py/iqqi/zhuyinToPinyin.cpp \
    py/iqqi/kt9ToPinyin.cpp \
    capi/dictionary.cpp \
    capi/proximity.cpp \
    capi/session.cpp \
    dictionary/header/header_policy.cpp \
    dictionary/header/header_read_write_utils.cpp \
    dictionary/property/ngram_context.cpp \
    dictionary/structure/dictionary_structure_with_buffer_policy_factory.cpp \
    dictionary/structure/backward/v402/ver4_dict_buffers.cpp \
    dictionary/structure/backward/v402/ver4_dict_constants.cpp \
    dictionary/structure/backward/v402/ver4_patricia_trie_node_reader.cpp \
    dictionary/structure/backward/v402/ver4_patricia_trie_node_writer.cpp \
    dictionary/structure/backward/v402/ver4_patricia_trie_policy.cpp \
    dictionary/structure/backward/v402/ver4_patricia_trie_reading_utils.cpp \
    dictionary/structure/backward/v402/ver4_patricia_trie_writing_helper.cpp \
    dictionary/structure/backward/v402/ver4_pt_node_array_reader.cpp \
    dictionary/structure/backward/v402/bigram/ver4_bigram_list_policy.cpp \
    dictionary/structure/backward/v402/content/bigram_dict_content.cpp \
    dictionary/structure/backward/v402/content/probability_dict_content.cpp \
    dictionary/structure/backward/v402/content/shortcut_dict_content.cpp \
    dictionary/structure/backward/v402/content/sparse_table_dict_content.cpp \
    dictionary/structure/backward/v402/content/terminal_position_lookup_table.cpp \
    dictionary/structure/pt_common/dynamic_pt_gc_event_listeners.cpp \
    dictionary/structure/pt_common/dynamic_pt_reading_helper.cpp \
    dictionary/structure/pt_common/dynamic_pt_reading_utils.cpp \
    dictionary/structure/pt_common/dynamic_pt_updating_helper.cpp \
    dictionary/structure/pt_common/dynamic_pt_writing_utils.cpp \
    dictionary/structure/pt_common/patricia_trie_reading_utils.cpp \
    dictionary/structure/pt_common/bigram/bigram_list_read_write_utils.cpp \
    dictionary/structure/pt_common/shortcut/shortcut_list_reading_utils.cpp \
    dictionary/structure/v2/patricia_trie_policy.cpp \
    dictionary/structure/v2/ver2_patricia_trie_node_reader.cpp \
    dictionary/structure/v2/ver2_pt_node_array_reader.cpp \
    dictionary/structure/v4/ver4_dict_buffers.cpp \
    dictionary/structure/v4/ver4_dict_constants.cpp \
    dictionary/structure/v4/ver4_patricia_trie_node_reader.cpp \
    dictionary/structure/v4/ver4_patricia_trie_node_writer.cpp \
    dictionary/structure/v4/ver4_patricia_trie_policy.cpp \
    dictionary/structure/v4/ver4_patricia_trie_reading_utils.cpp \
    dictionary/structure/v4/ver4_patricia_trie_writing_helper.cpp \
    dictionary/structure/v4/ver4_pt_node_array_reader.cpp \
    dictionary/structure/v4/content/dynamic_language_model_probability_utils.cpp \
    dictionary/structure/v4/content/language_model_dict_content.cpp \
    dictionary/structure/v4/content/language_model_dict_content_global_counters.cpp \
    dictionary/structure/v4/content/shortcut_dict_content.cpp \
    dictionary/structure/v4/content/sparse_table_dict_content.cpp \
    dictionary/structure/v4/content/terminal_position_lookup_table.cpp \
    dictionary/utils/buffer_with_extendable_buffer.cpp \
    dictionary/utils/byte_array_utils.cpp \
    dictionary/utils/dict_file_writing_utils.cpp \
    dictionary/utils/file_utils.cpp \
    dictionary/utils/forgetting_curve_utils.cpp \
    dictionary/utils/format_utils.cpp \
    dictionary/utils/mmapped_buffer.cpp \
    dictionary/utils/multi_bigram_map.cpp \
    dictionary/utils/probability_utils.cpp \
    dictionary/utils/sparse_table.cpp \
    dictionary/utils/trie_map.cpp \
    ime/autotest_kr.cpp \
    ime/autotest_v1.cpp \
    ime/cachedata.cpp \
    ime/data_korean.cpp \
    ime/data_korean2.cpp \
    ime/debug_util.cpp \
    ime/imebase.cpp \
    ime/iqqiapi.cpp \
    ime/maptable.cpp \
    ime/phraseimp.cpp \
    ime/phraseimp_v1.cpp \
    ime/phraseimp_v2.cpp \
    ime/phraseimp_v2_1.cpp \
    ime/phraseimp_v2_1w.cpp \
    ime/phraseimp_v2_2w.cpp \
    ime/phraseimp_v3.cpp \
    ime/string_util.cpp \
    ime/trans_key_korean.cpp \
    ime/vcandidate.cpp \
    ime/vmap_imp.cpp \
    ime/lang/Afrikaans.cpp \
    ime/lang/Albanian.cpp \
    ime/lang/Arabic.cpp \
    ime/lang/Armenian.cpp \
    ime/lang/Azerbaijani.cpp \
    ime/lang/Basque.cpp \
    ime/lang/Belarusian.cpp \
    ime/lang/Bengali_BD.cpp \
    ime/lang/Bengali_IN.cpp \
    ime/lang/Bosnian.cpp \
    ime/lang/Bulgarian.cpp \
    ime/lang/Catalan.cpp \
    ime/lang/Changjie.cpp \
    ime/lang/Chinese_CN.cpp \
    ime/lang/Chinese_HK.cpp \
    ime/lang/Chinese_TW.cpp \
    ime/lang/Croatian.cpp \
    ime/lang/Czech.cpp \
    ime/lang/Danish.cpp \
    ime/lang/Dutch.cpp \
    ime/lang/English_GB.cpp \
    ime/lang/English_US.cpp \
    ime/lang/Estonian.cpp \
    ime/lang/Finnish.cpp \
    ime/lang/French_CA.cpp \
    ime/lang/French_FR.cpp \
    ime/lang/Galician.cpp \
    ime/lang/Georgian.cpp \
    ime/lang/German.cpp \
    ime/lang/Greek.cpp \
    ime/lang/Hebrew.cpp \
    ime/lang/Hindi.cpp \
    ime/lang/Hungarian.cpp \
    ime/lang/Icelandic.cpp \
    ime/lang/Indonesian.cpp \
    ime/lang/Italian.cpp \
    ime/lang/Kazakh.cpp \
    ime/lang/Khmer.cpp \
    ime/lang/Korean.cpp \
    ime/lang/Lao.cpp \
    ime/lang/Latvian.cpp \
    ime/lang/Lithuanian.cpp \
    ime/lang/Macedonian.cpp \
    ime/lang/Malay.cpp \
    ime/lang/Norwegian.cpp \
    ime/lang/Persian.cpp \
    ime/lang/Philippines.cpp \
    ime/lang/Polish.cpp \
    ime/lang/Portuguese_BR.cpp \
    ime/lang/Portuguese_PT.cpp \
    ime/lang/Romanian.cpp \
    ime/lang/Russian.cpp \
    ime/lang/Serbian.cpp \
    ime/lang/Sinhala.cpp \
    ime/lang/Slovak.cpp \
    ime/lang/Slovenian.cpp \
    ime/lang/Spanish_ES.cpp \
    ime/lang/Spanish_US.cpp \
    ime/lang/Swahili.cpp \
    ime/lang/Swedish.cpp \
    ime/lang/Tagalog.cpp \
    ime/lang/Tamil.cpp \
    ime/lang/Telugu.cpp \
    ime/lang/Thai.cpp \
    ime/lang/Turkish.cpp \
    ime/lang/Ukrainian.cpp \
    ime/lang/Urdu.cpp \
    ime/lang/Uzbek_Cyrillic.cpp \
    ime/lang/Vietnamese.cpp \
    ime/lang/Xhosa.cpp \
    ime/lang/Zulu.cpp \
    ime/lang/Kannada.cpp \
    ime/lang/Malayalam.cpp \
    ime/lang/Farsi.cpp \
    ime/lang/Maithili.cpp \
    ime/lang/Burmese_Z.cpp \
    ime/lang/Burmese_U.cpp \
    ime/lang/Tibetan.cpp \
    ime/lang/Assamese.cpp \
    ime/lang/Gujarati.cpp \
    ime/lang/Marathi.cpp \
    ime/lang/Nepali.cpp \
    ime/lang/Oriya.cpp \
    ime/lang/Punjabi.cpp \
    ime/lang/Amharic.cpp \
    ime/lang/Jawa.cpp \
    ime/lang/Maori.cpp \
    ime/lang/Mongolian.cpp \
    ime/lang/Uyghur.cpp \
    suggest/core/suggest.cpp \
    suggest/core/suggest_instances.cpp \
    suggest/core/dicnode/dic_node.cpp \
    suggest/core/dicnode/dic_nodes_cache.cpp \
    suggest/core/dicnode/dic_node_utils.cpp \
    suggest/core/dictionary/dictionary.cpp \
    suggest/core/dictionary/dictionary_utils.cpp \
    suggest/core/dictionary/digraph_utils.cpp \
    suggest/core/dictionary/error_type_utils.cpp \
    suggest/core/layout/additional_proximity_chars.cpp \
    suggest/core/layout/proximity_info.cpp \
    suggest/core/layout/proximity_info_params.cpp \
    suggest/core/layout/proximity_info_state.cpp \
    suggest/core/layout/proximity_info_state_utils.cpp \
    suggest/core/policy/weighting.cpp \
    suggest/core/result/suggestions_output_utils.cpp \
    suggest/core/result/suggestion_results.cpp \
    suggest/core/rnn/rnn_context.cpp \
    suggest/core/rnn/rnn_utils.cpp \
    suggest/core/session/dic_traverse_session.cpp \
    suggest/policyimpl/gesture/gesture_scoring.cpp \
    suggest/policyimpl/gesture/gesture_suggest_policy_factory.cpp \
    suggest/policyimpl/gesture/gesture_traversal.cpp \
    suggest/policyimpl/gesture/gesture_weighting.cpp \
    suggest/policyimpl/gesture/scoring_params_g.cpp \
    suggest/policyimpl/typing/scoring_params.cpp \
    suggest/policyimpl/typing/typing_scoring.cpp \
    suggest/policyimpl/typing/typing_suggest_policy_factory.cpp \
    suggest/policyimpl/typing/typing_traversal.cpp \
    suggest/policyimpl/typing/typing_weighting.cpp \
    utils/autocorrection_threshold_utils.cpp \
    utils/char_utils.cpp \
    utils/time_keeper.cpp

include $(BUILD_SHARED_LIBRARY)