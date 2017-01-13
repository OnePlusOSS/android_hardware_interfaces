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

#ifndef WIFI_STA_IFACE_H_
#define WIFI_STA_IFACE_H_

#include <android-base/macros.h>
#include <android/hardware/wifi/1.0/IWifiStaIface.h>
#include <android/hardware/wifi/1.0/IWifiStaIfaceEventCallback.h>

#include "wifi_legacy_hal.h"

namespace android {
namespace hardware {
namespace wifi {
namespace V1_0 {
namespace implementation {

/**
 * HIDL interface object used to control a STA Iface instance.
 */
class WifiStaIface : public IWifiStaIface {
 public:
  WifiStaIface(const std::string& ifname,
               const std::weak_ptr<legacy_hal::WifiLegacyHal> legacy_hal);
  // Refer to |WifiChip::invalidate()|.
  void invalidate();
  bool isValid();
  std::vector<sp<IWifiStaIfaceEventCallback>> getEventCallbacks();

  // HIDL methods exposed.
  Return<void> getName(getName_cb hidl_status_cb) override;
  Return<void> getType(getType_cb hidl_status_cb) override;
  Return<void> registerEventCallback(
      const sp<IWifiStaIfaceEventCallback>& callback,
      registerEventCallback_cb hidl_status_cb) override;
  Return<void> getCapabilities(getCapabilities_cb hidl_status_cb) override;
  Return<void> getApfPacketFilterCapabilities(
      getApfPacketFilterCapabilities_cb hidl_status_cb) override;
  Return<void> installApfPacketFilter(
      uint32_t cmd_id,
      const hidl_vec<uint8_t>& program,
      installApfPacketFilter_cb hidl_status_cb) override;
  Return<void> getBackgroundScanCapabilities(
      getBackgroundScanCapabilities_cb hidl_status_cb) override;
  Return<void> getValidFrequenciesForBackgroundScan(
      StaBackgroundScanBand band,
      getValidFrequenciesForBackgroundScan_cb hidl_status_cb) override;
  Return<void> startBackgroundScan(
      uint32_t cmd_id,
      const StaBackgroundScanParameters& params,
      startBackgroundScan_cb hidl_status_cb) override;
  Return<void> stopBackgroundScan(
      uint32_t cmd_id, stopBackgroundScan_cb hidl_status_cb) override;
  Return<void> enableLinkLayerStatsCollection(
      bool debug, enableLinkLayerStatsCollection_cb hidl_status_cb) override;
  Return<void> disableLinkLayerStatsCollection(
      disableLinkLayerStatsCollection_cb hidl_status_cb) override;
  Return<void> getLinkLayerStats(getLinkLayerStats_cb hidl_status_cb) override;
  Return<void> startRssiMonitoring(
      uint32_t cmd_id,
      int32_t max_rssi,
      int32_t min_rssi,
      startRssiMonitoring_cb hidl_status_cb) override;
  Return<void> stopRssiMonitoring(
      uint32_t cmd_id, stopRssiMonitoring_cb hidl_status_cb) override;
  Return<void> getRoamingCapabilities(
      getRoamingCapabilities_cb hidl_status_cb) override;
  Return<void> configureRoaming(const StaRoamingConfig& config,
                                configureRoaming_cb hidl_status_cb) override;
  Return<void> setRoamingState(StaRoamingState state,
                               setRoamingState_cb hidl_status_cb) override;
  Return<void> startDebugPacketFateMonitoring(
      startDebugPacketFateMonitoring_cb hidl_status_cb) override;
  Return<void> stopDebugPacketFateMonitoring(
      stopDebugPacketFateMonitoring_cb hidl_status_cb) override;
  Return<void> getDebugTxPacketFates(
      getDebugTxPacketFates_cb hidl_status_cb) override;
  Return<void> getDebugRxPacketFates(
      getDebugRxPacketFates_cb hidl_status_cb) override;

 private:
  // Corresponding worker functions for the HIDL methods.
  std::pair<WifiStatus, std::string> getNameInternal();
  std::pair<WifiStatus, IfaceType> getTypeInternal();
  WifiStatus registerEventCallbackInternal(
      const sp<IWifiStaIfaceEventCallback>& callback);
  std::pair<WifiStatus, uint32_t> getCapabilitiesInternal();
  std::pair<WifiStatus, StaApfPacketFilterCapabilities>
  getApfPacketFilterCapabilitiesInternal();
  WifiStatus installApfPacketFilterInternal(
      uint32_t cmd_id, const std::vector<uint8_t>& program);
  std::pair<WifiStatus, StaBackgroundScanCapabilities>
  getBackgroundScanCapabilitiesInternal();
  std::pair<WifiStatus, std::vector<WifiChannelInMhz>>
  getValidFrequenciesForBackgroundScanInternal(StaBackgroundScanBand band);
  WifiStatus startBackgroundScanInternal(
      uint32_t cmd_id, const StaBackgroundScanParameters& params);
  WifiStatus stopBackgroundScanInternal(uint32_t cmd_id);
  WifiStatus enableLinkLayerStatsCollectionInternal(bool debug);
  WifiStatus disableLinkLayerStatsCollectionInternal();
  std::pair<WifiStatus, StaLinkLayerStats> getLinkLayerStatsInternal();
  WifiStatus startRssiMonitoringInternal(uint32_t cmd_id,
                                         int32_t max_rssi,
                                         int32_t min_rssi);
  WifiStatus stopRssiMonitoringInternal(uint32_t cmd_id);
  std::pair<WifiStatus, StaRoamingCapabilities>
  getRoamingCapabilitiesInternal();
  WifiStatus configureRoamingInternal(const StaRoamingConfig& config);
  WifiStatus setRoamingStateInternal(StaRoamingState state);
  WifiStatus startDebugPacketFateMonitoringInternal();
  WifiStatus stopDebugPacketFateMonitoringInternal();
  std::pair<WifiStatus, std::vector<WifiDebugTxPacketFateReport>>
  getDebugTxPacketFatesInternal();
  std::pair<WifiStatus, std::vector<WifiDebugRxPacketFateReport>>
  getDebugRxPacketFatesInternal();

  std::string ifname_;
  std::weak_ptr<legacy_hal::WifiLegacyHal> legacy_hal_;
  std::vector<sp<IWifiStaIfaceEventCallback>> event_callbacks_;
  bool is_valid_;

  DISALLOW_COPY_AND_ASSIGN(WifiStaIface);
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // WIFI_STA_IFACE_H_
