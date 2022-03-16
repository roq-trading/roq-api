/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include "roq/api.hpp"

#include "roq/cache/funds.hpp"
#include "roq/cache/market_by_price.hpp"
#include "roq/cache/market_status.hpp"
#include "roq/cache/order.hpp"
#include "roq/cache/position.hpp"
#include "roq/cache/reference_data.hpp"
#include "roq/cache/statistics.hpp"
#include "roq/cache/top_of_book.hpp"

namespace roq {
namespace cache {

struct Market final {
  template <typename MarketByPriceFactory>
  Market(
      uint32_t market_id,
      const std::string_view &exchange,
      const std::string_view &symbol,
      const MarketByPriceFactory &create_market_by_price)
      : market_id(market_id), exchange(exchange), symbol(symbol), reference_data(exchange, symbol),
        market_status(exchange, symbol), top_of_book(exchange, symbol),
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
    if (iter == std::end(funds_by_account)) {
      Funds funds(account, funds_update.currency);
      iter = funds_by_account.emplace(account, std::move(funds)).first;
    }
    return (*iter).second(event);
  }
  [[nodiscard]] bool operator()(const Event<PositionUpdate> &event) {
    auto &[message_info, position_update] = event;
    auto account = position_update.account;
    auto iter = position_by_account.find(account);
    if (iter == std::end(position_by_account)) {
      Position position(account, position_update.exchange, position_update.symbol);
      iter = position_by_account.emplace(account, std::move(position)).first;
    }
    return (*iter).second(event);
  }
  // note! assumes a single user_id
  [[nodiscard]] bool operator()(const Event<OrderUpdate> &event) {
    auto &[message_info, order_update] = event;
    auto account = order_update.account;
    auto order_id = order_update.order_id;
    auto &tmp = orders_by_account[account];
    auto iter = tmp.find(order_id);
    if (iter == std::end(tmp)) {
      Order order(account, order_update.exchange, order_update.symbol, order_id);
      iter = tmp.emplace(order_id, std::move(order)).first;
    }
    return (*iter).second(event);
  }
  // note! assumes a single user_id
  [[nodiscard]] bool operator()(const Event<TradeUpdate> &event) {
    return true;  // note! always signal update (not currently cached)
  }

  template <typename Callback>
  bool get_funds(const std::string_view &account, Callback callback) {
    auto iter = funds_by_account.find(account);
    if (iter == std::end(funds_by_account))
      return false;
    callback((*iter).second);
    return true;
  }

  template <typename Callback>
  bool get_position(const std::string_view &account, Callback callback) {
    auto iter = position_by_account.find(account);
    if (iter == std::end(position_by_account))
      return false;
    callback((*iter).second);
    return true;
  }

  // note! random ordering
  template <typename Callback>
  bool get_orders(const std::string_view &account, Callback callback) {
    auto iter = orders_by_account.find(account);
    if (iter == std::end(orders_by_account))
      return false;
    auto &tmp = (*iter).second;
    if (std::empty(tmp))
      return false;
    for (auto &[order_id, order] : tmp)
      callback(order);
    return true;
  }

  const uint32_t market_id;
  const string<MAX_LENGTH_EXCHANGE> exchange;
  const string<MAX_LENGTH_SYMBOL> symbol;

  ReferenceData reference_data;
  MarketStatus market_status;
  TopOfBook top_of_book;
  std::unique_ptr<MarketByPrice> market_by_price;
  // std::unique_ptr<MarketByOrder> market_by_order;
  Statistics statistics;
  // XXX TODO CustomMetrics
  absl::flat_hash_map<std::string, Funds> funds_by_account;
  absl::flat_hash_map<std::string, Position> position_by_account;
  absl::flat_hash_map<std::string, absl::flat_hash_map<uint32_t, Order>> orders_by_account;
};

}  // namespace cache
}  // namespace roq
