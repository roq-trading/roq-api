/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <string>
#include <unordered_map>

namespace examples {
namespace reference {

class PositionManager final {
 public:
  void on(const quinclas::common::TradeUpdate& trade_update);
 private:
  std::unordered_map<std::string, double> _position;
};

}  // namespace reference
}  // namespace examples

