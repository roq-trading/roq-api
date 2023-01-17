/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.hpp"

namespace roq {
namespace cache {

// !!! WORK IN PROGRESS !!!

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

 protected:
  virtual void update_helper(roq::ReferenceData const &) = 0;
  virtual void update_helper(MarketByOrderUpdate const &) = 0;
};

}  // namespace cache
}  // namespace roq
