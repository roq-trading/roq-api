/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

#include "roq/cache/market_by_price.h"

namespace roq {
namespace client {

//! An interface representing the operations needed to maintain a current view
//! of depth based on incremental updates.
/*!
 * A choice must be made whether to update based on
 * \ref roq::MarketByPriceUpdate or \ref roq::MarketByOrderUpdate.
 *
 * The `reset` method must be called following a disconnect.
 * (The next update is then expected to be a snapshot).
 */
class ROQ_PUBLIC DepthBuilder : public cache::MarketByPrice {
 public:
  void reset() { clear(); }  // note! will be deprecated

  template <typename T>
  void operator()(const T &value) {
    update_helper(value);
  }

  virtual size_t operator()(const MarketByPriceUpdate &, roq::span<Layer> &, bool fill_zero = true) = 0;

  virtual size_t operator()(const MarketByOrderUpdate &, roq::span<Layer> &, bool fill_zero = true) = 0;

  // helpers:

  template <typename T>
  size_t update(const T &update, Layer &depth, bool fill_zero = true) {
    roq::span tmp(&depth, 1);
    return (*this)(update, tmp, fill_zero);
  }

  template <typename T, typename Container>
  size_t update(const T &update, Container &depth, bool fill_zero = true) {
    roq::span tmp(std::data(depth), std::size(depth));
    return (*this)(update, tmp, fill_zero);
  }
};

}  // namespace client
}  // namespace roq
