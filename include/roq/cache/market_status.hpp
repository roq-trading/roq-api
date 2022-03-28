/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct MarketStatus final {
  MarketStatus() = default;

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

  template <typename Context>
  [[nodiscard]] roq::MarketStatus convert(const Context &context) const {
    return {
        .stream_id = stream_id,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .trading_status = trading_status,
    };
  }

  uint16_t stream_id = {};

  TradingStatus trading_status = {};
};

}  // namespace cache
}  // namespace roq
