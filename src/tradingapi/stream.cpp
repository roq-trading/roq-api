/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <cctz/time_zone.h>
#include <double-conversion/double-conversion.h>

#include <cmath>

#include "quinclas/tradingapi.h"

namespace quinclas {
namespace common {

namespace {
const double_conversion::DoubleToStringConverter DOUBLE_CONVERTER(
    double_conversion::DoubleToStringConverter::UNIQUE_ZERO,
    "", "", 'e', -6, 21, 6, 0
);
const char *TIME_FORMAT = "%E4Y-%m-%dT%H:%M:%E6S";
const auto TIME_ZONE = cctz::utc_time_zone();
}

std::ostream& operator<<(std::ostream& stream, const Number number) {
  char buffer[32];
  double_conversion::StringBuilder builder(buffer, sizeof(buffer));
  if (DOUBLE_CONVERTER.ToShortest(number._value, &builder))
    return stream << builder.Finalize();
  else
    return stream << "####";
}

std::ostream& operator<<(std::ostream& stream, const ConnectionStatus value) {
  return stream << EnumNameConnectionStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const LoginStatus value) {
  return stream << EnumNameLoginStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const TradeDirection value) {
  return stream << EnumNameTradeDirection(value);
}

std::ostream& operator<<(std::ostream& stream, const TradingStatus value) {
  return stream << EnumNameTradingStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const OrderStatus value) {
  return stream << EnumNameOrderStatus(value);
}

std::ostream& operator<<(std::ostream& stream, time_point_t time_point) {
  if (0 == time_point.time_since_epoch().count())
    return stream;
  return stream << cctz::format(TIME_FORMAT, time_point, TIME_ZONE);
}

std::ostream& operator<<(std::ostream& stream, const Layer& value) {
  return stream << "{"
    "bid_price=" << Number(value.bid_price) << ", "
    "bid_quantity=" << Number(value.bid_quantity) << ", "
    "ask_price=" << Number(value.ask_price) << ", "
    "ask_quantity=" << Number(value.ask_quantity) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const GatewayStatus& value) {
  return stream << "{"
    "market_data_connection_status=" << value.market_data_connection_status << ", "
    "market_data_login_status=" << value.market_data_login_status << ", "
    "order_management_connection_status=" << value.order_management_connection_status << ", "
    "order_management_login_status=" << value.order_management_login_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ReferenceData& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.exchange << "\", "
    "tick_size=" << Number(value.tick_size) << ", "
    "tick_size=" << Number(value.tick_size) << ", "
    "limit_up=" << Number(value.limit_up) << ", "
    "limit_down=" << Number(value.limit_down) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketStatus& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.exchange << "\", "
    "trading_status=" << value.trading_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketByPrice& value) {
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

std::ostream& operator<<(std::ostream& stream, const SessionStatistics& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "open=" << Number(value.open) << ", "
    "high=" << Number(value.high) << ", "
    "low=" << Number(value.low) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const DailyStatistics& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "settlement=" << Number(value.settlement) << ", "
    "open_interest=" << Number(value.open_interest) << ", "
    "volume=" << Number(value.volume) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const OrderUpdate& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_id=" << value.order_id << ", "
    "order_template=\"" << value.order_template << "\", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "status=" << value.status << ", "
    "trade_direction=" << value.trade_direction << ", "
    "remaining_quantity=" << Number(value.remaining_quantity) << ", "
    "traded_quantity=" << Number(value.traded_quantity) << ", "
    "insert_time=" << value.insert_time << ", "
    "cancel_time=" << value.cancel_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const TradeUpdate& value) {
  return stream << "{"
    "order_id=" << value.order_id << ", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "external_trade_id=\"" << value.external_trade_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "trade_direction=" << value.trade_direction << ", "
    "quantity=" << Number(value.quantity) << ", "
    "price=" << Number(value.price) << ", "
    "trade_time=" << value.trade_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CreateOrderAck& value) {
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

std::ostream& operator<<(std::ostream& stream, const ModifyOrderAck& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_id=" << value.order_id << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "reason=\"" << value.reason << "\", "
    "order_template=\"" << value.order_template << "\", "
    "external_order_id=\"" << value.external_order_id << "\", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "quantity_change=" << Number(value.quantity_change) << ", "
    "limit_price=" << Number(value.limit_price) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CancelOrderAck& value) {
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

std::ostream& operator<<(std::ostream& stream, const MessageInfo& value) {
  return stream << "{"
    "gateway=\"" << value.gateway << "\", "
    "message_id=" << value.message_id << ", "
    "exchange_time=" << value.exchange_time << ", "
    "receive_time=" << value.receive_time << ", "
    "enqueue_time=" << value.enqueue_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const GatewayStatusEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "gateway_status=" << value.gateway_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ReferenceDataEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "reference_data=" << value.reference_data <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketStatusEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "market_status=" << value.market_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketByPriceEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "market_by_price=" << value.market_by_price <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const SessionStatisticsEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "session_statistics=" << value.session_statistics <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const DailyStatisticsEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "daily_statistics=" << value.daily_statistics <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const OrderUpdateEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "order_update=" << value.order_update <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const TradeUpdateEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "trade_update=" << value.trade_update <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CreateOrderAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "create_order_ack=" << value.create_order_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ModifyOrderAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "modify_order_ack=" << value.modify_order_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CancelOrderAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "cancel_order_ack=" << value.cancel_order_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CreateOrder& value) {
  return stream << "{"
    "opaque=" << value.opaque << ", "
    "order_template=" << value.order_template << ", "
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "direction=" << value.direction << ", "
    "quantity=" << Number(value.quantity) << ", "
    "limit_price=" << Number(value.limit_price) << ", "
    "stop_price=" << Number(value.stop_price) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ModifyOrder& value) {
  return stream << "{"
    "order_id=" << value.order_id <<
    "quantity_change=" << Number(value.quantity_change) << ", "
    "limit_price=" << Number(value.limit_price) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CancelOrder& value) {
  return stream << "{"
    "order_id=" << value.order_id <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const RequestInfo& value) {
  return stream << "{"
    "destination=\"" << value.destination << "\", "
    "trace_source=\"" << value.trace_source << "\", "
    "trace_message_id=" << value.trace_message_id << ", "
    "send_time=" << value.send_time << ", "
    "receive_time=" << value.receive_time <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CreateOrderRequest& value) {
  return stream << "{"
    "request_info=" << value.request_info << ", "
    "create_order=" << value.create_order <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ModifyOrderRequest& value) {
  return stream << "{"
    "request_info=" << value.request_info << ", "
    "modify_order=" << value.modify_order <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CancelOrderRequest& value) {
  return stream << "{"
    "request_info=" << value.request_info << ", "
    "cancel_order=" << value.cancel_order <<
    "}";
}

}  // namespace common
}  // namespace quinclas
