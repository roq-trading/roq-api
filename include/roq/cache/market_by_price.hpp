/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.hpp"

#include "roq/utils/safe_cast.hpp"

namespace roq {
namespace cache {

//! Interface
class ROQ_PUBLIC MarketByPrice {
 public:
  virtual ~MarketByPrice() {}

  virtual std::string_view exchange() const = 0;
  virtual std::string_view symbol() const = 0;

  // maximum depth (used when maintaining a view of top N price levels, zero means unbounded)
  virtual size_t max_depth() const = 0;

  // increments (conversion between floating point and integer representation)
  virtual double price_increment() const = 0;
  virtual double quantity_increment() const = 0;

  // precision (required to show all significant decimal places)
  virtual Decimals price_decimals() const = 0;
  virtual Decimals quantity_decimals() const = 0;

  // last update
  virtual uint16_t stream_id() const = 0;
  virtual std::chrono::nanoseconds exchange_time_utc() const = 0;
  virtual int64_t exchange_sequence() const = 0;

  // checksum (useful for validating correct application of incremental updates)
  virtual uint32_t checksum() const = 0;

  // storage methods:

  // current depth
  //   returns {size(bids), size(asks)}
  virtual std::pair<size_t, size_t> size() const = 0;

  // is empty?
  virtual bool empty() const = 0;

  // reset internal state
  virtual void clear() = 0;

  // extract methods:

  // extract an array of Layer's
  //   storage is managed externally and must be passed as argument
  //   the entire array will be returned if fill_zero is true
  //   otherwise the array will be the maximum of size(bids) and size(asks)
  virtual std::span<Layer> extract(std::span<Layer> const &, bool fill_zero = false) const = 0;

  // extract an arrays of MBPUpdate's, one for each of bids and asks
  //   storage is managed externally and must be passed as arguments
  //   default is to throw an exception if internal storage exceeds provided storage
  //   setting allow_truncate to true will not throw and return arrays may then be truncated
  virtual std::pair<std::span<MBPUpdate>, std::span<MBPUpdate>> extract(
      std::span<MBPUpdate> const &bids, std::span<MBPUpdate> const &asks, bool allow_truncate = false) const = 0;

  // computation methods:

  // check if price level exists
  virtual bool exists(Side, double price) const = 0;

  // find price-level index (if exists)
  //   returns {index, exists?}
  virtual std::pair<size_t, bool> find_index(Side, double price) const = 0;

  // test if best bid price >= best ask price
  virtual bool is_bad() const = 0;

  // volume weighted average price (complexity depends on the number of required levels)
  //   returns Layer with bid_quantity/ask_quantity <= quantity depending on available liquidity
  virtual Layer compute_vwap(double quantity) const = 0;

  // update methods:

  // generic update interface using operator()
  template <typename T>
  void operator()(const T &value) {
    update_helper(value);
  }

  // single update to the order book (quantity == 0 means remove)
  void operator()(Side side, MBPUpdate const &mbp_update) { update_helper(side, mbp_update); }

  // simple update
  //   used when applying sequential updates, e.g. when caching
  void operator()(std::span<MBPUpdate> const &bids, std::span<MBPUpdate> const &asks) { update_helper(bids, asks); }

  // generate a normalized update (used when origin is an external "noisy" source)
  //   deals with sorting, de-duplication, etc.
  //   calls back with the final update
  template <typename F>
  void operator()(
      MarketByPriceUpdate const &market_by_price_update,
      std::span<MBPUpdate> const &final_bids,
      std::span<MBPUpdate> const &final_asks,
      F callback) {
    auto [bids, asks] = update_helper(market_by_price_update, final_bids, final_asks);
    MarketByPriceUpdate const final_market_by_price_update{
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
        .max_depth = utils::safe_cast(max_depth()),
        .checksum = checksum(),
    };
    callback(final_market_by_price_update);
  }

  // generate depth update from full update
  virtual std::pair<std::span<MBPUpdate>, std::span<MBPUpdate>> create_depth_update(
      MarketByPriceUpdate const &,
      size_t depth,
      std::span<MBPUpdate> const &bids,
      std::span<MBPUpdate> const &asks) const = 0;

 protected:
  virtual void update_helper(Side, MBPUpdate const &) = 0;
  virtual void update_helper(std::span<MBPUpdate> const &bids, std::span<MBPUpdate> const &asks) = 0;
  virtual void update_helper(MarketByPriceUpdate const &) = 0;
  virtual std::pair<std::span<MBPUpdate>, std::span<MBPUpdate>> update_helper(
      MarketByPriceUpdate const &, std::span<MBPUpdate> const &bids, std::span<MBPUpdate> const &asks) = 0;
};

}  // namespace cache
}  // namespace roq
