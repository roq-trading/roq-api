/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <cctz/time_zone.h>
#include <double-conversion/double-conversion.h>

#include <cmath>

#include "roq/stream.h"

namespace roq {

namespace {
const double_conversion::DoubleToStringConverter DOUBLE_CONVERTER(
    double_conversion::DoubleToStringConverter::UNIQUE_ZERO,
    "", "", 'e', -6, 21, 6, 0
);
const char *TIME_FORMAT = "%E4Y-%m-%dT%H:%M:%E6S";
const auto TIME_ZONE = cctz::utc_time_zone();
const char *PASSWORD = "<hidden>";
}

std::ostream& operator<<(std::ostream& stream, const Number value) {
  char buffer[32];
  double_conversion::StringBuilder builder(buffer, sizeof(buffer));
  if (DOUBLE_CONVERTER.ToShortest(value._value, &builder))
    return stream << builder.Finalize();
  else
    return stream << "####";
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Vector<T> value) {
  stream << "[";
  bool insert_delimiter = false;
  for (const auto& iter : value._value) {
    if (insert_delimiter)
      stream << ", ";
    stream << iter;
    insert_delimiter = true;
  }
  return stream << "]";
}

std::ostream& operator<<(std::ostream& stream, const ConnectionStatus value) {
  return stream << EnumNameConnectionStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const GatewayStatus value) {
  return stream << EnumNameGatewayStatus(value);
}

std::ostream& operator<<(std::ostream& stream, const Side value) {
  return stream << EnumNameSide(value);
}

std::ostream& operator<<(std::ostream& stream, const OrderType value) {
  return stream << EnumNameOrderType(value);
}

std::ostream& operator<<(std::ostream& stream, const TimeInForce value) {
  return stream << EnumNameTimeInForce(value);
}

std::ostream& operator<<(std::ostream& stream, const PositionEffect value) {
  return stream << EnumNamePositionEffect(value);
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

std::ostream& operator<<(std::ostream& stream, const Handshake& value) {
  return stream << "{"
    "api_version=\"" << value.api_version << "\", "
    "application=\"" << value.application << "\", "
    "hostname=\"" << value.hostname << "\", "
    "pid=" << value.pid << ", "
    "uuid=\"" << value.uuid << "\", "
    "login=\"" << value.login << "\", "
    "password=" << PASSWORD << ", "
    "subscriptions=" << Vector<decltype(value.subscriptions)::value_type>(value.subscriptions) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const HandshakeAck& value) {
  return stream << "{"
    "api_version=\"" << value.api_version << "\", "
    "application=\"" << value.application << "\", "
    "hostname=\"" << value.hostname << "\", "
    "pid=" << value.pid << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "failure_reason=\"" << value.failure_reason << "\", "
    "server_uuid=\"" << value.server_uuid << "\", "
    "server_name=\"" << value.server_name << "\", "
    "shmem_s2c_name=\"" << value.shmem_s2c_name << "\", "
    "shmem_s2c_size=" << value.shmem_s2c_size << ", "
    "shmem_c2s_name=\"" << value.shmem_c2s_name << "\", "
    "shmem_c2s_size=" << value.shmem_c2s_size <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const Heartbeat& value) {
  return stream << "{"
    "opaque=" << value.opaque <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const HeartbeatAck& value) {
  return stream << "{"
    "opaque=" << value.opaque <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const DownloadBegin& value) {
  return stream << "{}";
}

std::ostream& operator<<(std::ostream& stream, const DownloadEnd& value) {
  return stream << "{"
    "max_order_id=" << value.max_order_id <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketDataStatus& value) {
  return stream << "{"
    "status=" << value.status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const OrderManagerStatus& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "status=" << value.status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketByPrice& value) {
  stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "layers=[";
  const size_t length = sizeof(value.depth) / sizeof(value.depth[0]);
  for (size_t index = 0; index < length; ++index) {
    if (index > 0)
      stream << ", ";
    stream << value.depth[index];
  }
  return stream << "], "
    "exchange_time=" << value.exchange_time << ", "
    "channel=" << value.channel <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const TradeSummary& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "price=" << Number(value.price) << ", "
    "volume=" << Number(value.volume) << ", "
    "turnover=" << Number(value.turnover) << ", "
    "side=" << value.side << ", "
    "exchange_time=" << value.exchange_time << ", "
    "channel=" << value.channel <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ReferenceData& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "tick_size=" << Number(value.tick_size) << ", "
    "limit_up=" << Number(value.limit_up) << ", "
    "limit_down=" << Number(value.limit_down) << ", "
    "multiplier=" << Number(value.multiplier) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketStatus& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "trading_status=" << value.trading_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const PositionUpdate& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "side=" << value.side << ", "
    "position=" << Number(value.position) << ", "
    "yesterday=" << Number(value.yesterday) << ", "
    "last_order_id=" << value.last_order_id << ", "
    "last_trade_id=" << value.last_trade_id <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const OrderUpdate& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "order_id=" << value.order_id << ", "
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "order_status=" << value.order_status << ", "
    "side=" << value.side << ", "
    "remaining_quantity=" << Number(value.remaining_quantity) << ", "
    "traded_quantity=" << Number(value.traded_quantity) << ", "
    "position_effect=" << value.position_effect << ", "
    "order_template=\"" << value.order_template << "\", "
    "insert_time=" << value.insert_time << ", "
    "cancel_time=" << value.cancel_time << ", "
    "order_local_id=" << value.order_local_id << ", "
    "order_external_id=\"" << value.order_external_id << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const TradeUpdate& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "trade_id=" << value.trade_id << ", "
    "order_id=" << value.order_id << ", "
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "side=" << value.side << ", "
    "quantity=" << Number(value.quantity) << ", "
    "price=" << Number(value.price) << ", "
    "position_effect=" << value.position_effect << ", "
    "order_template=\"" << value.order_template << "\", "
    "trade_time=" << value.trade_time << ", "
    "order_local_id=" << value.order_local_id << ", "
    "order_external_id=\"" << value.order_external_id << "\", "
    "trade_external_id=\"" << value.trade_external_id << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CreateOrder& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "order_id=" << value.order_id << ", "
    "exchange=\"" << value.exchange << "\", "
    "symbol=\"" << value.symbol << "\", "
    "side=" << value.side << ", "
    "quantity=" << Number(value.quantity) << ", "
    "order_type=" << value.order_type << ", "
    "limit_price=" << Number(value.limit_price) << ", "
    "time_in_force=" << value.time_in_force << ", "
    "position_effect=" << value.position_effect << ", "
    "order_template=\"" << value.order_template << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ModifyOrder& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "order_id=" << value.order_id <<
    "quantity_change=" << Number(value.quantity_change) << ", "
    "limit_price=" << Number(value.limit_price) <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CancelOrder& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "order_id=" << value.order_id <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CreateOrderAck& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "order_id=" << value.order_id << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "reason=\"" << value.reason << "\", "
    "order_local_id=" << value.order_local_id << ", "
    "order_external_id=\"" << value.order_external_id << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ModifyOrderAck& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "order_id=" << value.order_id << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "reason=\"" << value.reason << "\", "
    "order_local_id=" << value.order_local_id << ", "
    "order_external_id=\"" << value.order_external_id << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CancelOrderAck& value) {
  return stream << "{"
    "account=\"" << value.account << "\", "
    "order_id=" << value.order_id << ", "
    "failure=" << (value.failure ? "true" : "false") << ", "
    "reason=\"" << value.reason << "\", "
    "order_local_id=" << value.order_local_id << ", "
    "order_external_id=\"" << value.order_external_id << "\""
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MessageInfo& value) {
  auto routing_latency = std::chrono::duration_cast<std::chrono::microseconds>(
      value.routing_latency).count();
  return stream << "{"
    "source=\"" << value.source << "\", "
    "source_create_time=" << value.source_create_time << ", "
    "client_receive_time=" << value.client_receive_time << ", "
    "routing_latency=" << routing_latency << ", "
    "from_cache=" << (value.from_cache ? "true" : "false") << ", "
    "is_last=" << (value.is_last ? "true" : "false") <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ConnectionStatusEvent& value) {
  return stream << "{"
    "source=\"" << value.source << "\", "
    "connection_status=" << value.connection_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const BatchBeginEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const BatchEndEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const HandshakeEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "handshake=" << value.handshake <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const HandshakeAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "handshake_ack=" << value.handshake_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const HeartbeatEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "heartbeat=" << value.heartbeat <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const HeartbeatAckEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "heartbeat_ack=" << value.heartbeat_ack <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const DownloadBeginEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "download_begin_event=" << value.download_begin <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const DownloadEndEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "download_end_event=" << value.download_end <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketDataStatusEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "market_data_status=" << value.market_data_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const OrderManagerStatusEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "order_manager_status=" << value.order_manager_status <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const MarketByPriceEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "market_by_price=" << value.market_by_price <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const TradeSummaryEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "trade_summary=" << value.trade_summary <<
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

std::ostream& operator<<(std::ostream& stream, const PositionUpdateEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "position_update=" << value.position_update <<
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

std::ostream& operator<<(std::ostream& stream, const CreateOrderEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "create_order=" << value.create_order <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const ModifyOrderEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "modify_order=" << value.modify_order <<
    "}";
}

std::ostream& operator<<(std::ostream& stream, const CancelOrderEvent& value) {
  return stream << "{"
    "message_info=" << value.message_info << ", "
    "cancel_order=" << value.cancel_order <<
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

}  // namespace roq
