/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string_view>
#include <type_traits>
#include <vector>

#include "roq/api.h"
#include "roq/fbs/api.h"

namespace roq {
namespace fbs {

template <typename B, typename T>
auto encode(B &b, const T &);

// std::string_view

template <typename B>
auto encode(B &builder, const std::string_view &value) {
  return builder.CreateString(value);
}

// std::chrono::duration

template <typename B>
auto encode(B &, const std::chrono::nanoseconds &value) {
  return value.count();
}

template <typename B>
auto encode(B &, const std::chrono::milliseconds &value) {
  return value.count();
}

template <typename B>
auto encode(B &, const std::chrono::seconds &value) {
  return value.count();
}

template <typename B>
auto encode(B &, const roq::chrono::days &value) {
  return value.count();
}

// enums

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::Error &value) {
  using result_type = Error;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::ExecutionInstruction &value) {
  using result_type = ExecutionInstruction;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::GatewayStatus &value) {
  using result_type = GatewayStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::OptionType &value) {
  using result_type = OptionType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::OrderStatus &value) {
  using result_type = OrderStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::OrderType &value) {
  using result_type = OrderType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::OrderUpdateAction &value) {
  using result_type = OrderUpdateAction;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::Origin &value) {
  using result_type = Origin;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::PositionEffect &value) {
  using result_type = PositionEffect;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::Priority &value) {
  using result_type = Priority;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::RequestStatus &value) {
  using result_type = RequestStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::RequestType &value) {
  using result_type = RequestType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}
template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::SecurityType &value) {
  using result_type = SecurityType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::Side &value) {
  using result_type = Side;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::StreamType &value) {
  using result_type = StreamType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

// note! not used directly... redundant, really
template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::SupportType &value) {
  using result_type = SupportType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::TimeInForce &value) {
  using result_type = TimeInForce;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::TradingStatus &value) {
  using result_type = TradingStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, const roq::StatisticsType &value) {
  using result_type = StatisticsType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

// helper

template <typename B>
auto encode(B &builder, const roq::Layer &value) {
  return CreateLayer(
      builder, value.bid_price, value.bid_quantity, value.ask_price, value.ask_quantity);
}

template <typename B>
auto encode(B &builder, const roq::MBPUpdate &value) {
  return CreateMBPUpdate(builder, value.price, value.quantity);
}

template <typename B>
auto encode(B &builder, const roq::MBOUpdate &value) {
  return CreateMBOUpdate(
      builder,
      value.price,
      value.remaining_quantity,
      encode(builder, value.action),
      value.priority,
      encode(builder, static_cast<std::string_view>(value.order_id)));
}

template <typename B>
auto encode(B &builder, const roq::Trade &value) {
  return CreateTrade(
      builder,
      encode(builder, value.side),
      value.price,
      value.quantity,
      encode(builder, static_cast<std::string_view>(value.trade_id)));
}

template <typename B>
auto encode(B &builder, const roq::Fill &value) {
  return CreateFill(
      builder,
      value.quantity,
      value.price,
      value.trade_id,
      value.gateway_trade_id,
      encode(builder, static_cast<std::string_view>(value.external_trade_id)));
}

template <typename B>
auto encode(B &builder, const roq::Statistics &value) {
  return CreateStatistics(builder, encode(builder, value.type), value.value);
}

// roq::span

template <typename B>
auto encode(B &builder, const roq::span<roq::MBPUpdate> &value) {
  std::vector<flatbuffers::Offset<MBPUpdate>> result;
  auto size = value.size();
  if (size) {
    result.reserve(size);
    for (const auto &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, const roq::span<roq::MBOUpdate> &value) {
  std::vector<flatbuffers::Offset<MBOUpdate>> result;
  auto size = value.size();
  if (size) {
    result.reserve(size);
    for (const auto &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, const roq::span<roq::Trade> &value) {
  std::vector<flatbuffers::Offset<Trade>> result;
  auto size = value.size();
  if (size) {
    result.reserve(size);
    for (const auto &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, const roq::span<roq::Fill> &value) {
  std::vector<flatbuffers::Offset<Fill>> result;
  auto size = value.size();
  if (size) {
    result.reserve(size);
    for (const auto &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, const roq::span<roq::Statistics> &value) {
  std::vector<flatbuffers::Offset<Statistics>> result;
  auto size = value.size();
  if (size) {
    result.reserve(size);
    for (const auto &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

// structs

template <typename B>
auto encode(B &builder, const roq::MessageInfo &value) {
  return CreateSourceInfo(
      builder,
      value.source_seqno,
      encode(builder, value.receive_time_utc),
      encode(builder, value.origin_create_time_utc),
      encode(builder, value.receive_time),
      encode(builder, value.origin_create_time));
}

template <typename B>
auto encode(B &builder, const roq::DownloadBegin &value) {
  return CreateDownloadBegin(builder, encode(builder, value.account));
}

template <typename B>
auto encode(B &builder, const roq::DownloadEnd &value) {
  return CreateDownloadEnd(builder, encode(builder, value.account), value.max_order_id);
}

template <typename B>
auto encode(B &builder, const roq::GatewaySettings &value) {
  return CreateGatewaySettings(builder, value.mbp_max_depth, value.mbp_allow_price_inversion);
}

template <typename B>
auto encode(B &builder, const roq::StreamUpdate &value) {
  return CreateStreamUpdate(
      builder,
      value.stream_id,
      encode(builder, value.type),
      value.supports,  // note! using Mask<SupportType>
      encode(builder, value.account),
      encode(builder, value.priority),
      encode(builder, value.status));
}

template <typename B>
auto encode(B &builder, const roq::ExternalLatency &value) {
  return CreateExternalLatency(builder, value.stream_id, encode(builder, value.latency));
}

template <typename B>
auto encode(B &builder, const roq::ReferenceData &value) {
  return CreateReferenceData(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.description),
      encode(builder, value.security_type),
      encode(builder, value.currency),
      encode(builder, value.settlement_currency),
      encode(builder, value.commission_currency),
      value.tick_size,
      value.multiplier,
      value.min_trade_vol,
      encode(builder, value.option_type),
      encode(builder, value.strike_currency),
      value.strike_price,
      encode(builder, value.underlying),
      encode(builder, value.time_zone),
      encode(builder, value.issue_date),
      encode(builder, value.settlement_date),
      encode(builder, value.expiry_datetime),
      encode(builder, value.expiry_datetime_utc));
}

template <typename B>
auto encode(B &builder, const roq::MarketStatus &value) {
  return CreateMarketStatus(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.trading_status));
}

template <typename B>
auto encode(B &builder, const roq::TopOfBook &value) {
  return CreateTopOfBook(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.layer),
      value.snapshot,
      encode(builder, value.exchange_time_utc));
}

template <typename B>
auto encode(B &builder, const roq::MarketByPriceUpdate &value) {
  return CreateMarketByPriceUpdate(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.bids),
      encode(builder, value.asks),
      value.snapshot,
      encode(builder, value.exchange_time_utc));
}

template <typename B>
auto encode(B &builder, const roq::MarketByOrderUpdate &value) {
  return CreateMarketByOrderUpdate(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.bids),
      encode(builder, value.asks),
      value.snapshot,
      encode(builder, value.exchange_time_utc));
}

template <typename B>
auto encode(B &builder, const roq::TradeSummary &value) {
  return CreateTradeSummary(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.trades),
      encode(builder, value.exchange_time_utc));
}

template <typename B>
auto encode(B &builder, const roq::StatisticsUpdate &value) {
  return CreateStatisticsUpdate(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.statistics),
      value.snapshot,
      encode(builder, value.exchange_time_utc));
}

template <typename B>
auto encode(B &builder, const roq::CreateOrder &value) {
  return CreateCreateOrder(
      builder,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.side),
      value.quantity,
      encode(builder, value.order_type),
      value.price,
      encode(builder, value.time_in_force),
      encode(builder, value.position_effect),
      encode(builder, value.execution_instruction),
      value.stop_price,
      value.max_show_quantity,
      encode(builder, value.order_template));
}

template <typename B>
auto encode(B &builder, const roq::ModifyOrder &value) {
  return CreateModifyOrder(
      builder, encode(builder, value.account), value.order_id, value.quantity, value.price);
}

template <typename B>
auto encode(B &builder, const roq::CancelOrder &value) {
  return CreateCancelOrder(builder, encode(builder, value.account), value.order_id);
}

template <typename B>
auto encode(B &builder, const roq::OrderAck &value) {
  return CreateOrderAck(
      builder,
      value.stream_id,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.type),
      encode(builder, value.origin),
      encode(builder, value.status),
      encode(builder, value.error),
      encode(builder, value.text),
      value.gateway_order_id,
      encode(builder, value.external_account),
      encode(builder, value.external_order_id),
      encode(builder, value.request_id));
}

template <typename B>
auto encode(B &builder, const roq::OrderUpdate &value) {
  return CreateOrderUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.status),
      encode(builder, value.side),
      value.price,
      value.remaining_quantity,
      value.traded_quantity,
      encode(builder, value.position_effect),
      encode(builder, value.order_template),
      encode(builder, value.create_time_utc),
      encode(builder, value.update_time_utc),
      value.gateway_order_id,
      encode(builder, value.external_account),
      encode(builder, value.external_order_id));
}

template <typename B>
auto encode(B &builder, const roq::TradeUpdate &value) {
  return CreateTradeUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.side),
      encode(builder, value.position_effect),
      encode(builder, value.order_template),
      encode(builder, value.create_time_utc),
      encode(builder, value.update_time_utc),
      value.gateway_order_id,
      encode(builder, value.external_account),
      encode(builder, value.external_order_id),
      encode(builder, value.fills));
}

