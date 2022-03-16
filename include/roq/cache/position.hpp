/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Position final {
  Position(const std::string_view &account, const std::string_view &exchange, const std::string_view &symbol)
      : account(account), exchange(exchange), symbol(symbol) {}

  Position(const Position &) = delete;
  Position(Position &&) = default;

  void clear() {
    stream_id = {};
    external_account.clear();
    long_quantity = NaN;
    short_quantity = NaN;
    long_quantity_begin = NaN;
    short_quantity_begin = NaN;
  }

  [[nodiscard]] bool operator()(const PositionUpdate &position_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, position_update.stream_id);
    dirty |= utils::update(external_account, position_update.external_account);
    dirty |= utils::update(long_quantity, position_update.long_quantity);
    dirty |= utils::update(short_quantity, position_update.short_quantity);
    dirty |= utils::update(long_quantity_begin, position_update.long_quantity_begin);
    dirty |= utils::update(short_quantity_begin, position_update.short_quantity_begin);
    return dirty;
  }

  [[nodiscard]] operator PositionUpdate() const {
    return {
        .stream_id = stream_id,
        .account = account,
        .exchange = exchange,
        .symbol = symbol,
        .external_account = external_account,
        .long_quantity = long_quantity,
        .short_quantity = short_quantity,
        .long_quantity_begin = long_quantity_begin,
        .short_quantity_begin = short_quantity_begin,
    };
  }

  uint16_t stream_id = {};
  const string<MAX_LENGTH_ACCOUNT> account;
  const string<MAX_LENGTH_EXCHANGE> exchange;
  const string<MAX_LENGTH_SYMBOL> symbol;
  string<MAX_LENGTH_EXTERNAL_ACCOUNT> external_account;
  double long_quantity = NaN;
  double short_quantity = NaN;
  double long_quantity_begin = NaN;
  double short_quantity_begin = NaN;
};

}  // namespace cache
}  // namespace roq
