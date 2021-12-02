/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

#include "roq/utils/update.h"

namespace roq {
namespace cache {

struct MarketStatus final {
  MarketStatus(const std::string_view &exchange, const std::string_view &symbol) : exchange(exchange), symbol(symbol) {}

  MarketStatus(const MarketStatus &) = delete;
  MarketStatus(MarketStatus &&) = default;

  void clear() {
    stream_id = {};
    trading_status = {};
  }

  [[nodiscard]] bool operator()(const roq::MarketStatus &market_status) {
    auto dirty = false;
    dirty |= utils::update(stream_id, market_status.stream_id);
    dirty |= utils::update(trading_status, market_status.trading_status);
    return dirty;
  }

  [[nodiscard]] bool operator()(const Event<roq::MarketStatus> &event) { return (*this)(event.value); }

  [[nodiscard]] operator roq::MarketStatus() const {
    return {
        .stream_id = stream_id,
        .exchange = exchange,
        .symbol = symbol,
        .trading_status = trading_status,
    };
  }

  uint16_t stream_id = {};
  const string_buffer<MAX_LENGTH_EXCHANGE> exchange;
  const string_buffer<MAX_LENGTH_SYMBOL> symbol;
  TradingStatus trading_status = {};
};

}  // namespace cache
}  // namespace roq
