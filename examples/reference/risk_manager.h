/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>
#include "reference/config.h"
#include "reference/position_manager.h"

namespace examples {
namespace reference {

class RiskManager final {
 public:
  RiskManager(const Config& config, const PositionManager& position_manager);
  bool can_trade(const std::string& instrument, quinclas::common::TradeDirection direction,
                 double quantity, double existing_exposure) const;
  bool is_above_limit(const std::string& instrument) const;

 private:
  double get_limit(const std::string& instrument) const;

 private:
  const Config& _config;
  const PositionManager& _position_manager;
};

}  // namespace reference
}  // namespace examples
