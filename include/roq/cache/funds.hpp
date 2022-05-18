/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Funds final {
  Funds() = default;

  Funds(Funds const &) = delete;
  Funds(Funds &&) = default;

  void clear() {
    stream_id = {};
    balance = NaN;
    hold = NaN;
    external_account.clear();
  }

  [[nodiscard]] bool operator()(FundsUpdate const &funds_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, funds_update.stream_id);
    dirty |= utils::update(balance, funds_update.balance);
    dirty |= utils::update(hold, funds_update.hold);
    dirty |= utils::update(external_account, funds_update.external_account);
    return dirty;
  }

  template <typename Context>
  [[nodiscard]] FundsUpdate convert(Context const &context) const {
    return {
        .stream_id = stream_id,
        .account = context.account,
        .currency = context.symbol,
        .balance = balance,
        .hold = hold,
        .external_account = external_account,
    };
  }

  uint16_t stream_id = {};

  double balance = NaN;
  double hold = NaN;
  ExternalAccount external_account;
};

}  // namespace cache
}  // namespace roq
