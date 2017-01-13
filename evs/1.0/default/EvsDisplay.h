/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ANDROID_HARDWARE_EVS_V1_0_EVSDISPLAY_H
#define ANDROID_HARDWARE_EVS_V1_0_EVSDISPLAY_H

#include <android/hardware/evs/1.0/IEvsDisplay.h>
#include <ui/GraphicBuffer.h>

namespace android {
namespace hardware {
namespace evs {
namespace V1_0 {
namespace implementation {

class EvsDisplay : public IEvsDisplay {
public:
    // Methods from ::android::hardware::evs::V1_0::IEvsDisplay follow.
    Return<void> getDisplayInfo(getDisplayInfo_cb _hidl_cb)  override;
    Return<EvsResult> setDisplayState(DisplayState state)  override;
    Return<DisplayState> getDisplayState()  override;
    Return<void> getTargetBuffer(getTargetBuffer_cb _hidl_cb)  override;
    Return<EvsResult> returnTargetBufferForDisplay(const hidl_handle& bufferHandle)  override;

    // Implementation details
    EvsDisplay();
    virtual ~EvsDisplay() override;

private:
    DisplayDesc     mInfo           = {};
    buffer_handle_t mBuffer         = nullptr;      // A graphics buffer into which we'll store images
    uint32_t        mStride         = 0;            // Bytes per line in the buffer

    bool            mFrameBusy      = false;        // A flag telling us our buffer is in use
    DisplayState    mRequestedState = DisplayState::NOT_VISIBLE;

    std::mutex      mAccessLock;
};

} // namespace implementation
} // namespace V1_0
} // namespace evs
} // namespace hardware
} // namespace android

#endif  // ANDROID_HARDWARE_EVS_V1_0_EVSDISPLAY_H
