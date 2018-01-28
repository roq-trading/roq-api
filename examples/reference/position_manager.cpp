/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/position_manager.h"
#include <glog/logging.h>

namespace examples {
namespace reference {

const double TOLERANCE = 1.0e-8;

namespace {
// TODO(thraneh): this is a utility funtion!
static double get_real_quantity(const quinclas::common::TradeDirection direction, const double quantity) {
  switch (direction) {
    case quinclas::common::TradeDirection::Buy:
      return quantity;
    case quinclas::common::TradeDirection::Sell:
      return -quantity;
    default:
      LOG(FATAL) << "Received unknown trade direction!";
  }
}
}  // namespace

void PositionManager::on(const quinclas::common::TradeUpdate& trade_update) {
  const auto trade_id = trade_update.external_trade_id;
  const auto quantity = get_real_quantity(trade_update.trade_direction, trade_update.quantity);
  const auto result = _trades.emplace(std::make_pair(trade_id, quantity));
  if (result.second) {
    _position[trade_update.instrument] += trade_update.quantity;
  } else {
    if (std::fabs((*result.first).second - quantity) > TOLERANCE)
      LOG(WARNING) << "Received different trade quantity!";
  }
}

double PositionManager::get_position(const std::string& instrument) const {
  const auto iter = _position.find(instrument);
  return iter == _position.end() ? 0.0 : (*iter).second;
}

}  // namespace reference
}  // namespace examples
