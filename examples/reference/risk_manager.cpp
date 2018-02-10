/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/risk_manager.h"
#include <quinclas/logging.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include "reference/utilities.h"

namespace examples {
namespace reference {

// constructor

RiskManager::RiskManager(const Config& config, const PositionManager& position_manager)
    : _config(config), _position_manager(position_manager) {}

// TODO(thraneh): access global time -- risk limits could very well be time-dependent

bool RiskManager::can_trade(const std::string& instrument,
                            quinclas::common::TradeDirection direction,
                            double quantity, double exposure) const {
  assert(quantity >= 0.0 && exposure >= 0.0);
  auto position = _position_manager.get_position(instrument);
  // current position must be seen from the direction of the proposed trade
  //   buy: (position + exposure + quantity) <= limit
  //   sell: (-position + exposure + quantity) <= limit
  position *= sign(direction);
  auto total = position + exposure + quantity;
  auto limit = get_limit(instrument);
  return is_less_than_or_equal(total, limit);
}

bool RiskManager::is_above_limit(const std::string& instrument) const {
  auto position = _position_manager.get_position(instrument);
  auto limit = get_limit(instrument);
  return is_less_than_or_equal(std::fabs(position), limit);
}

double RiskManager::get_limit(const std::string& instrument) const {
  return _config.risk_limit;
}

}  // namespace reference
}  // namespace examples
