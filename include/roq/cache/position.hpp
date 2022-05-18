/* Copyright (c) 2017-2022, Hans Erik Thrane */

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
    long_quantity_begin = NaN;
    short_quantity_begin = NaN;
  }

  [[nodiscard]] bool operator()(PositionUpdate const &position_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, position_update.stream_id);
    dirty |= utils::update(external_account, position_update.external_account);
    dirty |= utils::update(long_quantity, position_update.long_quantity);
    dirty |= utils::update(short_quantity, position_update.short_quantity);
    dirty |= utils::update(long_quantity_begin, position_update.long_quantity_begin);
    dirty |= utils::update(short_quantity_begin, position_update.short_quantity_begin);
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
        .long_quantity_begin = long_quantity_begin,
        .short_quantity_begin = short_quantity_begin,
    };
  }

  uint16_t stream_id = {};

  ExternalAccount external_account;
  double long_quantity = NaN;
  double short_quantity = NaN;
  double long_quantity_begin = NaN;
  double short_quantity_begin = NaN;
};

}  // namespace cache
}  // namespace roq
