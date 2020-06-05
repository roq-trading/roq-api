/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {
namespace client {

class ROQ_PUBLIC DepthBuilder {
 public:
  virtual ~DepthBuilder() {
  }

  virtual void update(const ReferenceData& reference_data) = 0;

  virtual size_t update(
      const MarketByPrice& market_by_price,
      bool fill_zero = true) = 0;

  virtual size_t update(
      const MarketByOrder& market_by_order,
      bool fill_zero = true) = 0;

  virtual void reset() = 0;
};

}  // namespace client
}  // namespace roq
