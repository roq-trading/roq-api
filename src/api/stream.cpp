/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "roq/stream.h"

#include <cinttypes>
#include <cmath>
#include <ctime>

#define STACK_BUFFER_SIZE 4096
#define FLOAT_REPR ".8g"  // include 1/256th
#define PASSWORD "<hidden>"

namespace roq {

typedef stream::details::StackWriter<STACK_BUFFER_SIZE> Writer;

namespace stream {

// FIXME(thraneh): this only works if we drop thread_local from cached_time_str
// --> in which case it's too dangerous to explicitly call warm_cache()
void warm_cache() {
  auto time_point = std::chrono::system_clock::from_time_t(0);
  auto one_second = std::chrono::seconds(1);
  for (auto i = 0; i < 86400; ++i, time_point += one_second) {
    stream::details::TimePointStr<decltype(time_point)>(time_point).c_str();
  }
}

namespace details {

// cache

thread_local std::unordered_map<uint32_t, std::string> cached_date_str;
thread_local std::unordered_map<uint32_t, std::string> cached_time_str;

// BasicTimePointStr

BasicTimePointStr::BasicTimePointStr(uint64_t microseconds) {
  auto seconds = static_cast<uint32_t>(microseconds / 1000000);
  auto fraction = microseconds % 1000000;
  auto& date_str = cached_date_str[seconds / 86400];
  auto& time_str = cached_time_str[seconds % 86400];
  if (date_str.empty() || time_str.empty()) {
    std::time_t time = seconds;
    struct tm tm;
    ::gmtime_r(&time, &tm);
    if (date_str.empty())
      StackWriter<11>().strftime("%F", &tm).finish(date_str);
    if (time_str.empty())
      StackWriter<11>().strftime("T%T.", &tm).finish(time_str);
  }
  BasicBufferWriter<sizeof(_result)> writer(_result);
  writer.printf(
      "%s%s%06" PRIu64,
      date_str.c_str(),
      time_str.c_str(),
      fraction).finish();
}

// Layer

template<typename Writer>
Writer& write(Writer& writer, const Layer& value) {
  const char *FORMAT =
    "{"
    "bid_price=%" FLOAT_REPR ", "
    "bid_quantity=%" FLOAT_REPR ", "
    "ask_price=%" FLOAT_REPR ", "
    "ask_quantity=%" FLOAT_REPR
    "}";
  return writer.printf(
      FORMAT,
      value.bid_price,
      value.bid_quantity,
      value.ask_price,
      value.ask_quantity);
}

// Handshake

template<typename Writer>
Writer& write(Writer& writer, const Handshake& value) {
  const char *FORMAT_1 =
    "{"
    "api_version=\"%s\", "
    "application=\"%s\", "
    "hostname=\"%s\", "
    "pid=%" PRIu32 ", "
    "uuid=\"%s\", "
    "login=\"%s\", "
    "password=" PASSWORD ", "
    "symbols={";
  writer.printf(
      FORMAT_1,
      value.api_version,
      value.application,
      value.hostname,
      value.pid,
      value.uuid,
      value.login);
  bool first = true;
  for (auto& iter : value.symbols) {
    if (first) {
      first = false;
      writer.printf(", ");
    }
    writer.printf("\"%s\"", iter);
  }
  writer.printf("}, accounts={");
  first = true;
  for (auto& iter : value.accounts) {
    if (first) {
      first = false;
      writer.printf(", ");
    }
    writer.printf("\"%s\"", iter);
  }
  const char *FORMAT_2 =
    "}, "
    "client_type=%s, "
    "shmem_name=\"%s\", "
    "shmem_size=%" PRIu32 ", "
    "shmem_index=%" PRIu32
    "}";
  return writer.printf(
      FORMAT_2,
      EnumNameClientType(value.client_type),
      value.shmem_name,
      value.shmem_size,
      value.shmem_index);
}

// HandshakeAck

template<typename Writer>
Writer& write(Writer& writer, const HandshakeAck& value) {
  const char *FORMAT =
    "{"
    "api_version=\"%s\", "
    "application=\"%s\", "
    "hostname=\"%s\", "
    "pid=%" PRIu32 ", "
    "failure=%s, "
    "failure_reason=\"%s\", "
    "server_uuid=\"%s\", "
    "server_name=\"%s\", "
    "shmem_name=\"%s\", "
    "shmem_size=%" PRIu32 ", "
    "shmem_index=%" PRIu32
    "}";
  return writer.printf(
      FORMAT,
      value.api_version,
      value.application,
      value.hostname,
      value.pid,
      value.failure ? "true" : "false",
      value.failure_reason,
      value.server_uuid,
      value.server_name,
      value.shmem_name,
      value.shmem_size,
      value.shmem_index);
}

// Heartbeat

template<typename Writer>
Writer& write(Writer& writer, const Heartbeat& value) {
  const char *FORMAT =
    "{"
    "opaque=%" PRIu64
    "}";
  return writer.printf(
      FORMAT,
      value.opaque);
}

// HeartbeatAck

template<typename Writer>
Writer& write(Writer& writer, const HeartbeatAck& value) {
  const char *FORMAT =
    "{"
    "opaque=%" PRIu64
    "}";
  return writer.printf(
      FORMAT,
      value.opaque);
}

// DownloadBegin

template<typename Writer>
Writer& write(Writer& writer, const DownloadBegin& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\""
    "}";
  return writer.printf(
      FORMAT,
      value.account);
}

// DownloadEnd

template<typename Writer>
Writer& write(Writer& writer, const DownloadEnd& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "max_order_id=%" PRIu32
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.max_order_id);
}

// MarketDataStatus

template<typename Writer>
Writer& write(Writer& writer, const MarketDataStatus& value) {
  const char *FORMAT =
    "{"
    "status=%s"
    "}";
  return writer.printf(
      FORMAT,
      EnumNameGatewayStatus(value.status));
}

// OrderManagerStatus

template<typename Writer>
Writer& write(Writer& writer, const OrderManagerStatus& value) {
  const char *FORMAT =
    "{"
    "accounts=\"%s\", "
    "status=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      EnumNameGatewayStatus(value.status));
}

template<typename Writer>
Writer& write(Writer& writer, const SessionStatistics& value) {
  TimePointStr<decltype(value.exchange_time)> exchange_time(value.exchange_time);
  const char *FORMAT =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "pre_open_interest=%" FLOAT_REPR ", "
    "pre_settlement_price=%" FLOAT_REPR ", "
    "pre_close_price=%" FLOAT_REPR ", "
    "highest_traded_price=%" FLOAT_REPR ", "
    "lowest_traded_price=%" FLOAT_REPR ", "
    "upper_limit_price=%" FLOAT_REPR ", "
    "lower_limit_price=%" FLOAT_REPR ", "
    "exchange_time=%s, "
    "channel=%" PRIu16
    "}";
  return writer.printf(
      FORMAT,
      value.exchange,
      value.symbol,
      value.pre_open_interest,
      value.pre_settlement_price,
      value.pre_close_price,
      value.highest_traded_price,
      value.lowest_traded_price,
      value.upper_limit_price,
      value.lower_limit_price,
      exchange_time.c_str(),
      value.channel);
}

template<typename Writer>
Writer& write(Writer& writer, const DailyStatistics& value) {
  TimePointStr<decltype(value.exchange_time)> exchange_time(value.exchange_time);
  const char *FORMAT =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "open_price=%" FLOAT_REPR ", "
    "settlement_price=%" FLOAT_REPR ", "
    "close_price=%" FLOAT_REPR ", "
    "open_interest=%" FLOAT_REPR ", "
    "exchange_time=%s, "
    "channel=%" PRIu16
    "}";
  return writer.printf(
      FORMAT,
      value.exchange,
      value.symbol,
      value.open_price,
      value.settlement_price,
      value.close_price,
      value.open_interest,
      exchange_time.c_str(),
      value.channel);
}

// MarketByPrice

template<typename Writer>
Writer& write(Writer& writer, const MarketByPrice& value) {
  TimePointStr<decltype(value.exchange_time)> exchange_time(value.exchange_time);
  const char *FORMAT_1 =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "layers=[";
  writer.printf(
      FORMAT_1,
      value.exchange,
      value.symbol);
  auto length = sizeof(value.depth) / sizeof(value.depth[0]);
  for (auto i = 0; i < length; ++i) {
    if (i)
      writer.printf(", ");
    write(writer, value.depth[i]);
  }
  const char *FORMAT_2 =
    "], "
    "total_bid_volume=%" FLOAT_REPR ", "
    "total_ask_volume=%" FLOAT_REPR ", "
    "exchange_time=%s, "
    "channel=%" PRIu16
    "}";
  return writer.printf(
      FORMAT_2,
      exchange_time.c_str(),
      value.channel);
}

// TradeSummary

template<typename Writer>
Writer& write(Writer& writer, const TradeSummary& value) {
  TimePointStr<decltype(value.exchange_time)> exchange_time(value.exchange_time);
  const char *FORMAT =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "price=%" FLOAT_REPR ", "
    "volume=%" FLOAT_REPR ", "
    "turnover=%" FLOAT_REPR ", "
    "side=%s, "
    "exchange_time=%s, "
    "channel=%" PRIu16
    "}";
  return writer.printf(
      FORMAT,
      value.exchange,
      value.symbol,
      value.price,
      value.volume,
      value.turnover,
      EnumNameSide(value.side),
      exchange_time.c_str(),
      value.channel);
}

// ReferenceData

template<typename Writer>
Writer& write(Writer& writer, const ReferenceData& value) {
  const char *FORMAT =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "tick_size=%" FLOAT_REPR ", "
    "limit_up=%" FLOAT_REPR ", "
    "limit_down=%" FLOAT_REPR ", "
    "multiplier=%f "
    "}";
  return writer.printf(
      FORMAT,
      value.exchange,
      value.symbol,
      value.tick_size,
      value.limit_up,
      value.limit_down,
      value.multiplier);
}

// MarketStatus

template<typename Writer>
Writer& write(Writer& writer, const MarketStatus& value) {
  const char *FORMAT =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "trading_status=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.exchange,
      value.symbol,
      EnumNameTradingStatus(value.trading_status));
}

// PositionUpdate

template<typename Writer>
Writer& write(Writer& writer, const PositionUpdate& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "side=%s, "
    "position=%" FLOAT_REPR ", "
    "yesterday=%" FLOAT_REPR ", "
    "last_order_local_id=%" PRIu32 ", "
    "last_trade_id=%" PRIu32
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.exchange,
      value.symbol,
      EnumNameSide(value.side),
      value.position,
      value.yesterday,
      value.last_order_local_id,
      value.last_trade_id);
}

