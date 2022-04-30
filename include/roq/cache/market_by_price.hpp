/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.hpp"

namespace roq {
namespace cache {

//! Interface
class ROQ_PUBLIC MarketByPrice {
 public:
  virtual ~MarketByPrice() {}

  virtual std::string_view exchange() const = 0;
  virtual std::string_view symbol() const = 0;

  // validation
  virtual uint32_t checksum() const = 0;

  // increments used to convert between integer and floating point representation
  virtual double price_increment() const = 0;
  virtual double quantity_increment() const = 0;

  // precision required to show all significant decimal places
  virtual Decimals price_decimals() const = 0;
  virtual Decimals quantity_decimals() const = 0;

  // max-depth (used when maintaining a view of the top N price levels)
  virtual uint16_t max_depth() const = 0;

  // note! the internal representation can change at any time

  // convert price/quantity to internal representation
  virtual int64_t price_to_internal(double price) const = 0;
  virtual uint64_t quantity_to_internal(double quantity) const = 0;

  // convert internal representation to price/quantity
  virtual double internal_to_price(int64_t price) const = 0;
  virtual double internal_to_quantity(uint64_t quantity) const = 0;

  virtual bool empty() const = 0;
  virtual void clear() = 0;

  virtual bool is_bad() const = 0;

  virtual std::chrono::nanoseconds exchange_time_utc() const = 0;
  virtual int64_t exchange_sequence() const = 0;

  template <typename T>
  void operator()(const T &value) {
    update_helper(value);
  }

  // normalized update, e.g. unsorted update or "incremental" full image
  template <typename F>
  void operator()(
      const MarketByPriceUpdate &market_by_price_update,
      const std::span<MBPUpdate> &final_bids,
      const std::span<MBPUpdate> &final_asks,
      F callback) {
    auto [final_bids_size, final_asks_size] = update_helper(market_by_price_update, final_bids, final_asks);
    auto bids = final_bids_size ? std::span{std::data(final_bids), final_bids_size} : market_by_price_update.bids;
    auto asks = final_asks_size ? std::span{std::data(final_asks), final_asks_size} : market_by_price_update.asks;
    const MarketByPriceUpdate final_market_by_price_update{
        .stream_id = market_by_price_update.stream_id,
        .exchange = market_by_price_update.exchange,
        .symbol = market_by_price_update.symbol,
        .bids = bids,
        .asks = asks,
        .update_type = market_by_price_update.update_type,
        .exchange_time_utc = market_by_price_update.exchange_time_utc,
        .exchange_sequence = market_by_price_update.exchange_sequence,
        .price_decimals = price_decimals(),
        .quantity_decimals = quantity_decimals(),
        .max_depth = max_depth(),
        .checksum = checksum(),
    };
    callback(final_market_by_price_update);
  }

  // copy-out
  virtual size_t extract(const std::span<Layer> &, bool fill_zero = false) const = 0;
  virtual std::pair<size_t, size_t> extract(
      const std::span<MBPUpdate> &bids, const std::span<MBPUpdate> &asks) const = 0;

  // atomic update to the order book (quantity == 0 means remove)
  void operator()(Side side, double price, double quantity) { update_helper(side, price, quantity); }

  // apply incremental update
  void operator()(const std::span<MBPUpdate> &bids, const std::span<MBPUpdate> &asks) { update_helper(bids, asks); }

  // check if price exists
  virtual bool exists(Side, double price) const = 0;

  // volume weighted average price (complexity depends on the number of required levels)
  virtual Layer compute_vwap(double quantity) const = 0;

  // note! the following methods should **NOT** be considered stable

  using price_level_t = std::pair<int64_t, uint64_t>;

  virtual std::span<price_level_t const> bids() const = 0;
  virtual std::span<price_level_t const> asks() const = 0;

 protected:
  virtual void update_helper(const MarketByPriceUpdate &) = 0;

  virtual std::pair<size_t, size_t> update_helper(
      const MarketByPriceUpdate &, const std::span<MBPUpdate> &bids, const std::span<MBPUpdate> &asks) = 0;

  virtual void update_helper(Side, double price, double quantity) = 0;

  virtual void update_helper(const std::span<MBPUpdate> &bids, const std::span<MBPUpdate> &asks) = 0;
};

}  // namespace cache
}  // namespace roq
