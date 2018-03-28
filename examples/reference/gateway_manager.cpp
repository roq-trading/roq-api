/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/gateway_manager.h"
#include <roq/logging.h>

using namespace roq::common;  // NOLINT

namespace examples {
namespace reference {

// constructor

GatewayManager::GatewayManager(
    roq::common::Strategy::Dispatcher& dispatcher,
    Config&& config)
    : _dispatcher(dispatcher), _config(std::move(config)),
      _risk_manager(_config, _position_manager),
      _order_manager(_config, _risk_manager, _dispatcher),
      _trading_model(_config) {}

// event handlers

void GatewayManager::on(const TimerEvent& event) {
  // FIXME(thraneh): re-enable time-check
  // _order_manager.on(event);
  // check(event.message_info);
}

void GatewayManager::on(const roq::common::ConnectionStatusEvent& event) {
  if (event.connection_status != ConnectionStatus::Connected) {
    _market_data_ready = _order_manager_ready = false;
  }
}

void GatewayManager::on(const BatchBeginEvent&) {
}

void GatewayManager::on(const BatchEndEvent&) {
}

void GatewayManager::on(const ReadyEvent&) {
}

void GatewayManager::on(const GatewayStatusEvent& event) {
  const auto& gateway_status = event.gateway_status;
  // Check the status of each channel -- market data and order management.
  // Note! We currently expose the name of the gateway internal channel.
  // This is done because we do not yet know how many potential channels
  // a gateway may have. We may replace the name with an enum later on!
  if (std::strcmp(gateway_status.name, "MDUser") == 0) {
    _market_data_ready = gateway_status.status == GatewayState::Ready;
  } else if (std::strcmp(gateway_status.name, "Trader") == 0) {
    _order_manager_ready = gateway_status.status == GatewayState::Ready;
  }
  check(event.message_info);
}

void GatewayManager::on(const ReferenceDataEvent& event) {
}

void GatewayManager::on(const MarketStatusEvent& event) {
  const auto& market_status = event.market_status;
  if (_config.instrument.compare(market_status.instrument) != 0)
    return;
  _market_open = market_status.trading_status == TradingStatus::Open;
  check(event.message_info);
}

void GatewayManager::on(const MarketByPriceEvent& event) {
  const auto& market_by_price = event.market_by_price;
  if (_config.instrument.compare(market_by_price.instrument) != 0)
    return;
  _trading_model.on(event);
  check(event.message_info);
}

void GatewayManager::on(const TradeSummaryEvent& event) {
  const auto& trade_summary = event.trade_summary;
  if (_config.instrument.compare(trade_summary.instrument) != 0)
    return;
  _trading_model.on(event);
  check(event.message_info);
}

void GatewayManager::on(const CreateOrderAckEvent& event) {
  _order_manager.on(event);
  check(event.message_info);
}

void GatewayManager::on(const ModifyOrderAckEvent& event) {
  _order_manager.on(event);
  check(event.message_info);
}

void GatewayManager::on(const CancelOrderAckEvent& event) {
  _order_manager.on(event);
  check(event.message_info);
}

void GatewayManager::on(const OrderUpdateEvent& event) {
  _order_manager.on(event);
  check(event.message_info);
}

void GatewayManager::on(const TradeUpdateEvent& event) {
  const auto& trade_update = event.trade_update;
  _position_manager.on(trade_update);
  if (_risk_manager.is_above_limit(trade_update.instrument)) {
    LOG(WARNING) << "Limit has been breached!";
    // TODO(thraneh): take action to reduce risk exposure
  }
  check(event.message_info);
}

void GatewayManager::on(const roq::common::PositionUpdateEvent& event) {
}

// state management

void GatewayManager::check(const roq::common::MessageInfo& message_info) {
  // consistency check
  auto current_time = message_info.client_receive_time;
  LOG_IF(FATAL, _last_update_time < current_time) << "Wrong sequencing!";
  _last_update_time = current_time;
  // we can't do anything if the gateway isn't functional
  if (!(_order_manager_ready && _market_data_ready && _market_open))
    return;
  // TODO(thraneh): trade!
}

}  // namespace reference
}  // namespace examples
