/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/mask.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct GatewayStatus final {
  GatewayStatus() {}
  GatewayStatus(const std::string_view &account) : account(account) {}

  GatewayStatus(const GatewayStatus &) = delete;
  GatewayStatus(GatewayStatus &&) = default;

  void clear() {
    supported.reset();
    available.reset();
    unavailable.reset();
  }

  [[nodiscard]] bool operator()(const roq::GatewayStatus &gateway_status) {
    auto dirty = false;
    dirty |= utils::update(supported, Mask<SupportType>{gateway_status.supported});
    dirty |= utils::update(available, Mask<SupportType>{gateway_status.available});
    dirty |= utils::update(unavailable, Mask<SupportType>{gateway_status.unavailable});
    return dirty;
  }

  [[nodiscard]] bool operator()(const Event<roq::GatewayStatus> &event) { return (*this)(event.value); }

  [[nodiscard]] operator roq::GatewayStatus() const {
    return {
        .account = account,
        .supported = supported.get(),
        .available = available.get(),
        .unavailable = unavailable.get(),
    };
  }

  const Account account;
  Mask<SupportType> supported = {};
  Mask<SupportType> available = {};
  Mask<SupportType> unavailable = {};
};

}  // namespace cache
}  // namespace roq
