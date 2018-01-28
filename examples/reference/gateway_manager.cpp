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
      _risk_manager(_config, _position_manager),
      _order_manager(_config, _risk_manager, _dispatcher) {
  LOG(INFO) << _config;
}

// event handlers

void GatewayManager::on(const TimerEvent& event) {
  _order_manager.on(event.timer);
  trade();
}

void GatewayManager::on(const IdleEvent&) {
}

void GatewayManager::on(const GatewayStatusEvent& event) {
  const auto& gateway_status = event.gateway_status;
  _order_manager_ready = gateway_status.market_data_login_status == LoginStatus::On;
  _market_data_ready = gateway_status.market_data_login_status == LoginStatus::On;
  trade();
}

void GatewayManager::on(const ReferenceDataEvent& event) {
}

void GatewayManager::on(const MarketStatusEvent& event) {
  const auto& market_status = event.market_status;
  if (_config.instrument.compare(market_status.instrument) != 0)
    return;
  _market_open = market_status.trading_status == TradingStatus::Open;
}

void GatewayManager::on(const MarketByPriceEvent& event) {
  const auto& market_by_price = event.market_by_price;
  if (_config.instrument.compare(market_by_price.instrument) != 0)
    return;
  _order_imbalance.on(market_by_price);
  trade();
}

void GatewayManager::on(const SessionStatisticsEvent& event) {
}

void GatewayManager::on(const DailyStatisticsEvent& event) {
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
  const auto& trade_update = event.trade_update;
  _position_manager.on(trade_update);
  if (_risk_manager.is_above_limit(trade_update.instrument)) {
    LOG(WARNING) << "Limit has been breached!";
    // TODO(thraneh): take action to reduce risk exposure
  }
}

// state management

void GatewayManager::trade() {
  if (!(_order_manager_ready && _market_data_ready && _market_open))
    return;
  // TODO(thraneh): model implementation
}

}  // namespace reference
}  // namespace examples
