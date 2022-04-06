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
  struct Context final {
    Exchange exchange;
    Symbol symbol;
  };

  template <typename MarketByPriceFactory>
  Market(
      uint32_t market_id,
      const std::string_view &exchange,
      const std::string_view &symbol,
      const MarketByPriceFactory &create_market_by_price)
      : market_id(market_id),
        context{
            .exchange = exchange,
            .symbol = symbol,
        },
        market_by_price(create_market_by_price(exchange, symbol)) {}

  Market() = delete;
  Market(const Market &) = delete;

  Market(Market &&) = default;

  operator const Context &() const { return context; }

  [[nodiscard]] bool operator()(const Event<roq::ReferenceData> &event) { return reference_data(event); }
  [[nodiscard]] bool operator()(const Event<roq::MarketStatus> &event) { return market_status(event); }
  [[nodiscard]] bool operator()(const Event<roq::TopOfBook> &event) { return top_of_book(event); }
  [[nodiscard]] bool operator()(const Event<MarketByPriceUpdate> &event) {
    (*market_by_price)(event);
    return true;  // note! always updated
  }
  // bool operator()(const Event<MarketByOrderUpdate> &event) { (*market_by_order)(event.value); return true; }
  [[nodiscard]] bool operator()(const Event<TradeSummary> &) {
    return true;  // note! always signal update (never cached)
  }
  [[nodiscard]] bool operator()(const Event<StatisticsUpdate> &event) { return statistics(event); }
  // XXX TODO CustomMetrics

  [[nodiscard]] bool operator()(const Event<FundsUpdate> &event) {
    auto &account = get_account(event.value.account);
    return account.funds(event);
  }
  [[nodiscard]] bool operator()(const Event<PositionUpdate> &event) {
    auto &account = get_account(event.value.account);
    return account.position(event);
  }
  // note! assumes a single user_id
  [[nodiscard]] bool operator()(const Event<OrderUpdate> &event) {
    auto &[message_info, order_update] = event;
    auto &account = get_account(order_update.account);
    auto order_id = order_update.order_id;
    auto iter_2 = account.orders.try_emplace(order_id, order_id).first;
    return (*iter_2).second(event);
  }
  // note! assumes a single user_id
  [[nodiscard]] bool operator()(const Event<TradeUpdate> &) {
    return true;  // note! always signal update (not currently cached)
  }

  template <typename Callback>
  bool get_funds(const std::string_view &account, Callback callback) {
    auto iter = accounts.find(account);
    if (iter == std::end(accounts))
      return false;
    callback((*iter).second.funds);
    return true;
  }

  template <typename Callback>
  bool get_position(const std::string_view &account, Callback callback) {
    auto iter = accounts.find(account);
    if (iter == std::end(accounts))
      return false;
    callback((*iter).second.position);
    return true;
  }

  // note! random ordering
  template <typename Callback>
  bool get_orders(const std::string_view &account, Callback callback) {
    auto iter = accounts.find(account);
    if (iter == std::end(accounts))
      return false;
    auto &tmp = (*iter).second.orders;
    if (std::empty(tmp))
      return false;
    for (auto &[order_id, order] : tmp)
      callback(order);
    return true;
  }

  const uint32_t market_id;

  const Context context;

  ReferenceData reference_data;
  MarketStatus market_status;
  TopOfBook top_of_book;
  std::unique_ptr<MarketByPrice> market_by_price;
  // XXX TODO std::unique_ptr<MarketByOrder> market_by_order;
  Statistics statistics;
  // XXX TODO CustomMetrics custom_metrics;

  struct Account final {
    Funds funds;
    Position position;
    // XXX TODO absl::flat_hash_map<uint32_t, Trade> trades;
    absl::flat_hash_map<uint32_t, Order> orders;
    // XXX TODO CustomMetrics custom_metrics;
  };

  absl::flat_hash_map<roq::Account, Account> accounts;

 protected:
  Account &get_account(const std::string_view &account) {
    auto iter = accounts.try_emplace(account).first;
    return (*iter).second;
  }
};

}  // namespace cache
}  // namespace roq
