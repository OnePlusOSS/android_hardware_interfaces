LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := hal-server
LOCAL_INIT_RC := hal-server.rc
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := \
  hal-server.cpp \

LOCAL_SHARED_LIBRARIES := \
  libhidlbase \
  libhwbinder \
  libhidltransport \
  liblog \
  libutils \
  libhardware \

############Audio#################
LOCAL_SHARED_LIBRARIES += \
    android.hardware.audio@2.0 \
    android.hardware.audio.common@2.0 \
    android.hardware.audio.effect@2.0 \
    android.hardware.soundtrigger@2.0 \
    android.hardware.broadcastradio@1.0 \
    android.hardware.broadcastradio@1.1 \
    com.qualcomm.qti.bluetooth_audio@1.0_vendor

ifeq ($(strip $(AUDIOSERVER_MULTILIB)),)
LOCAL_MULTILIB := 32
else
LOCAL_MULTILIB := $(AUDIOSERVER_MULTILIB)
endif

ifeq ($(TARGET_USES_BCRADIO_FUTURE_FEATURES),true)
LOCAL_CFLAGS += -DTARGET_USES_BCRADIO_FUTURE_FEATURES
endif

######## Audio ##################

####### Sensor #################
LOCAL_SHARED_LIBRARIES += \
    android.hardware.sensors@1.0 \
    libbinder
####### Sensor #################


####### Drm ####################
LOCAL_SHARED_LIBRARIES += \
    android.hardware.drm@1.0 \
    android.hidl.memory@1.0 \
    libbinder

LOCAL_STATIC_LIBRARIES += \
    android.hardware.drm@1.0-helper

LOCAL_C_INCLUDES += hardware/interfaces/drm

LOCAL_HEADER_LIBRARIES := \
    media_plugin_headers

# TODO(b/18948909) Some legacy DRM plugins only support 32-bit. They need to be
# migrated to 64-bit. Once all of a device's legacy DRM plugins support 64-bit,
# that device can turn on TARGET_ENABLE_MEDIADRM_64 to build this service as
# 64-bit.
ifneq ($(TARGET_ENABLE_MEDIADRM_64), true)
LOCAL_32_BIT_ONLY := true
endif
####### Drm ####################
include $(BUILD_EXECUTABLE)
