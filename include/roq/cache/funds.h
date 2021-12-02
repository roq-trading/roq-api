/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

#include "roq/utils/update.h"

namespace roq {
namespace cache {

struct Funds final {
  Funds(const std::string_view &account, const std::string_view &currency) : account(account), currency(currency) {}

  Funds(const Funds &) = delete;
  Funds(Funds &&) = default;

  void clear() {
    stream_id = {};
    balance = NaN;
    hold = NaN;
    external_account.clear();
  }

  [[nodiscard]] bool operator()(const roq::FundsUpdate &funds_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, funds_update.stream_id);
    dirty |= utils::update(balance, funds_update.balance);
    dirty |= utils::update(hold, funds_update.hold);
    dirty |= utils::update(external_account, funds_update.external_account);
    return dirty;
  }

  [[nodiscard]] operator roq::FundsUpdate() const {
    return {
        .stream_id = stream_id,
        .account = account,
        .currency = currency,
        .balance = balance,
        .hold = hold,
        .external_account = external_account,
    };
  }

  uint16_t stream_id = {};
  const string_buffer<MAX_LENGTH_ACCOUNT> account;
  const string_buffer<MAX_LENGTH_CURRENCY> currency;
  double balance = NaN;
  double hold = NaN;
  string_buffer<MAX_LENGTH_EXTERNAL_ACCOUNT> external_account;
};

}  // namespace cache
}  // namespace roq
