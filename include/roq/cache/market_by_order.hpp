/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.hpp"

namespace roq {
namespace cache {

struct ROQ_PUBLIC MarketByOrder {
  virtual ~MarketByOrder() {}

  // meta data:

  virtual std::string_view exchange() const = 0;
  virtual std::string_view symbol() const = 0;

  // maximum depth (used when maintaining a view of top N price levels, zero means unbounded)
  virtual uint16_t max_depth() const = 0;

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

  // generic update interface using operator()
  inline void operator()(auto const &value) { update_helper(value); }

  // extract methods:

  // extract vectors of MBOUpdate's
  //   note! max_depth == 0 means full snapshot
  virtual void extract(std::vector<MBOUpdate> &bids, std::vector<MBOUpdate> &asks, size_t max_depth = 0) const = 0;

  // extract a vector of Layer's
  //   note! max_depth == 0 means full snapshot
  virtual void extract(std::vector<Layer> &, size_t max_depth = 0) const = 0;

  // computation methods:

  // check if price level exists
  virtual bool exists(Side, double price) const = 0;

  // find price-level index (if exists)
  //   returns {index, exists?}
  virtual std::pair<size_t, bool> find_index(Side, double price) const = 0;

  // find order (if exists)
  //   returns {order, exists?}
  virtual std::pair<MBOUpdate, bool> find_order(Side, std::string_view const &order_id) const = 0;

  // order's queue position
  struct Position final {
    double quantity = NaN;  // quantity for order
    double before = NaN;    // total quantity ahead in queue priority
    double total = NaN;     // total quantity
  };
  virtual Position get_queue_position(Side, std::string_view const &order_id) const = 0;

 protected:
  virtual void update_helper(roq::ReferenceData const &) = 0;
  virtual void update_helper(MarketByOrderUpdate const &) = 0;
};

}  // namespace cache
}  // namespace roq
