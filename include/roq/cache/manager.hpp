/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>

#include <string>
#include <utility>

#include "roq/api.hpp"

#include "roq/cache/market.hpp"

namespace roq {
namespace cache {

struct Manager final {
  using MarketId = uint32_t;
  using MarketByPriceFactory =
      std::function<std::unique_ptr<MarketByPrice>(std::string_view const &exchange, std::string_view const &symbol)>;

  explicit Manager(MarketByPriceFactory const &market_by_price_factory)
      : market_by_price_factory_(market_by_price_factory) {}
  explicit Manager(MarketByPriceFactory &&market_by_price_factory)
      : market_by_price_factory_(std::move(market_by_price_factory)) {}

  // always guaranteed to return (or throw)
  std::pair<Market &, bool> get_market_or_create(std::string_view const &exchange, std::string_view const &symbol) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      auto iter = markets_.find(market_id);
      if (iter != std::end(markets_))
        return {(*iter).second, false};
      std::abort();  // must be a bug
    } else {
      auto market_id = ++next_market_id_;
      exchange_to_symbols_[exchange].emplace(symbol, market_id);
      auto res = markets_.try_emplace(market_id, market_id, exchange, symbol, market_by_price_factory_);
      assert(res.second);
      return {(*res.first).second, true};
    }
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market(uint32_t market_id, Callback callback) {
    auto iter = markets_.find(market_id);
    if (iter == std::end(markets_))
      return false;
    callback((*iter).second);
    return true;
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market(std::string_view const &exchange, std::string_view const &symbol, Callback callback) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      auto iter = markets_.find(market_id);
      if (iter != std::end(markets_)) {
        callback((*iter).second);
        return true;
      }
    }
    return false;
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market_with_id(std::string_view const &exchange, std::string_view const &symbol, Callback callback) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      auto iter = markets_.find(market_id);
      if (iter != std::end(markets_)) {
        callback(market_id, (*iter).second);
        return true;
      }
    }
    return false;
  }

  // returns false if non-existing, calls back with market_id if exists
  template <typename Callback>
  bool get_market_id(std::string_view const &exchange, std::string_view const &symbol, Callback callback) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      callback(market_id);
      return true;
    }
    return false;
  }

  // calls back with all markets
  template <typename Callback>
  void get_all_markets(Callback callback) {
    for (auto &[_, market] : markets_)
      callback(market);
  }
  template <typename Callback>
  void get_all_markets(Callback callback) const {
    for (auto &[_, market] : markets_)
      callback(market);
  }

  // calls back with all exchanges
  template <typename Callback>
  void get_all_exchanges(Callback callback) const {
    for (auto &[exchange, _] : exchange_to_symbols_)
      callback(exchange);
  }

  // calls back with all symbols
  template <typename Callback>
  bool get_all_symbols(std::string_view const &exchange, Callback callback) const {
    auto iter = exchange_to_symbols_.find(exchange);
    if (iter == std::end(exchange_to_symbols_))
      return false;
    auto &tmp = (*iter).second;
    for (auto &[symbol, _] : tmp)
      callback(symbol);
    return true;
  }

 protected:
  uint32_t find_market_id(std::string_view const &exchange, std::string_view const &symbol) const {
    auto iter_1 = exchange_to_symbols_.find(exchange);
    if (iter_1 == std::end(exchange_to_symbols_))
      return 0;
    auto &symbols = (*iter_1).second;
    auto iter_2 = symbols.find(symbol);
    return iter_2 == std::end(symbols) ? 0 : (*iter_2).second;
  }

 private:
  const MarketByPriceFactory market_by_price_factory_;
  MarketId next_market_id_ = 0;
  absl::flat_hash_map<Exchange, absl::flat_hash_map<Symbol, MarketId>> exchange_to_symbols_;
  absl::node_hash_map<MarketId, Market> markets_;
};

}  // namespace cache
}  // namespace roq