// OrderUpdate

template<typename Writer>
Writer& write(Writer& writer, const OrderUpdate& value) {
  TimePointStr<decltype(value.insert_time)> insert_time(value.insert_time);
  TimePointStr<decltype(value.cancel_time)> cancel_time(value.cancel_time);
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "order_id=%" PRIu32 ", "
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "order_status=%s, "
    "side=%s, "
    "remaining_quantity=%" FLOAT_REPR ", "
    "traded_quantity=%" FLOAT_REPR ", "
    "position_effect=%s, "
    "order_template=\"%s\", "
    "insert_time=%s, "
    "cancel_time=%s, "
    "order_local_id=%" PRIu32 ", "
    "order_external_id=\"%s\""
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.order_id,
      value.exchange,
      value.symbol,
      EnumNameOrderStatus(value.order_status),
      EnumNameSide(value.side),
      value.remaining_quantity,
      value.traded_quantity,
      EnumNamePositionEffect(value.position_effect),
      value.order_template,
      insert_time.c_str(),
      cancel_time.c_str(),
      value.order_local_id,
      value.order_external_id);
}

// TradeUpdate

template<typename Writer>
Writer& write(Writer& writer, const TradeUpdate& value) {
  TimePointStr<decltype(value.trade_time)> trade_time(value.trade_time);
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "trade_id=%" PRIu32 ", "
    "order_id=%" PRIu32 ", "
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "side=%s, "
    "quantity=%" FLOAT_REPR ", "
    "price=%" FLOAT_REPR ", "
    "position_effect=%s, "
    "order_template=\"%s\", "
    "trade_time=%s, "
    "order_external_id=\"%s\", "
    "trade_external_id=\"%s\""
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.trade_id,
      value.order_id,
      value.exchange,
      value.symbol,
      EnumNameSide(value.side),
      value.quantity,
      value.price,
      EnumNamePositionEffect(value.position_effect),
      value.order_template,
      trade_time.c_str(),
      value.order_external_id,
      value.trade_external_id);
}

