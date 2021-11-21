/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <memory>
#include <string>
#include <utility>

#include "roq/api.h"

#include "roq/cache/funds.h"
#include "roq/cache/market_by_price.h"
#include "roq/cache/market_status.h"
#include "roq/cache/position.h"
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

  [[nodiscard]] bool operator()(const Event<FundsUpdate> &event) {
    auto &[message_info, funds_update] = event;
    auto account = funds_update.account;
    auto iter = funds_by_account.find(account);
    if (iter == funds_by_account.end()) {
      Funds funds(account, funds_update.currency);
      iter = funds_by_account.emplace(account, std::move(funds)).first;
    }
    return (*iter).second(event);
  }
  [[nodiscard]] bool operator()(const Event<PositionUpdate> &event) {
    auto &[message_info, position_update] = event;
    auto account = position_update.account;
    auto iter = position_by_account.find(account);
    if (iter == position_by_account.end()) {
      Position position(account, position_update.exchange, position_update.symbol);
      iter = position_by_account.emplace(account, std::move(position)).first;
    }
    return (*iter).second(event);
  }

  template <typename Callback>
  bool get_funds(const std::string_view &account, Callback &&callback) {
    auto iter = funds_by_account.find(account);
    if (iter == funds_by_account.end())
      return false;
    callback((*iter).second);
    return true;
  }

  template <typename Callback>
  bool get_position(const std::string_view &account, Callback &&callback) {
    auto iter = position_by_account.find(account);
    if (iter == position_by_account.end())
      return false;
    callback((*iter).second);
    return true;
  }

  const uint32_t id;
  ReferenceData reference_data;
  MarketStatus market_status;
  TopOfBook top_of_book;
  std::unique_ptr<MarketByPrice> market_by_price;
  // std::unique_ptr<MarketByOrder> market_by_order;
  Statistics statistics;
  // XXX TODO CustomMetrics
  absl::flat_hash_map<std::string, Funds> funds_by_account;
  absl::flat_hash_map<std::string, Position> position_by_account;
};

}  // namespace cache
}  // namespace roq
