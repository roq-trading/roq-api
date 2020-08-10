/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

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
class ROQ_PUBLIC DepthBuilder {
 public:
  virtual ~DepthBuilder() {
  }

  virtual void update(const ReferenceData& reference_data) = 0;

  virtual size_t update(
      const MarketByPriceUpdate& market_by_price,
      bool fill_zero = true) = 0;

  virtual size_t update(
      const MarketByOrderUpdate& market_by_order,
      bool fill_zero = true) = 0;

  virtual void reset() = 0;
};

}  // namespace client
}  // namespace roq
