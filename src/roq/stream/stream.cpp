/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/stream.h"

#include <cinttypes>
#include <cmath>
#include <ctime>

#include "roq/stream/utility.h"

#define FLOAT_REPR ".8g"  // include 1/256th
#define PASSWORD "<hidden>"

namespace roq {

constexpr size_t STACK_BUFFER_SIZE = 4096;

typedef StackWriter<STACK_BUFFER_SIZE> Writer;

// Update

template <typename Writer>
Writer& write(Writer& writer, const MBPUpdate& value) {
  const char *FORMAT =
    "{"
    "price=%" FLOAT_REPR ", "
    "quantity=%" FLOAT_REPR ", "
    "action=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.price,
      value.quantity,
      EnumNameUpdateAction(value.action));
}

// MessageInfo

template <typename Writer>
Writer& write(Writer& writer, const MessageInfo& value) {
  stream::ClockStr<decltype(value.receive_time_utc)> receive_time_utc(value.receive_time_utc);
  stream::ClockStr<decltype(value.receive_time)> receive_time(value.receive_time);
  stream::ClockStr<decltype(value.source_send_time)> source_send_time(value.source_send_time);
  stream::ClockStr<decltype(value.source_receive_time)> source_receive_time(value.source_receive_time);
  stream::ClockStr<decltype(value.origin_create_time)> origin_create_time(value.origin_create_time);
  const char *FORMAT =
    "{"
    "source=%d, "
    "source_name=\"%s\", "
    "source_seqno=%" PRIu64 ", "
    "receive_time_utc=%s, "
    "receive_time=%s, "
    "source_send_time=%s, "
    "source_receive_time=%s, "
    "origin_create_time=%s, "
    "is_last=%s, "
    "opaque=%" PRIu64
    "}";
  return writer.printf(
      FORMAT,
      static_cast<int>(value.source),
      value.source_name,
      value.source_seqno,
      receive_time_utc.c_str(),
      receive_time.c_str(),
      source_send_time.c_str(),
      source_receive_time.c_str(),
      origin_create_time.c_str(),
      value.is_last ? "true" : "false",
      value.opaque);
}

// Subscribe

template <typename Writer>
Writer& write(Writer& writer, const Subscribe& value) {
  writer.printf("{accounts={");
  bool first = true;
  for (const auto& iter : value.accounts) {
    if (first) {
      first = false;
      writer.printf("\"%s\"", iter.c_str());
    } else {
      writer.printf(", \"%s\"", iter.c_str());
    }
  }
  writer.printf("}, symbols={");
  bool first_1 = true;
  for (const auto& iter_1 : value.symbols_by_exchange) {
    if (first_1) {
      first_1 = false;
      writer.printf("\"%s\": {", iter_1.first.c_str());
    } else {
      writer.printf(", \"%s\": {", iter_1.first.c_str());
    }
    bool first_2 = true;
    for (const auto& iter_2 : iter_1.second) {
      if (first_2) {
        first_2 = false;
        writer.printf("\"%s\"", iter_2.c_str());
      } else {
        writer.printf(", \"%s\"", iter_2.c_str());
      }
    }
    writer.printf("}");
  }
  return writer.printf("}}");
}

// DownloadBegin

template <typename Writer>
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

template <typename Writer>
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

template <typename Writer>
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

template <typename Writer>
Writer& write(Writer& writer, const OrderManagerStatus& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "status=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      EnumNameGatewayStatus(value.status));
}

template <typename Writer>
Writer& write(Writer& writer, const SessionStatistics& value) {
  stream::TimePointStr<decltype(value.exchange_time_utc)> exchange_time_utc(value.exchange_time_utc);
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
    "exchange_time_utc=%s"
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
      exchange_time_utc.c_str());
}

template <typename Writer>
Writer& write(Writer& writer, const DailyStatistics& value) {
  stream::TimePointStr<decltype(value.exchange_time_utc)> exchange_time_utc(value.exchange_time_utc);
  const char *FORMAT =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "open_price=%" FLOAT_REPR ", "
    "settlement_price=%" FLOAT_REPR ", "
    "close_price=%" FLOAT_REPR ", "
    "open_interest=%" FLOAT_REPR ", "
    "exchange_time_utc=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.exchange,
      value.symbol,
      value.open_price,
      value.settlement_price,
      value.close_price,
      value.open_interest,
      exchange_time_utc.c_str());
}

