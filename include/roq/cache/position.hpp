/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Position final {
  Position() = default;

  Position(Position const &) = delete;
  Position(Position &&) = default;

  void clear() {
    stream_id = {};
    external_account.clear();
    long_quantity = NaN;
    short_quantity = NaN;
    exchange_time_utc = {};
  }

  [[nodiscard]] bool operator()(PositionUpdate const &position_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, position_update.stream_id);
    dirty |= utils::update_if_not_empty(external_account, position_update.external_account);
    dirty |= utils::update(long_quantity, position_update.long_quantity);
    dirty |= utils::update(short_quantity, position_update.short_quantity);
    dirty |= utils::update_if_not_empty(exchange_time_utc, position_update.exchange_time_utc);
    return dirty;
  }

  template <typename Context>
  [[nodiscard]] PositionUpdate convert(Context const &context) const {
    return {
        .stream_id = stream_id,
        .account = context.account,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .external_account = external_account,
        .long_quantity = long_quantity,
        .short_quantity = short_quantity,
        .update_type = UpdateType::SNAPSHOT,
        .exchange_time_utc = exchange_time_utc,
        .sending_time_utc = {},
    };
  }

  uint16_t stream_id = {};

  ExternalAccount external_account;
  double long_quantity = NaN;
  double short_quantity = NaN;
  std::chrono::nanoseconds exchange_time_utc = {};
};

}  // namespace cache
}  // namespace roq
