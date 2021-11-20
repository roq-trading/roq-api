/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/api.h"

#include "roq/cache/market_by_price.h"
#include "roq/cache/market_status.h"
#include "roq/cache/reference_data.h"
#include "roq/cache/statistics.h"
#include "roq/cache/top_of_book.h"

namespace roq {
namespace cache {

struct Market final {
  template <typename MarketByPriceFactory>
  Market(
      uint32_t id,
      const std::string_view &exchange,
      const std::string_view &symbol,
      const MarketByPriceFactory &create_market_by_price)
      : id(id), reference_data(exchange, symbol), market_status(exchange, symbol), top_of_book(exchange, symbol),
        market_by_price(create_market_by_price(exchange, symbol)), statistics(exchange, symbol) {}

  Market() = delete;
  Market(const Market &) = delete;

  Market(Market &&) = default;

  [[nodiscard]] bool operator()(const Event<roq::ReferenceData> &event) { return reference_data(event); }
  [[nodiscard]] bool operator()(const Event<roq::MarketStatus> &event) { return market_status(event); }
  [[nodiscard]] bool operator()(const Event<roq::TopOfBook> &event) { return top_of_book(event); }
  [[nodiscard]] bool operator()(const Event<MarketByPriceUpdate> &event) {
    (*market_by_price)(event);
    return true;  // note! always updated
  }
  // bool operator()(const Event<MarketByOrderUpdate> &event) { (*market_by_order)(event.value); return true; }
  [[nodiscard]] bool operator()(const Event<TradeSummary> &event) {
    return true;  // note! always signal update (never cached)
  }
  [[nodiscard]] bool operator()(const Event<StatisticsUpdate> &event) { return statistics(event); }
  // XXX TODO CustomMetrics

  const uint32_t id;
  ReferenceData reference_data;
  MarketStatus market_status;
  TopOfBook top_of_book;
  std::unique_ptr<MarketByPrice> market_by_price;
  // std::unique_ptr<MarketByOrder> market_by_order;
  Statistics statistics;
  // XXX TODO CustomMetrics
};

}  // namespace cache
}  // namespace roq
