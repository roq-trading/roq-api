/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order_imbalance.h"

namespace examples {
namespace reference {

void OrderImbalance::on(const quinclas::common::MarketByPrice& market_by_price) {
  const auto& layer = market_by_price.depth[0];
  _price = (layer.bid_price * layer.ask_quantity + layer.ask_price * layer.bid_quantity) /
           (layer.bid_quantity + layer.ask_quantity);
}

}  // namespace reference
}  // namespace examples