// CreteOrder

template<typename Writer>
Writer& write(Writer& writer, const CreateOrder& value) {
  const char *FORMAT =
      "{"
      "account=\"%s\", "
      "order_id=%" PRIu32 ", "
      "exchange=\"%s\", "
      "symbol=\"%s\", "
      "side=%s, "
      "quantity=%" FLOAT_REPR ", "
      "order_type=%s, "
      "limit_price=%" FLOAT_REPR ", "
      "time_in_force=%s, "
      "position_effect=%s, "
      "order_template=\"%s\""
      "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.order_id,
      value.exchange,
      value.symbol,
      EnumNameSide(value.side),
      value.quantity,
      EnumNameOrderType(value.order_type),
      value.limit_price,
      EnumNameTimeInForce(value.time_in_force),
      EnumNamePositionEffect(value.position_effect),
      value.order_template);
}

// ModifyOrder

template<typename Writer>
Writer& write(Writer& writer, const ModifyOrder& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "order_id=%" PRIu32 ", "
    "quantity_change=%" FLOAT_REPR ", "
    "limit_price=%" FLOAT_REPR
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.order_id,
      value.quantity_change,
      value.limit_price);
}

// CancelOrder

template<typename Writer>
Writer& write(Writer& writer, const CancelOrder& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "order_id=%" PRIu32
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.order_id);
}

