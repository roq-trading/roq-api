/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct MarketStatus final {
  MarketStatus() = default;

  MarketStatus(MarketStatus const &) = delete;
  MarketStatus(MarketStatus &&) = default;

  void clear() {
    stream_id = {};
    trading_status = {};
    exchange_time_utc = {};
  }

  [[nodiscard]] bool operator()(roq::MarketStatus const &market_status) {
    auto dirty = false;
    dirty |= utils::update(stream_id, market_status.stream_id);
    dirty |= utils::update(trading_status, market_status.trading_status);
    dirty |= utils::update(exchange_time_utc, market_status.exchange_time_utc);
    return dirty;
  }

  [[nodiscard]] bool operator()(Event<roq::MarketStatus> const &event) { return (*this)(event.value); }

  template <typename Context>
  [[nodiscard]] roq::MarketStatus convert(Context const &context) const {
    return {
        .stream_id = stream_id,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .trading_status = trading_status,
        .exchange_time_utc = exchange_time_utc,
        .exchange_sequence = {},
        .sending_time_utc = {},
    };
  }

  uint16_t stream_id = {};

  TradingStatus trading_status = {};
  std::chrono::nanoseconds exchange_time_utc = {};
};

}  // namespace cache
}  // namespace roq
