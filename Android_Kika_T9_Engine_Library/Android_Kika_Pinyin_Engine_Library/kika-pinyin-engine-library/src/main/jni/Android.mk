LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := iqqipy
LOCAL_LDLIBS := -llog
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src/export_include
LOCAL_CFLAGS += -DIQQI_EXTEND
LOCAL_CPPFLAGS += -std=c++11 -fexceptions -DNDEBUG -D__NO_KOREAN -D__LINUX -D__GPP -D__FPHONE -D__CH_KAIOS -D__LINUX_BASE -D__KAIOS -D__FOR_ANDROID -D__I_WITH -D__FOR_HUAWEI -DIQQI_EXTEND -D__IQQI_PY -Diqqi_EXPORTS
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src/include $(LOCAL_PATH)/src/export_include
LOCAL_SRC_FILES := $(LOCAL_PATH)/android/com_android_inputmethod_pinyin_PinyinDecoderService.cpp \
	$(LOCAL_PATH)/src/share/dictbuilder.cpp \
	$(LOCAL_PATH)/src/share/dictlist.cpp \
	$(LOCAL_PATH)/src/share/dicttrie.cpp \
	$(LOCAL_PATH)/src/share/lpicache.cpp \
	$(LOCAL_PATH)/src/share/matrixsearch.cpp \
	$(LOCAL_PATH)/src/share/mystdlib.cpp \
	$(LOCAL_PATH)/src/share/ngram.cpp \
	$(LOCAL_PATH)/src/share/pinyinime.cpp \
	$(LOCAL_PATH)/src/share/searchutility.cpp \
	$(LOCAL_PATH)/src/share/spellingtable.cpp \
	$(LOCAL_PATH)/src/share/spellingtrie.cpp \
	$(LOCAL_PATH)/src/share/splparser.cpp \
	$(LOCAL_PATH)/src/share/userdict.cpp \
	$(LOCAL_PATH)/src/share/utf16char.cpp \
	$(LOCAL_PATH)/src/share/utf16reader.cpp \
	$(LOCAL_PATH)/src/share/sync.cpp

include $(BUILD_SHARED_LIBRARY)
