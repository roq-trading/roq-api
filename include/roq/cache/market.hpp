/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/node_hash_map.h>

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
      std::string_view const &exchange,
      std::string_view const &symbol,
      MarketByPriceFactory const &create_market_by_price)
      : market_id{market_id},
        context{
            .exchange = exchange,
            .symbol = symbol,
        },
        market_by_price{create_market_by_price(exchange, symbol)} {}

  Market() = delete;
  Market(Market const &) = delete;

  Market(Market &&) = default;

  operator Context const &() const { return context; }

  [[nodiscard]] bool operator()(Event<roq::ReferenceData> const &event) { return reference_data(event); }
  [[nodiscard]] bool operator()(Event<roq::MarketStatus> const &event) { return market_status(event); }
  [[nodiscard]] bool operator()(Event<roq::TopOfBook> const &event) { return top_of_book(event); }
  [[nodiscard]] bool operator()(Event<MarketByPriceUpdate> const &event) {
    (*market_by_price)(event);
    return true;  // note! always updated
  }
  // bool operator()(const Event<MarketByOrderUpdate> &event) { (*market_by_order)(event.value); return true; }
  [[nodiscard]] bool operator()(Event<TradeSummary> const &) {
    return true;  // note! always signal update (never cached)
  }
  [[nodiscard]] bool operator()(Event<StatisticsUpdate> const &event) { return statistics(event); }
  // XXX TODO CustomMetrics

  [[nodiscard]] bool operator()(Event<FundsUpdate> const &event) {
    auto &account = get_account(event.value.account);
    return account.funds(event);
  }
  [[nodiscard]] bool operator()(Event<PositionUpdate> const &event) {
    auto &account = get_account(event.value.account);
    return account.position(event);
  }
  // note! assumes a single user_id
  [[nodiscard]] bool operator()(Event<OrderUpdate> const &event) {
    auto &[message_info, order_update] = event;
    auto &account = get_account(order_update.account);
    auto order_id = order_update.order_id;
    auto iter_2 = account.orders.try_emplace(order_id, order_id).first;
    return (*iter_2).second(event);
  }
  // note! assumes a single user_id
  [[nodiscard]] bool operator()(Event<TradeUpdate> const &) {
    return true;  // note! always signal update (not currently cached)
  }

  template <typename Callback>
  bool get_funds(std::string_view const &account, Callback callback) {
    auto iter = accounts.find(account);
    if (iter == std::end(accounts))
      return false;
    callback((*iter).second.funds);
    return true;
  }

  template <typename Callback>
  bool get_funds(std::string_view const &account, Callback callback) const {
    auto iter = accounts.find(account);
    if (iter == std::end(accounts))
      return false;
    callback((*iter).second.funds);
    return true;
  }

  template <typename Callback>
  bool get_position(std::string_view const &account, Callback callback) {
    auto iter = accounts.find(account);
    if (iter == std::end(accounts))
      return false;
    callback((*iter).second.position);
    return true;
  }

  template <typename Callback>
  bool get_position(std::string_view const &account, Callback callback) const {
    auto iter = accounts.find(account);
    if (iter == std::end(accounts))
      return false;
    callback((*iter).second.position);
    return true;
  }

  // note! random ordering
  template <typename Callback>
  bool get_orders(std::string_view const &account, Callback callback) {
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

  // note! random ordering
  template <typename Callback>
  bool get_orders(std::string_view const &account, Callback callback) const {
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

  uint32_t const market_id;

  Context const context;

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
    // XXX TODO absl::node_hash_map<uint32_t, Trade> trades;
    absl::node_hash_map<uint32_t, Order> orders;
    // XXX TODO CustomMetrics custom_metrics;
  };

  absl::node_hash_map<roq::Account, Account> accounts;

 protected:
  Account &get_account(std::string_view const &account) {
    auto iter = accounts.try_emplace(account).first;
    return (*iter).second;
  }
};

}  // namespace cache
}  // namespace roq
