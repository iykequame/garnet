// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GARNET_LIB_WLAN_MLME_INCLUDE_WLAN_MLME_CLIENT_BSS_H_
#define GARNET_LIB_WLAN_MLME_INCLUDE_WLAN_MLME_CLIENT_BSS_H_

#include <wlan/common/element.h>
#include <wlan/common/energy.h>
#include <wlan/common/logging.h>
#include <wlan/common/macaddr.h>
#include <wlan/mlme/mac_frame.h>
#include <wlan/mlme/macaddr_map.h>

#include <fuchsia/wlan/mlme/cpp/fidl.h>

#include <fbl/macros.h>
#include <fbl/ref_counted.h>
#include <fbl/ref_ptr.h>
#include <fbl/unique_ptr.h>
#include <wlan/protocol/mac.h>
#include <zircon/types.h>

namespace wlan {

// BeaconHash is a signature to compare consecutive beacons without memcmp().
// TODO(porce): Revamp to exclude varying IEs.
typedef uint32_t BeaconHash;

class Bss : public fbl::RefCounted<Bss> {
   public:
    Bss(const common::MacAddr& bssid) : bssid_(bssid) {
        supported_rates_.reserve(kMaxSupportedRatesLen);
        bss_desc_.ssid.resize(0);  // Make sure SSID is not marked as null
    }

    zx_status_t ProcessBeacon(const Beacon& beacon, size_t len, const wlan_rx_info_t* rx_info);

    std::string ToString() const;

    // TODO(porce): Move these out of Bss class.
    std::string RatesToString(const std::vector<uint8_t>& rates) const;

    const common::MacAddr& bssid() { return bssid_; }
    zx::time_utc ts_refreshed() { return ts_refreshed_; }
    const ::fuchsia::wlan::mlme::BSSDescription& bss_desc() const { return bss_desc_; }

   private:
    bool IsBeaconValid(const Beacon& beacon) const;

    // Refreshes timestamp and signal strength.
    void Renew(const Beacon& beacon, const wlan_rx_info_t* rx_info);
    bool HasBeaconChanged(const Beacon& beacon, size_t len) const;

    // Update content such as IEs.
    zx_status_t Update(const Beacon& beacon, size_t len);
    void ParseIE(Span<const uint8_t> ies);

    // TODO(porce): Move Beacon method into Beacon class.
    uint32_t GetBeaconSignature(const Beacon& beacon, size_t len) const;

    common::MacAddr bssid_;      // From Addr3 of Mgmt Header.
    zx::time_utc ts_refreshed_;  // Last time of Bss object update.

    // TODO(porce): Separate into class BeaconTracker.
    BeaconHash bcn_hash_{0};
    size_t bcn_len_{0};
    wlan_channel_t bcn_rx_chan_;

    // TODO(porce): Add ProbeResponse.
    ::fuchsia::wlan::mlme::BSSDescription bss_desc_;

    // TODO(porce): Unify into FIDL data structure
    std::vector<uint8_t> supported_rates_{};
    std::vector<uint8_t> ext_supp_rates_{};

    // Conditionally present. See IEEE Std 802.11-2016, 9.3.3.3 Table 9-27
    bool has_dsss_param_set_chan_ = false;
    uint8_t dsss_param_set_chan_;

    DISALLOW_COPY_AND_ASSIGN_ALLOW_MOVE(Bss);
};

using BssMap = MacAddrMap<fbl::RefPtr<Bss>, macaddr_map_type::kBss>;

::fuchsia::wlan::mlme::BSSTypes GetBssType(const CapabilityInfo& cap);

}  // namespace wlan

#endif  // GARNET_LIB_WLAN_MLME_INCLUDE_WLAN_MLME_CLIENT_BSS_H_
