# Copyright (c) 2017, The Linux Foundation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#    * Neither the name of The Linux Foundation nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
# ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
# BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#define LOG_TAG "halserver"
#include <hidl/HidlTransportSupport.h>
#include <hidl/LegacySupport.h>
#include<log/log.h>

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::hardware::registerPassthroughServiceImplementation;
using android::OK;

//Audio
#include <android/hardware/audio/2.0/IDevicesFactory.h>
#include <android/hardware/audio/effect/2.0/IEffectsFactory.h>
#include <android/hardware/soundtrigger/2.0/ISoundTriggerHw.h>
#include <android/hardware/broadcastradio/1.0/IBroadcastRadioFactory.h>
#include <android/hardware/broadcastradio/1.1/IBroadcastRadioFactory.h>
#include <com/qualcomm/qti/bluetooth_audio/1.0/IBluetoothAudio.h>
//Audio

//sensor
#include <android/hardware/sensors/1.0/ISensors.h>
#include <binder/ProcessState.h>
//sensor

//Drm
#include <1.0/default/CryptoFactory.h>
#include <1.0/default/DrmFactory.h>
//

//Audio
using android::hardware::audio::effect::V2_0::IEffectsFactory;
using android::hardware::audio::V2_0::IDevicesFactory;
using android::hardware::soundtrigger::V2_0::ISoundTriggerHw;
using android::hardware::registerPassthroughServiceImplementation;
using com::qualcomm::qti::bluetooth_audio::V1_0::IBluetoothAudio;

namespace broadcastradio = android::hardware::broadcastradio;

#ifdef TARGET_USES_BCRADIO_FUTURE_FEATURES
static const bool useBroadcastRadioFutureFeatures = true;
#else
static const bool useBroadcastRadioFutureFeatures = false;
#endif
//Audio

//sensor
using android::hardware::sensors::V1_0::ISensors;
//sensori

//DRM
using android::hardware::drm::V1_0::ICryptoFactory;
using android::hardware::drm::V1_0::IDrmFactory;
//DRM

android::status_t registerAudioServices(){
    android::status_t status;
    status = registerPassthroughServiceImplementation<IDevicesFactory>();
    LOG_ALWAYS_FATAL_IF(status != OK, "Error while registering audio service: %d", status);
    status = registerPassthroughServiceImplementation<IEffectsFactory>();
    LOG_ALWAYS_FATAL_IF(status != OK, "Error while registering audio effects service: %d", status);
    // Soundtrigger and FM radio might be not present.
    status = registerPassthroughServiceImplementation<ISoundTriggerHw>();
    ALOGE_IF(status != OK, "Error while registering soundtrigger service: %d", status);
    status = registerPassthroughServiceImplementation<IBluetoothAudio>();
    ALOGE_IF(status != OK, "Error while registering bluetooth_audio service: %d", status);
    if (useBroadcastRadioFutureFeatures) {
        status = registerPassthroughServiceImplementation<
            broadcastradio::V1_1::IBroadcastRadioFactory>();
    } else {
        status = registerPassthroughServiceImplementation<
            broadcastradio::V1_0::IBroadcastRadioFactory>();
    }
    ALOGE_IF(status != OK, "Error while registering fm radio service: %d", status);
    return status;
}


int main() {
android::ProcessState::initWithDriver("/dev/vndbinder");
android::status_t status;
configureRpcThreadpool(64, true /*callerWillJoin*/);

//audio
status = registerAudioServices();
//audio

//sensor
registerPassthroughServiceImplementation<ISensors>();
//sensor

//DRM
status = registerPassthroughServiceImplementation<IDrmFactory>();
LOG_ALWAYS_FATAL_IF(
      status != android::OK,
      "Error while registering drm service: %d", status);

status = registerPassthroughServiceImplementation<ICryptoFactory>();
LOG_ALWAYS_FATAL_IF(
     status != android::OK,
     "Error while registering crypto service: %d", status);
//DRM

joinRpcThreadpool();

return 0;
}