// CreateOrderAck

template<typename Writer>
Writer& write(Writer& writer, const CreateOrderAck& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "order_id=%" PRIu32 ", "
    "failure=%s, "
    "reason=\"%s\", "
    "order_local_id=%" PRIu32 ", "
    "order_external_id=\"%s\""
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.order_id,
      value.failure ? "true" : "false",
      value.reason,
      value.order_local_id,
      value.order_external_id);
}

// ModifyOrderAck

template<typename Writer>
Writer& write(Writer& writer, const ModifyOrderAck& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "order_id=%" PRIu32 ", "
    "failure=%s, "
    "reason=\"%s\", "
    "order_local_id=%" PRIu32 ", "
    "order_external_id=\"%s\""
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.order_id,
      value.failure ? "true" : "false",
      value.reason,
      value.order_local_id,
      value.order_external_id);
}

// CancelOrderAck

template<typename Writer>
Writer& write(Writer& writer, const CancelOrderAck& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "order_id=%" PRIu32 ", "
    "failure=%s, "
    "reason=\"%s\", "
    "order_local_id=%" PRIu32 ", "
    "order_external_id=\"%s\""
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.order_id,
      value.failure ? "true" : "false",
      value.reason,
      value.order_local_id,
      value.order_external_id);
}

// MessageInfo

template <typename Writer>
Writer& write(Writer& writer, const MessageInfo& value) {
  auto routing_latency = static_cast<int32_t>(
      std::chrono::duration_cast<std::chrono::microseconds>(
          value.routing_latency).count());
  TimePointStr<decltype(value.source_create_time)> source_create_time(value.source_create_time);
  TimePointStr<decltype(value.client_receive_time)> client_receive_time(value.client_receive_time);
  const char *FORMAT =
    "{"
    "source=\"%s\", "
    "source_create_time=%s, "
    "client_receive_time=%s, "
    "routing_latency=%" PRIu32 ", "
    "from_cache=%s, "
    "is_last=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.source,
      source_create_time.c_str(),
      client_receive_time.c_str(),
      routing_latency,
      value.from_cache ? "true" : "false",
      value.is_last ? "true" : "false");
}

// ConnectionStatusEvent