// MarketByPrice

template <typename Writer>
Writer& write(Writer& writer, const MarketByPrice& value) {
  stream::TimePointStr<decltype(value.exchange_time_utc)> exchange_time_utc(value.exchange_time_utc);
  const char *FORMAT_1 =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "bid=[";
  writer.printf(
      FORMAT_1,
      value.exchange,
      value.symbol);
  for (size_t i = 0; i < value.bid_length; ++i) {
    if (i)
      writer.printf(", ");
    write(writer, value.bid[i]);
  }
  const char *FORMAT_2 =
    "], "
    "ask=[";
  writer.printf(
      FORMAT_2);
  for (size_t i = 0; i < value.ask_length; ++i) {
    if (i)
      writer.printf(", ");
    write(writer, value.ask[i]);
  }
  const char *FORMAT_3 =
    "], "
    "snapshot=%s, "
    "exchange_time_utc=%s"
    "}";
  return writer.printf(
      FORMAT_3,
      value.snapshot ? "true" : "false",
      exchange_time_utc.c_str());
}

// TradeSummary

template <typename Writer>
Writer& write(Writer& writer, const TradeSummary& value) {
  stream::TimePointStr<decltype(value.exchange_time_utc)> exchange_time_utc(value.exchange_time_utc);
  const char *FORMAT =
    "{"
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "price=%" FLOAT_REPR ", "
    "volume=%" FLOAT_REPR ", "
    "turnover=%" FLOAT_REPR ", "
    "side=%s, "
    "exchange_time_utc=%s"
    "}";
  return writer.printf(
      FORMAT,
      value.exchange,
      value.symbol,
      value.price,
      value.volume,
      value.turnover,
      EnumNameSide(value.side),
      exchange_time_utc.c_str());
}

// ReferenceData

template <typename Writer>
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

template <typename Writer>
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

template <typename Writer>
Writer& write(Writer& writer, const PositionUpdate& value) {
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "side=%s, "
    "last_trade_id=%" PRIu32 ", "
    "position=%" FLOAT_REPR ", "
    "position_cost=%" FLOAT_REPR ", "
    "position_yesterday=%" FLOAT_REPR ", "
    "position_cost_yesterday=%" FLOAT_REPR
    "}";
  return writer.printf(
      FORMAT,
      value.account,
      value.exchange,
      value.symbol,
      EnumNameSide(value.side),
      value.last_trade_id,
      value.position,
      value.position_cost,
      value.position_yesterday,
      value.position_cost_yesterday);
}

// OrderUpdate

template <typename Writer>
Writer& write(Writer& writer, const OrderUpdate& value) {
  stream::TimePointStr<decltype(value.insert_time_utc)> insert_time_utc(value.insert_time_utc);
  stream::TimePointStr<decltype(value.cancel_time_utc)> cancel_time_utc(value.cancel_time_utc);
  const char *FORMAT =
    "{"
    "account=\"%s\", "
    "order_id=%" PRIu32 ", "
    "exchange=\"%s\", "
    "symbol=\"%s\", "
    "order_status=%s, "
    "side=%s, "
    "limit_price=%" FLOAT_REPR ", "
    "remaining_quantity=%" FLOAT_REPR ", "
    "traded_quantity=%" FLOAT_REPR ", "
    "position_effect=%s, "
    "order_template=\"%s\", "
    "insert_time_utc=%s, "
    "cancel_time_utc=%s, "
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
      value.limit_price,
      value.remaining_quantity,
      value.traded_quantity,
      EnumNamePositionEffect(value.position_effect),
      value.order_template,
      insert_time_utc.c_str(),
      cancel_time_utc.c_str(),
      value.order_local_id,
      value.order_external_id);
}

// TradeUpdate

template <typename Writer>
Writer& write(Writer& writer, const TradeUpdate& value) {
  stream::TimePointStr<decltype(value.trade_time_utc)> trade_time_utc(value.trade_time_utc);
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
    "trade_time_utc=%s, "
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
      trade_time_utc.c_str(),
      value.order_external_id,
      value.trade_external_id);
}

