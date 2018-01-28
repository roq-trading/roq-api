/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/strategy.h"
#include <glog/logging.h>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace reference {

namespace {
// TODO(thraneh): move to config.cpp
static void log(const Config& config) {
  LOG(INFO) << "Config = {"
      "time_zone=\"" << config.time_zone.name() << "\", "
      "instrument=\"" << config.instrument << "\", "
      "exchange=\"" << config.exchange << "\", "
      "model_params=(" <<
      config.model_params.first << ", " <<
      config.model_params.second <<
      ")"
      "}";
}
}  // namespace

// constructor

Strategy::Strategy(quinclas::common::Strategy::Dispatcher& dispatcher,
                   const Config&& config)
    : _dispatcher(dispatcher), _config(std::move(config)),
      _order_manager(dispatcher, _config) {
  log(_config);
}

// event handlers

void Strategy::on(const IdleEvent&) {
  LOG(INFO) << "idle";
}

void Strategy::on(const GatewayStatusEvent& event) {
  LOG(INFO) << "xyz";
}

void Strategy::on(const ReferenceDataEvent& event) {
  LOG(INFO) << "xyz";
}

void Strategy::on(const MarketStatusEvent& event) {
  LOG(INFO) << "xyz";
}

void Strategy::on(const MarketByPriceEvent& event) {
  // TODO(thraneh): need price class + pre-initialized unordered_map
  const auto& market_by_price = event.market_by_price;
  if (_config.instrument.compare(market_by_price.instrument) != 0)
    return;
  const auto& depth = market_by_price.depth;
  double bid_price = depth[0].bid_price;
  double ask_price = depth[0].ask_price;
}

void Strategy::on(const SessionStatisticsEvent& event) {
  LOG(INFO) << "xyz";
}

void Strategy::on(const DailyStatisticsEvent& event) {
  LOG(INFO) << "xyz";
}

void Strategy::on(const CreateOrderAckEvent& event) {
  _order_manager.on(event.create_order_ack);
}

void Strategy::on(const ModifyOrderAckEvent& event) {
  _order_manager.on(event.modify_order_ack);
}

void Strategy::on(const CancelOrderAckEvent& event) {
  _order_manager.on(event.cancel_order_ack);
}

void Strategy::on(const OrderUpdateEvent& event) {
  _order_manager.on(event.order_update);
}

void Strategy::on(const TradeUpdateEvent& event) {
  _position_manager.on(event.trade_update);
}

// state management

void Strategy::trade() {
}

}  // namespace reference
}  // namespace examples