template <typename Writer>
Writer& write(Writer& writer, const ConnectionStatusEvent& value) {
  const char *FORMAT =
    "{"
    "source=\"%s\", "
    "connection_status=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.source,
      EnumNameConnectionStatus(value.connection_status));
}

// BatchBeginEvent

template <typename Writer>
Writer& write(Writer& writer, const BatchBeginEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf("}");
}

// BatchEndEvent

template <typename Writer>
Writer& write(Writer& writer, const BatchEndEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const HandshakeEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", handshake=")
    .print(value.handshake)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const HandshakeAckEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", handshake_ack=")
    .print(value.handshake_ack)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const HeartbeatEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", heartbeat=")
    .print(value.heartbeat)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const HeartbeatAckEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", heartbeat_ack=")
    .print(value.heartbeat_ack)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const DownloadBeginEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", download_begin=")
    .print(value.download_begin)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const DownloadEndEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", download_end=")
    .print(value.download_end)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const MarketDataStatusEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", market_data_status=")
    .print(value.market_data_status)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const OrderManagerStatusEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", order_manager_status=")
    .print(value.order_manager_status)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const SessionStatisticsEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", session_statistics=")
    .print(value.session_statistics)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const DailyStatisticsEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", daily_statistics=")
    .print(value.daily_statistics)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const MarketByPriceEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", market_by_price=")
    .print(value.market_by_price)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const TradeSummaryEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", trade_summary=")
    .print(value.trade_summary)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const ReferenceDataEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", reference_data=")
    .print(value.reference_data)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const MarketStatusEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", market_status=")
    .print(value.market_status)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const PositionUpdateEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", position_update=")
    .print(value.position_update)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const OrderUpdateEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", order_update=")
    .print(value.order_update)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const TradeUpdateEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", trade_update=")
    .print(value.trade_update)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const CreateOrderEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", create_order=")
    .print(value.create_order)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const ModifyOrderEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", modify_order=")
    .print(value.modify_order)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const CancelOrderEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", cancel_order=")
    .print(value.cancel_order)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const CreateOrderAckEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", create_order_ack=")
    .print(value.create_order_ack)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const ModifyOrderAckEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", modify_order_ack=")
    .print(value.modify_order_ack)
    .printf("}");
}

template <typename Writer>
Writer& write(Writer& writer, const CancelOrderAckEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", cancel_order_ack=")
    .print(value.cancel_order_ack)
    .printf("}");
}

}  // namespace details

}  // namespace stream

// ...

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

