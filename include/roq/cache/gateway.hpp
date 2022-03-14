/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <cassert>
#include <string>

#include "roq/api.hpp"

#include "roq/cache/gateway_settings.hpp"
#include "roq/cache/gateway_status.hpp"

namespace roq {
namespace cache {

struct Gateway final {
  Gateway() = default;

  Gateway(const Gateway &) = delete;
  Gateway(Gateway &&) = default;

  [[nodiscard]] bool operator()(const Event<roq::Connected> &) { return true; }

  [[nodiscard]] bool operator()(const Event<roq::Disconnected> &) {
    new (this) Gateway{};
    return true;
  }

  [[nodiscard]] bool operator()(const Event<roq::DownloadBegin> &event) {
    auto &[message_info, download_begin] = event;
    auto &state = get_state(download_begin.account);
    return utils::update(state.downloading, true);
  }

  [[nodiscard]] bool operator()(const Event<roq::DownloadEnd> &event) {
    auto &[message_info, download_begin] = event;
    auto &state = get_state(download_begin.account);
    return utils::update(state.downloading, false);
  }

  [[nodiscard]] bool operator()(const Event<roq::GatewaySettings> &event) { return settings(event); }

  [[nodiscard]] bool operator()(const Event<roq::GatewayStatus> &event) {
    auto &[message_info, gateway_status] = event;
    auto &state = get_state(gateway_status.account);
    return state.status(event);
  }

  bool operator()(const SupportType &support) const { return settings.supports.has(support); }
  bool operator()(const utils::Mask<SupportType> &expected) const { return settings.supports.has_all(expected); }

  bool ready(const utils::Mask<SupportType> &expected) const { return ready(expected, state); }
  bool ready(const utils::Mask<SupportType> &expected, const std::string_view &account) const {
    auto iter = state_by_account.find(account);
    if (iter == std::end(state_by_account))
      return false;
    auto &state = (*iter).second;
    return ready(expected, state);
  }

  struct State final {
    bool downloading = false;
    GatewayStatus status = {};
  };

  GatewaySettings settings = {};
  State state = {};
  absl::flat_hash_map<std::string, State> state_by_account;

 private:
  State &get_state(const std::string_view &account) {
    if (std::empty(account))
      return state;
    return state_by_account[account];
  }

  bool ready(const utils::Mask<SupportType> &expected, const State &state) const {
    if (state.downloading)
      return false;
    return state.status.available.has_all(expected) && state.status.unavailable.has_none(expected);
  }
};

}  // namespace cache
}  // namespace roq
