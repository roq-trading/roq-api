/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/risk_manager.h"
#include <glog/logging.h>

namespace examples {
namespace reference {

const double TOLERANCE = 1.0e-8;

// constructor

RiskManager::RiskManager(const Config& config, const PositionManager& position_manager)
    : _config(config), _position_manager(position_manager) {}

double RiskManager::available_quantity(const std::string& instrument,
                                       quinclas::common::TradeDirection direction) const {
  // TODO(thraneh): correct implementation -- should take drection into consideration
  auto current_position = _position_manager.get_position(instrument);
  auto risk_limit = get_limit(instrument);
  return std::max(risk_limit - std::fabs(current_position), -0.0);
}

bool RiskManager::is_above_limit(const std::string& instrument) const {
  auto current_position = _position_manager.get_position(instrument);
  auto risk_limit = get_limit(instrument);
  return std::fabs(risk_limit - std::fabs(current_position)) > TOLERANCE;
}

double RiskManager::get_limit(const std::string& instrument) const {
  return 2.0;  // TODO(thraneh): must get this from the config + check against current time
}

}  // namespace reference
}  // namespace examples
