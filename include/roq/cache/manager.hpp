/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <string>
#include <utility>

#include "roq/api.hpp"

#include "roq/cache/market.hpp"

namespace roq {
namespace cache {

template <typename MarketByPriceFactory>
struct Manager final {
  explicit Manager(const MarketByPriceFactory &market_by_price_factory)
      : market_by_price_factory_(market_by_price_factory) {}
  explicit Manager(MarketByPriceFactory &&market_by_price_factory)
      : market_by_price_factory_(std::move(market_by_price_factory)) {}

  // always guaranteed to return (or throw)
  std::pair<Market &, bool> get_market_or_create(const std::string_view &exchange, const std::string_view &symbol) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      auto iter = markets_.find(market_id);
      if (iter != std::end(markets_))
        return {(*iter).second, false};
      assert(false);  // must be a bug
    } else {
      auto market_id = ++next_market_id_;
      exchange_to_symbols_[exchange].emplace(symbol, market_id);
      Market market{market_id, exchange, symbol, market_by_price_factory_};
      auto res = markets_.emplace(market_id, std::move(market));
      assert(res.second);
      return {(*res.first).second, true};
    }
    std::abort();  // must never get here
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market(uint32_t market_id, Callback &&callback) {
    auto iter = markets_.find(market_id);
    if (iter == std::end(markets_))
      return false;
    callback((*iter).second);
    return true;
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market(const std::string_view &exchange, const std::string_view &symbol, Callback &&callback) {
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

  // calls back with all existing markets

  template <typename Callback>
  void get_all_markets(Callback &&callback) {
    for (auto &[_, market] : markets_)
      callback(market);
  }

  template <typename Callback>
  void get_all_markets(Callback &&callback) const {
    for (auto &[_, market] : markets_)
      callback(market);
  }

  template <typename Callback>
  void get_all_exchanges(Callback &&callback) const {
    for (auto &[exchange, _] : exchange_to_symbols_)
      callback(exchange);
  }

  template <typename Callback>
  bool get_all_symbols(const std::string_view &exchange, Callback &&callback) const {
    auto iter = exchange_to_symbols_.find(exchange);
    if (iter == std::end(exchange_to_symbols_))
      return false;
    auto &tmp = (*iter).second;
    for (auto &[symbol, _] : tmp)
      callback(symbol);
    return true;
  }

 protected:
  uint32_t find_market_id(const std::string_view &exchange, const std::string_view &symbol) const {
    auto iter_1 = exchange_to_symbols_.find(exchange);
    if (iter_1 == std::end(exchange_to_symbols_))
      return 0;
    auto &symbols = (*iter_1).second;
    auto iter_2 = symbols.find(symbol);
    return iter_2 == std::end(symbols) ? 0 : (*iter_2).second;
  }

 private:
  const std::function<MarketByPriceFactory> market_by_price_factory_;
  uint32_t next_market_id_ = 0;
  absl::flat_hash_map<std::string, absl::flat_hash_map<std::string, uint32_t>> exchange_to_symbols_;
  absl::flat_hash_map<uint32_t, Market> markets_;
};

}  // namespace cache
}  // namespace roq