template <typename B>
auto encode(B &builder, const roq::PositionUpdate &value) {
  return CreatePositionUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.side),
      value.position,
      value.last_trade_id,
      value.position_cost,
      value.position_yesterday,
      value.position_cost_yesterday,
      encode(builder, value.external_account));
}

template <typename B>
auto encode(B &builder, const roq::FundsUpdate &value) {
  return CreateFundsUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      encode(builder, value.currency),
      value.balance,
      value.hold,
      encode(builder, value.external_account));
}

// events

template <typename B>
flatbuffers::Offset<Event> encode(B &builder, const roq::Event<roq::DownloadBegin> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_DownloadBegin,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::DownloadEnd> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_DownloadEnd,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::GatewaySettings> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_GatewaySettings,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::StreamUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_StreamUpdate,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::ExternalLatency> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_ExternalLatency,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::ReferenceData> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_ReferenceData,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::MarketStatus> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_MarketStatus,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::TopOfBook> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_TopOfBook,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::MarketByPriceUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_MarketByPriceUpdate,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::MarketByOrderUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_MarketByOrderUpdate,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::TradeSummary> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_TradeSummary,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::StatisticsUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_StatisticsUpdate,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::CreateOrder> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_CreateOrder,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::ModifyOrder> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_ModifyOrder,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::CancelOrder> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_CancelOrder,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::OrderAck> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_OrderAck,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::OrderUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_OrderUpdate,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::TradeUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_TradeUpdate,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::PositionUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_PositionUpdate,
      encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, const roq::Event<roq::FundsUpdate> &event) {
  return CreateEvent(
      builder,
      encode(builder, event.message_info),
      Message_FundsUpdate,
      encode(builder, event.value).Union());
}

}  // namespace fbs
}  // namespace roq