template <>
std::ostream& operator<<(std::ostream& stream, const Vector<std::string> value) {
  stream << "[";
  bool insert_delimiter = false;
  for (const auto& iter : value._value) {
    if (insert_delimiter)
      stream << ", ";
    stream << "\"" << iter << "\"";
    insert_delimiter = true;
  }
  return stream << "]";
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Set<T> value) {
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

template <>
std::ostream& operator<<(std::ostream& stream, const Set<std::string> value) {
  stream << "[";
  bool insert_delimiter = false;
  for (const auto& iter : value._value) {
    if (insert_delimiter)
      stream << ", ";
    stream << "\"" << iter << "\"";
    insert_delimiter = true;
  }
  return stream << "]";
}

// enums

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

// Layer

std::ostream& operator<<(std::ostream& stream, const Layer& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// Handshake

std::ostream& operator<<(std::ostream& stream, const Handshake& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// HandshakeAck

std::ostream& operator<<(std::ostream& stream, const HandshakeAck& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// Heartbeat

std::ostream& operator<<(std::ostream& stream, const Heartbeat& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// HeartbeatAck

std::ostream& operator<<(std::ostream& stream, const HeartbeatAck& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DownloadBegin

std::ostream& operator<<(std::ostream& stream, const DownloadEnd& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DownloadEnd

std::ostream& operator<<(std::ostream& stream, const DownloadBegin& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MarketDataStatus

std::ostream& operator<<(std::ostream& stream, const MarketDataStatus& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// OrderManagerStatus

std::ostream& operator<<(std::ostream& stream, const OrderManagerStatus& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// SessionStatistics

std::ostream& operator<<(std::ostream& stream, const SessionStatistics& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DailyStatistics

std::ostream& operator<<(std::ostream& stream, const DailyStatistics& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MarketByPrice

std::ostream& operator<<(std::ostream& stream, const MarketByPrice& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// TradeSummary

std::ostream& operator<<(std::ostream& stream, const TradeSummary& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// ReferenceData

std::ostream& operator<<(std::ostream& stream, const ReferenceData& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MarketStatus

std::ostream& operator<<(std::ostream& stream, const MarketStatus& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// PositionUpdate

std::ostream& operator<<(std::ostream& stream, const PositionUpdate& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// OrderUpdate

std::ostream& operator<<(std::ostream& stream, const OrderUpdate& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// TradeUpdate

std::ostream& operator<<(std::ostream& stream, const TradeUpdate& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CreateOrder

std::ostream& operator<<(std::ostream& stream, const CreateOrder& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// ModifyOrder

std::ostream& operator<<(std::ostream& stream, const ModifyOrder& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CancelOrder

std::ostream& operator<<(std::ostream& stream, const CancelOrder& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CreateOrderAck

std::ostream& operator<<(std::ostream& stream, const CreateOrderAck& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// ModifyOrderAck

std::ostream& operator<<(std::ostream& stream, const ModifyOrderAck& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CancelOrderAck

std::ostream& operator<<(std::ostream& stream, const CancelOrderAck& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MessageInfo

std::ostream& operator<<(std::ostream& stream, const MessageInfo& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// ConnectionStatusEvent

std::ostream& operator<<(std::ostream& stream, const ConnectionStatusEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// BatchBeginEvent

std::ostream& operator<<(std::ostream& stream, const BatchBeginEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// BatchEndEvent

std::ostream& operator<<(std::ostream& stream, const BatchEndEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// HandshakeEvent

std::ostream& operator<<(std::ostream& stream, const HandshakeEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// HandshakeAckEvent

std::ostream& operator<<(std::ostream& stream, const HandshakeAckEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// HeartbeatEvent

std::ostream& operator<<(std::ostream& stream, const HeartbeatEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// HeartbeatAckEvent

std::ostream& operator<<(std::ostream& stream, const HeartbeatAckEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DownloadBeginEvent

std::ostream& operator<<(std::ostream& stream, const DownloadBeginEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DownloadEndEvent

std::ostream& operator<<(std::ostream& stream, const DownloadEndEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MarketDataStatusEvent

std::ostream& operator<<(std::ostream& stream, const MarketDataStatusEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// OrderManagerStatusEvent

std::ostream& operator<<(std::ostream& stream, const OrderManagerStatusEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// SessionStatisticsEvent

std::ostream& operator<<(std::ostream& stream, const SessionStatisticsEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DailyStatisticsEvent

std::ostream& operator<<(std::ostream& stream, const DailyStatisticsEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MarketByPriceEvent

std::ostream& operator<<(std::ostream& stream, const MarketByPriceEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// TradeSummaryEvent

std::ostream& operator<<(std::ostream& stream, const TradeSummaryEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// ReferenceDataEvent

std::ostream& operator<<(std::ostream& stream, const ReferenceDataEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MarketStatusEvent

std::ostream& operator<<(std::ostream& stream, const MarketStatusEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// PositionUpdateEvent

std::ostream& operator<<(std::ostream& stream, const PositionUpdateEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// OrderUpdateEvent

std::ostream& operator<<(std::ostream& stream, const OrderUpdateEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// TradeUpdateEvent

std::ostream& operator<<(std::ostream& stream, const TradeUpdateEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CreateOrderEvent

std::ostream& operator<<(std::ostream& stream, const CreateOrderEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// ModifyOrderEvent

std::ostream& operator<<(std::ostream& stream, const ModifyOrderEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CancelOrderEvent

std::ostream& operator<<(std::ostream& stream, const CancelOrderEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CreateOrderAckEvent

std::ostream& operator<<(std::ostream& stream, const CreateOrderAckEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// ModifyOrderAckEvent

std::ostream& operator<<(std::ostream& stream, const ModifyOrderAckEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// CancelOrderAckEvent

std::ostream& operator<<(std::ostream& stream, const CancelOrderAckEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

}  // namespace roq
