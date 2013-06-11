LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := b
LOCAL_SRC_FILES := test.c
LOCAL_PRELINK_MODULE := true
LOCAL_LDFLAGS := -L/sourcecode/arm-workspace/my-test/arm-package/jni/
LOCAL_LDLIBS += -la

include $(BUILD_EXECUTABLE)

