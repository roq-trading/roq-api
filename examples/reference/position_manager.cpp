/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/position_manager.h"
#include <glog/logging.h>

namespace examples {
namespace reference {

void PositionManager::on(const quinclas::common::TradeUpdate& trade_update) {
  // TODO(thraneh): multiply by sign derived from direction
  _position[trade_update.instrument] += trade_update.quantity;
}

}  // namespace reference
}  // namespace examples
