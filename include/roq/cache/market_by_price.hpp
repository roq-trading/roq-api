/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.hpp"

namespace roq {
namespace cache {

//! Interface
struct ROQ_PUBLIC MarketByPrice {
  virtual ~MarketByPrice() {}

  // meta data:

  virtual std::string_view exchange() const = 0;
  virtual std::string_view symbol() const = 0;

  // maximum depth (used when maintaining a view of top N price levels, zero means unbounded)
  virtual uint16_t max_depth() const = 0;

  // increments (conversion between floating point and integer representation)
  virtual double price_increment() const = 0;
  virtual double quantity_increment() const = 0;

  // precision (required to show all significant decimal places)
  virtual Precision price_decimals() const = 0;
  virtual Precision quantity_decimals() const = 0;

  // last update
  virtual uint16_t stream_id() const = 0;
  virtual std::chrono::nanoseconds exchange_time_utc() const = 0;
  virtual uint64_t exchange_sequence() const = 0;

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

  // extract arrays of MBPUpdate's, one for each of bids and asks
  //   storage is managed externally and must be passed as arguments
  //   default is to throw an exception if internal storage exceeds provided storage
  //   setting allow_truncate to true will not throw and return arrays may then be truncated
  virtual std::pair<std::span<MBPUpdate const>, std::span<MBPUpdate const>> extract(
      std::span<MBPUpdate> const &bids, std::span<MBPUpdate> const &asks, bool allow_truncate = false) const = 0;

  // extract vectors of MBPUpdate's
  //   note! max_depth == 0 means full snapshot
  virtual void extract_2(std::vector<MBPUpdate> &bids, std::vector<MBPUpdate> &asks, size_t max_depth = 0) const = 0;

  // extract an array of Layer's
  //   storage is managed externally and must be passed as argument
  //   the entire array will be returned if fill_zero is true
  //   otherwise the array will be the maximum of size(bids) and size(asks)
  virtual std::span<Layer const> extract(std::span<Layer> const &, bool fill_zero = false) const = 0;

  // extract a vector of Layer's
  //   note! max_depth == 0 means full snapshot
  virtual void extract_2(std::vector<Layer> &, size_t max_depth = 0) const = 0;

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
  virtual Layer compute_vwap(double total_quantity) const = 0;

  // impact price (complexity depends on the number of required levels)
  //   returns Layer with bid_quantity/ask_quantity <= quantity depending on available liquidity
  virtual Layer compute_impact_price(double total_quantity) const = 0;

  // update methods:

  // generic update interface using operator()
  inline void operator()(auto const &value) { update_helper(value); }

  // simple update
  //   used when applying sequential updates, e.g. when caching
  inline void operator()(std::span<MBPUpdate const> const &bids, std::span<MBPUpdate const> const &asks) {
    update_helper(bids, asks);
  }

  // create normalized update (used when origin is an external "noisy" source)
  //   deals with sorting, de-duplication, etc.
  //   calls back with the final update
  template <typename Callback>
  inline void operator()(
      MarketByPriceUpdate const &market_by_price_update,
      std::vector<MBPUpdate> &bids,
      std::vector<MBPUpdate> &asks,
      Callback callback) {
    auto market_by_price_update_2 = create_update_helper(market_by_price_update, bids, asks);
    callback(std::as_const(market_by_price_update_2));
  }

  // apply a simple update (warning! do not use this function to process batch updates)
  //   note! quantity == 0 means remove
  //   note! should only be used for testing
  inline void operator()(Side side, MBPUpdate const &mbp_update) { update_helper(side, mbp_update); }

  // create snapshot
  template <typename Callback>
  inline void create_snapshot(std::vector<MBPUpdate> &bids, std::vector<MBPUpdate> &asks, Callback callback) const {
    auto market_by_price_update = create_snapshot_helper(bids, asks);
    callback(std::as_const(market_by_price_update));
  }

  // generate depth update from full update
  virtual std::pair<std::span<MBPUpdate const>, std::span<MBPUpdate const>> create_depth_update(
      MarketByPriceUpdate const &,
      size_t depth,
      std::span<MBPUpdate> const &bids,
      std::span<MBPUpdate> const &asks) const = 0;

 protected:
  virtual void update_helper(roq::ReferenceData const &) = 0;
  virtual void update_helper(MarketByPriceUpdate const &) = 0;

  virtual void update_helper(Side, MBPUpdate const &) = 0;

  virtual MarketByPriceUpdate create_update_helper(
      MarketByPriceUpdate const &, std::vector<MBPUpdate> &bids, std::vector<MBPUpdate> &asks) = 0;

  virtual MarketByPriceUpdate create_snapshot_helper(
      std::vector<MBPUpdate> &bids, std::vector<MBPUpdate> &asks) const = 0;

  // note! used when applying sequential updates
  virtual void update_helper(std::span<MBPUpdate const> const &bids, std::span<MBPUpdate const> const &asks) = 0;
};

}  // namespace cache
}  // namespace roq
