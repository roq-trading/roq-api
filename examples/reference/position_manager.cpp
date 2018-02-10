/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/position_manager.h"
#include <quinclas/logging.h>
#include "reference/utilities.h"

namespace examples {
namespace reference {

void PositionManager::on(const quinclas::common::TradeUpdate& trade_update) {
  const auto trade_id = trade_update.external_trade_id;
  const auto real_quantity = signed_quantity(
      trade_update.trade_direction, trade_update.quantity);
  const auto insert_result = _trades.emplace(
      std::make_pair(trade_id, real_quantity));
  if (insert_result.second) {
    _position[trade_update.instrument] += real_quantity;
  } else if (!is_equal((*insert_result.first).second, real_quantity)) {
      LOG(WARNING) << "Received different trade quantity. "
          "Previously recorded quantity: " << (*insert_result.first).second;
  }
}

double PositionManager::get_position(const std::string& instrument) const {
  const auto iter = _position.find(instrument);
  return iter == _position.end() ? 0.0 : (*iter).second;
}

}  // namespace reference
}  // namespace examples
