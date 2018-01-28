/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <limits>

namespace examples {
namespace reference {

class OrderImbalance final {
 public:
  void on(const quinclas::common::MarketByPrice&);
  double get_price() const { return _price; }
 private:
  double _price = std::numeric_limits<double>::quiet_NaN();
};

}  // namespace reference
}  // namespace examples
