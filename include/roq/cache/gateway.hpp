/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/node_hash_map.h>

#include <cassert>
#include <string>

#include "roq/api.hpp"

#include "roq/cache/gateway_settings.hpp"
#include "roq/cache/gateway_status.hpp"

namespace roq {
namespace cache {

struct Gateway final {
  Gateway() = default;

  Gateway(Gateway const &) = delete;
  Gateway(Gateway &&) = default;

  [[nodiscard]] bool operator()(Event<Connected> const &) { return true; }

  [[nodiscard]] bool operator()(Event<Disconnected> const &) {
    new (this) Gateway{};
    return true;
  }

  [[nodiscard]] bool operator()(Event<DownloadBegin> const &event) {
    auto &[message_info, download_begin] = event;
    auto &state = get_state(download_begin.account);
    return utils::update(state.downloading, true);
  }

  [[nodiscard]] bool operator()(Event<DownloadEnd> const &event) {
    auto &[message_info, download_begin] = event;
    auto &state = get_state(download_begin.account);
    return utils::update(state.downloading, false);
  }

  [[nodiscard]] bool operator()(Event<roq::GatewaySettings> const &event) { return settings(event); }

  [[nodiscard]] bool operator()(Event<roq::GatewayStatus> const &event) {
    auto &[message_info, gateway_status] = event;
    auto &state = get_state(gateway_status.account);
    return state.status(event);
  }

  bool operator()(SupportType support) const { return settings.supports.has(support); }
  bool operator()(Mask<SupportType> expected) const { return settings.supports.has_all(expected); }

  bool ready(Mask<SupportType> expected) const { return ready(expected, state); }
  bool ready(Mask<SupportType> expected, std::string_view const &account) const {
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
  absl::node_hash_map<Account, State> state_by_account;

 private:
  State &get_state(std::string_view const &account) {
    if (std::empty(account))
      return state;
    return state_by_account[account];
  }

  bool ready(Mask<SupportType> expected, State const &state) const {
    if (state.downloading)
      return false;
    return state.status.available.has_all(expected) && state.status.unavailable.has_none(expected);
  }
};

}  // namespace cache
}  // namespace roq
