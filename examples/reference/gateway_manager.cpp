/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/gateway_manager.h"
#include <glog/logging.h>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace reference {

// constructor

GatewayManager::GatewayManager(
    quinclas::common::Strategy::Dispatcher& dispatcher,
    const Config&& config)
    : _dispatcher(dispatcher), _config(std::move(config)),
      _order_manager(dispatcher, _config) {
  LOG(INFO) << _config;
}

// event handlers

void GatewayManager::on(const TimerEvent& event) {
  _order_manager.on(event.timer);
}

void GatewayManager::on(const IdleEvent&) {
  LOG(INFO) << "idle";
}

void GatewayManager::on(const GatewayStatusEvent& event) {
  LOG(INFO) << "xyz";
}

void GatewayManager::on(const ReferenceDataEvent& event) {
  LOG(INFO) << "xyz";
}

void GatewayManager::on(const MarketStatusEvent& event) {
  LOG(INFO) << "xyz";
}

void GatewayManager::on(const MarketByPriceEvent& event) {
  // TODO(thraneh): need price class + pre-initialized unordered_map
  const auto& market_by_price = event.market_by_price;
  if (_config.instrument.compare(market_by_price.instrument) != 0)
    return;
  const auto& depth = market_by_price.depth;
  double bid_price = depth[0].bid_price;
  double ask_price = depth[0].ask_price;
}

void GatewayManager::on(const SessionStatisticsEvent& event) {
  LOG(INFO) << "xyz";
}

void GatewayManager::on(const DailyStatisticsEvent& event) {
  LOG(INFO) << "xyz";
}

void GatewayManager::on(const CreateOrderAckEvent& event) {
  _order_manager.on(event.create_order_ack);
}

void GatewayManager::on(const ModifyOrderAckEvent& event) {
  _order_manager.on(event.modify_order_ack);
}

void GatewayManager::on(const CancelOrderAckEvent& event) {
  _order_manager.on(event.cancel_order_ack);
}

void GatewayManager::on(const OrderUpdateEvent& event) {
  _order_manager.on(event.order_update);
}

void GatewayManager::on(const TradeUpdateEvent& event) {
  _position_manager.on(event.trade_update);
}

// state management

void GatewayManager::trade() {
}

}  // namespace reference
}  // namespace examples
