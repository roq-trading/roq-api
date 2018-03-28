/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>
#include <string>
#include <unordered_map>

namespace examples {
namespace reference {

class PositionManager final {
 public:
  void on(const roq::common::TradeUpdate& trade_update);
  double get_position(const std::string& instrument) const;
 private:
  std::unordered_map<std::string, double> _trades;
  std::unordered_map<std::string, double> _position;
};

}  // namespace reference
}  // namespace examples

