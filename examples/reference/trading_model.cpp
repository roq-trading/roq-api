/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/trading_model.h"

namespace examples {
namespace reference {

TradingModel::TradingModel(const Config& config) : _config(config) {}

void TradingModel::on(const roq::common::TimerEvent& event) {
}

void TradingModel::on(const roq::common::MarketByPriceEvent& event) {
  const roq::common::MarketByPrice& market_by_price = event.market_by_price;
  const auto& layer = market_by_price.depth[0];
  // trivial model-price dervied from order imbalance
  _model_price = (layer.bid_price * layer.ask_quantity + layer.ask_price * layer.bid_quantity) /
                 (layer.bid_quantity + layer.ask_quantity);
}

void TradingModel::on(const roq::common::TradeSummaryEvent& event) {
}

}  // namespace reference
}  // namespace examples