// CreteOrder

template <typename Writer>
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

template <typename Writer>
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

template <typename Writer>
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

template <typename Writer>
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

template <typename Writer>
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

template <typename Writer>
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

// StartEvent

template <typename Writer>
Writer& write(Writer& writer, const StartEvent&) {
  const char *FORMAT =
    "{"
    "}";
  return writer.printf(
      FORMAT);
}

// StopEvent

template <typename Writer>
Writer& write(Writer& writer, const StopEvent&) {
  const char *FORMAT =
    "{"
    "}";
  return writer.printf(
      FORMAT);
}

// TimerEvent

template <typename Writer>
Writer& write(Writer& writer, const TimerEvent& value) {
  stream::ClockStr<decltype(value.now)> now(value.now);
  const char *FORMAT =
    "{"
    "now=%s"
    "}";
  return writer.printf(
      FORMAT,
      now.c_str());
}

// ConnectionStatusEvent

template <typename Writer>
Writer& write(Writer& writer, const ConnectionStatusEvent& value) {
  const char *FORMAT =
    "{"
    "source=%d, "
    "source_name=\"%s\", "
    "connection_status=%s"
    "}";
  return writer.printf(
      FORMAT,
      static_cast<int>(value.source),
      value.source_name,
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
Writer& write(Writer& writer, const SubscribeEvent& value) {
  return writer
    .printf("{message_info=")
    .print(value.message_info)
    .printf(", subscribe=")
    .print(value.subscribe)
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

template <typename Writer>
Writer& write(Writer& writer, const CustomMessageEvent&) {
  return writer
    .printf("{}");
}

template <typename Writer>
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

std::ostream& operator<<(std::ostream& stream, const UpdateAction value) {
  return stream << EnumNameUpdateAction(value);
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

// MBPUpdate

std::ostream& operator<<(std::ostream& stream, const MBPUpdate& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// MessageInfo

std::ostream& operator<<(std::ostream& stream, const MessageInfo& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// Subscribe

std::ostream& operator<<(std::ostream& stream, const Subscribe& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DownloadBegin

std::ostream& operator<<(std::ostream& stream, const DownloadBegin& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// DownloadEnd

std::ostream& operator<<(std::ostream& stream, const DownloadEnd& value) {
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

// StartEvent

std::ostream& operator<<(std::ostream& stream, const StartEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// StopEvent

std::ostream& operator<<(std::ostream& stream, const StopEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// TimerEvent

std::ostream& operator<<(std::ostream& stream, const TimerEvent& value) {
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

// SubscribeEvent

std::ostream& operator<<(std::ostream& stream, const SubscribeEvent& value) {
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

// CustomMessageEvent

std::ostream& operator<<(std::ostream& stream, const CustomMessageEvent& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// Layer

std::ostream& operator<<(std::ostream& stream, const Layer& value) {
  Writer writer;
  return writer.print(value).finish(stream);
}

// misc:

std::ostream& operator<<(
    std::ostream& stream,
    const roq::User::Limits::CreateOrder& value) {
  return stream << "{"
    "max=" << value.max << ", "
    "monitor_period=" << value.monitor_period.count() << "s, "
    "ban_period=" << value.ban_period.count() << "s"
    "}";
}
std::ostream& operator<<(
    std::ostream& stream,
    const roq::User::Limits& value) {
  return stream << "{"
    "create_order=" << value.create_order <<
    "}";
}

// Account

std::ostream& operator<<(std::ostream& stream, const Account& value) {
  return stream << "{"
    "id=" << static_cast<int>(value.id) << ", "
    "name=\"" << value.name << "\", "
    "user=\"" << value.user << "\", "
    "password=" << PASSWORD << ", "
    "symbols=" << join(value.symbols) <<
    "}";
}

// User

std::ostream& operator<<(std::ostream& stream, const User& value) {
  return stream << "{"
    "id=" << static_cast<int>(value.id) << ", "
    "name=\"" << value.name << "\", "
    "password=" << PASSWORD << ", "
    "accounts=" << join(value.accounts) << ", "
    "symbols=" << join(value.symbols) << ", "
    "limits=" << value.limits <<
    "}";
}

}  // namespace roq
