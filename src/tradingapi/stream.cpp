/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <cctz/time_zone.h>

#include <cmath>

#include "quinclas/tradingapi.h"

namespace quinclas {
namespace common {

}  // namespace common
}  // namespace quinclas

namespace {
const char *TIME_FORMAT = "%E4Y-%m-%dT%H:%M:%E6S";
const auto TIME_ZONE = cctz::utc_time_zone();
}


std::ostream& operator<<(std::ostream& stream, const quinclas::common::Number number) {
  if (std::isfinite(number._value))
    return stream << number._value;
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::ConnectionStatus value) {
  return stream << quinclas::common::EnumNameConnectionStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::LoginStatus value) {
  return stream << quinclas::common::EnumNameLoginStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradeDirection value) {
  return stream << quinclas::common::EnumNameTradeDirection(value);
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradingStatus value) {
  return stream << quinclas::common::EnumNameTradingStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::OrderStatus value) {
  return stream << quinclas::common::EnumNameOrderStatus(value);
}

std::ostream& operator<<(std::ostream& stream, quinclas::common::time_point_t time_point) {
  if (0 == time_point.time_since_epoch().count())
    return stream;
  return stream << cctz::format(TIME_FORMAT, time_point, TIME_ZONE);
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::Layer& value) {
  return stream << "{"
    "bid_price=" << quinclas::common::Number(value.bid_price) << ", "
    "bid_quantity=" << quinclas::common::Number(value.bid_quantity) << ", "
    "ask_price=" << quinclas::common::Number(value.ask_price) << ", "
    "ask_quantity=" << quinclas::common::Number(value.ask_quantity) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::GatewayStatus& value) {
  return stream << "{"
    "market_data_connection_status=" << value.market_data_connection_status << ", "
    "market_data_login_status=" << value.market_data_login_status << ", "
    "order_management_connection_status=" << value.order_management_connection_status << ", "
    "order_management_login_status=" << value.order_management_login_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::ReferenceData& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.exchange << "\", "
    "tick_size=" << quinclas::common::Number(value.tick_size) << ", "
    "tick_size=" << quinclas::common::Number(value.tick_size) << ", "
    "limit_up=" << quinclas::common::Number(value.limit_up) << ", "
    "limit_down=" << quinclas::common::Number(value.limit_down) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketStatus& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.exchange << "\", "
    "trading_status=" << value.trading_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketByPrice& value) {
  stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "layers=[";
  const size_t length = sizeof(value.depth) / sizeof(value.depth[0]);
  for (size_t index = 0; index < length; ++index) {
    if (index > 0)
      stream << ", ";
    stream << value.depth[index];
  }
  return stream << "]";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::SessionStatistics& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "open=" << quinclas::common::Number(value.open) << ", "
    "high=" << quinclas::common::Number(value.high) << ", "
    "low=" << quinclas::common::Number(value.low) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::DailyStatistics& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "settlement=" << quinclas::common::Number(value.settlement) << ", "
    "open_interest=" << quinclas::common::Number(value.open_interest) << ", "
    "volume=" << quinclas::common::Number(value.volume) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::OrderUpdate& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_id=" << value.order_id << ", "
    "order_template=\"" << value.order_template << "\", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "status=" << value.status << ", "
    "trade_direction=" << value.trade_direction << ", "
    "remaining_quantity=" << quinclas::common::Number(value.remaining_quantity) << ", "
    "traded_quantity=" << quinclas::common::Number(value.traded_quantity) << ", "
    "insert_time=" << value.insert_time << ", "
    "cancel_time=" << value.cancel_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradeUpdate& value) {
  return stream << "{"
    "order_id=" << value.order_id << ", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "external_trade_id=\"" << value.external_trade_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "trade_direction=" << value.trade_direction << ", "
    "quantity=" << quinclas::common::Number(value.quantity) << ", "
    "price=" << quinclas::common::Number(value.price) << ", "
    "trade_time=" << value.trade_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CreateOrderAck& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_id=" << value.order_id << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "reason=\"" << value.reason << "\", "
    "order_template=\"" << value.order_template << "\", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::ModifyOrderAck& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_id=" << value.order_id << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "reason=\"" << value.reason << "\", "
    "order_template=\"" << value.order_template << "\", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "quantity_change=" << quinclas::common::Number(value.quantity_change) << ", "
    "limit_price=" << quinclas::common::Number(value.limit_price) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CancelOrderAck& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_id=" << value.order_id << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "reason=\"" << value.reason << "\", "
    "order_template=\"" << value.order_template << "\", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::MessageInfo& value) {
  return stream << "{"
    "gateway=\"" << value.gateway << "\", "
    "message_id=" << value.message_id << ", "
    "exchange_time=" << value.exchange_time << ", "
    "receive_time=" << value.receive_time << ", "
    "enqueue_time=" << value.enqueue_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::GatewayStatusEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "gateway_status=" << value.gateway_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::ReferenceDataEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "reference_data=" << value.reference_data <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketStatusEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "market_status=" << value.market_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketByPriceEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "market_by_price=" << value.market_by_price <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::SessionStatisticsEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "session_statistics=" << value.session_statistics <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::DailyStatisticsEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "daily_statistics=" << value.daily_statistics <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::OrderUpdateEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "order_update=" << value.order_update <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradeUpdateEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "trade_update=" << value.trade_update <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CreateOrderAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "create_order_ack=" << value.create_order_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::ModifyOrderAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "modify_order_ack=" << value.modify_order_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CancelOrderAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "cancel_order_ack=" << value.cancel_order_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CreateOrder& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_template=" << value.order_template << ", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "direction=" << value.direction << ", "
    "quantity=" << quinclas::common::Number(value.quantity) << ", "
    "limit_price=" << quinclas::common::Number(value.limit_price) << ", "
    "stop_price=" << quinclas::common::Number(value.stop_price) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::ModifyOrder& value) {
  return stream << "{"
    "order_id=" << value.order_id <<
    "quantity_change=" << quinclas::common::Number(value.quantity_change) << ", "
    "limit_price=" << quinclas::common::Number(value.limit_price) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CancelOrder& value) {
  return stream << "{"
    "order_id=" << value.order_id <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::RequestInfo& value) {
  return stream << "{"
    "destination=\"" << value.destination << "\", "
    "trace_source=\"" << value.trace_source << "\", "
    "trace_message_id=" << value.trace_message_id << ", "
    "send_time=" << value.send_time << ", "
    "receive_time=" << value.receive_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CreateOrderRequest& value) {
  return stream << "{"
    "request_info=" << value.request_info << ", "
    "create_order=" << value.create_order <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::ModifyOrderRequest& value) {
  return stream << "{"
    "request_info=" << value.request_info << ", "
    "modify_order=" << value.modify_order <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const quinclas::common::CancelOrderRequest& value) {
  return stream << "{"
    "request_info=" << value.request_info << ", "
    "cancel_order=" << value.cancel_order <<
    "}";
}
