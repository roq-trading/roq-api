/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/mask.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct GatewaySettings final {
  GatewaySettings() = default;

  GatewaySettings(const GatewaySettings &) = delete;
  GatewaySettings(GatewaySettings &&) = default;

  void clear() { new (this) GatewaySettings{}; }

  [[nodiscard]] bool operator()(const roq::GatewaySettings &gateway_settings) {
    auto dirty = false;
    dirty |= utils::update(supports, gateway_settings.supports);
    dirty |= utils::update(mbp_max_depth, gateway_settings.mbp_max_depth);
    dirty |= utils::update(mbp_tick_size_multiplier, gateway_settings.mbp_tick_size_multiplier);
    dirty |= utils::update(mbp_min_trade_vol_multiplier, gateway_settings.mbp_min_trade_vol_multiplier);
    dirty |= utils::update(mbp_allow_remove_non_existing, gateway_settings.mbp_allow_remove_non_existing);
    dirty |= utils::update(mbp_allow_price_inversion, gateway_settings.mbp_allow_price_inversion);
    dirty |= utils::update(oms_download_has_state, gateway_settings.oms_download_has_state);
    dirty |= utils::update(oms_download_has_routing_id, gateway_settings.oms_download_has_routing_id);
    return dirty;
  }

  [[nodiscard]] bool operator()(const Event<roq::GatewaySettings> &event) { return (*this)(event.value); }

  [[nodiscard]] roq::GatewaySettings convert() const {
    return {
        .supports = supports,
        .mbp_max_depth = mbp_max_depth,
        .mbp_tick_size_multiplier = mbp_tick_size_multiplier,
        .mbp_min_trade_vol_multiplier = mbp_min_trade_vol_multiplier,
        .mbp_allow_remove_non_existing = mbp_allow_remove_non_existing,
        .mbp_allow_price_inversion = mbp_allow_price_inversion,
        .oms_download_has_state = oms_download_has_state,
        .oms_download_has_routing_id = oms_download_has_routing_id,
    };
  }

  Mask<SupportType> supports;
  uint32_t mbp_max_depth = {};
  double mbp_tick_size_multiplier = NaN;
  double mbp_min_trade_vol_multiplier = NaN;
  bool mbp_allow_remove_non_existing = false;
  bool mbp_allow_price_inversion = false;
  bool oms_download_has_state = false;
  bool oms_download_has_routing_id = false;
};

}  // namespace cache
}  // namespace roq
