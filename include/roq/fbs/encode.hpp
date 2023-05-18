/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string_view>
#include <type_traits>
#include <vector>

#include "roq/api.hpp"

#include "roq/fbs/api.hpp"

#include "roq/utils/traits.hpp"

namespace roq {
namespace fbs {

// mappings

namespace detail {
template <typename T> struct maps_to;
// enums
template<> struct maps_to<roq::BufferCapacity> final { using type = fbs::BufferCapacity; };
template<> struct maps_to<roq::ConnectionStatus> final { using type = fbs::ConnectionStatus; };
template<> struct maps_to<roq::Decimals> final { using type = fbs::Decimals; };
template<> struct maps_to<roq::Encoding> final { using type = fbs::Encoding; };
template<> struct maps_to<roq::Error> final { using type = fbs::Error; };
template<> struct maps_to<roq::Liquidity> final { using type = fbs::Liquidity; };
template<> struct maps_to<roq::ExecutionInstruction> final { using type = fbs::ExecutionInstruction; };
template<> struct maps_to<roq::OptionType> final { using type = fbs::OptionType; };
template<> struct maps_to<roq::OrderStatus> final { using type = fbs::OrderStatus; };
template<> struct maps_to<roq::OrderType> final { using type = fbs::OrderType; };
template<> struct maps_to<roq::Origin> final { using type = fbs::Origin; };
template<> struct maps_to<roq::PositionEffect> final { using type = fbs::PositionEffect; };
template<> struct maps_to<roq::Priority> final { using type = fbs::Priority; };
template<> struct maps_to<roq::Protocol> final { using type = fbs::Protocol; };
template<> struct maps_to<roq::RateLimitType> final { using type = fbs::RateLimitType; };
template<> struct maps_to<roq::RequestIdType> final { using type = fbs::RequestIdType; };
template<> struct maps_to<roq::RequestStatus> final { using type = fbs::RequestStatus; };
template<> struct maps_to<roq::RequestType> final { using type = fbs::RequestType; };
template<> struct maps_to<roq::SecurityType> final { using type = fbs::SecurityType; };
template<> struct maps_to<roq::Side> final { using type = fbs::Side; };
template<> struct maps_to<roq::StatisticsType> final { using type = fbs::StatisticsType; };
template<> struct maps_to<roq::SupportType> final { using type = fbs::SupportType; };
template<> struct maps_to<roq::TimeInForce> final { using type = fbs::TimeInForce; };
template<> struct maps_to<roq::TradingStatus> final { using type = fbs::TradingStatus; };
template<> struct maps_to<roq::Transport> final { using type = fbs::Transport; };
template<> struct maps_to<roq::UpdateAction> final { using type = fbs::UpdateAction; };
template<> struct maps_to<roq::UpdateReason> final { using type = fbs::UpdateReason; };
template<> struct maps_to<roq::UpdateType> final { using type = fbs::UpdateType; };
// helper
template<> struct maps_to<roq::MBPUpdate> final { using type = fbs::MBPUpdate; };
template<> struct maps_to<roq::MBOUpdate> final { using type = fbs::MBOUpdate; };
template<> struct maps_to<roq::Trade> final { using type = fbs::Trade; };
template<> struct maps_to<roq::Fill> final { using type = fbs::Fill; };
template<> struct maps_to<roq::Statistics> final { using type = fbs::Statistics; };
template<> struct maps_to<roq::Measurement> final { using type = fbs::Measurement; };
template<> struct maps_to<roq::Parameter> final { using type = fbs::Parameter; };
template<> struct maps_to<roq::Position> final { using type = fbs::Position; };
// structs
template <> struct maps_to<roq::DownloadBegin> { static constexpr auto value = Message::DownloadBegin; };
template <> struct maps_to<roq::DownloadEnd> { static constexpr auto value = Message::DownloadEnd; };
template <> struct maps_to<roq::GatewaySettings> { static constexpr auto value = Message::GatewaySettings; };
template <> struct maps_to<roq::StreamStatus> { static constexpr auto value = Message::StreamStatus; };
template <> struct maps_to<roq::ExternalLatency> { static constexpr auto value = Message::ExternalLatency; };
template <> struct maps_to<roq::RateLimitTrigger> { static constexpr auto value = Message::RateLimitTrigger; };
template <> struct maps_to<roq::GatewayStatus> { static constexpr auto value = Message::GatewayStatus; };
template <> struct maps_to<roq::ReferenceData> { static constexpr auto value = Message::ReferenceData; };
template <> struct maps_to<roq::MarketStatus> { static constexpr auto value = Message::MarketStatus; };
template <> struct maps_to<roq::TopOfBook> { static constexpr auto value = Message::TopOfBook; };
template <> struct maps_to<roq::MarketByPriceUpdate> { static constexpr auto value = Message::MarketByPriceUpdate; };
template <> struct maps_to<roq::MarketByOrderUpdate> { static constexpr auto value = Message::MarketByOrderUpdate; };
template <> struct maps_to<roq::TradeSummary> { static constexpr auto value = Message::TradeSummary; };
template <> struct maps_to<roq::StatisticsUpdate> { static constexpr auto value = Message::StatisticsUpdate; };
template <> struct maps_to<roq::CreateOrder> { static constexpr auto value = Message::CreateOrder; };
template <> struct maps_to<roq::ModifyOrder> { static constexpr auto value = Message::ModifyOrder; };
template <> struct maps_to<roq::CancelOrder> { static constexpr auto value = Message::CancelOrder; };
template <> struct maps_to<roq::CancelAllOrders> { static constexpr auto value = Message::CancelAllOrders; };
template <> struct maps_to<roq::OrderAck> { static constexpr auto value = Message::OrderAck; };
template <> struct maps_to<roq::OrderUpdate> { static constexpr auto value = Message::OrderUpdate; };
template <> struct maps_to<roq::TradeUpdate> { static constexpr auto value = Message::TradeUpdate; };
template <> struct maps_to<roq::PositionUpdate> { static constexpr auto value = Message::PositionUpdate; };
template <> struct maps_to<roq::FundsUpdate> { static constexpr auto value = Message::FundsUpdate; };
template <> struct maps_to<roq::CustomMetrics> { static constexpr auto value = Message::CustomMetrics; };
template <> struct maps_to<roq::CustomMetricsUpdate> { static constexpr auto value = Message::CustomMetricsUpdate; };
template <> struct maps_to<roq::CustomMatrix> { static constexpr auto value = Message::CustomMatrix; };
template <> struct maps_to<roq::CustomMatrixUpdate> { static constexpr auto value = Message::CustomMatrixUpdate; };
template <> struct maps_to<roq::ParametersUpdate> { static constexpr auto value = Message::ParametersUpdate; };
template <> struct maps_to<roq::PortfolioUpdate> { static constexpr auto value = Message::PortfolioUpdate; };
}

// std::chrono::duration

template <typename B, typename T, typename std::enable_if<utils::is_duration<T>::value>::type* = nullptr>
auto encode([[maybe_unused]]B &, T value) {
  return value.count();
}

// std::string_view

template <typename B>
auto encode(B &builder, std::string_view const &value) {
  return builder.CreateString(value);
}

// String<N>

template <typename B, typename T, std::size_t N, typename std::enable_if<std::is_base_of<roq::String<N>, T>::value>::type* = nullptr>
auto encode(B &builder, T const &value) {
  return encode(builder, static_cast<std::string_view>(value));
}

// enums

template <typename B, typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
auto encode([[maybe_unused]]B &, T value) {
  using result_type = typename detail::maps_to<T>::type;
  using value_type = std::underlying_type_t<T>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

// helper

template <typename B>
auto encode(B &builder, roq::Fill const &value) {
  return CreateFill(
      builder,
      encode(builder, static_cast<std::string_view>(value.external_trade_id)),
      value.quantity,
      value.price,
      encode(builder, value.liquidity));
}

template <typename B>
auto encode(B &builder, roq::Layer const &value) {
  return CreateLayer(builder, value.bid_price, value.bid_quantity, value.ask_price, value.ask_quantity);
}

template <typename B>
auto encode(B &builder, roq::MBPUpdate const &value) {
  return CreateMBPUpdate(
      builder, value.price, value.quantity, value.implied_quantity, value.price_level, value.number_of_orders, encode(builder, value.update_action));
}

template <typename B>
auto encode(B &builder, roq::MBOUpdate const &value) {
  return CreateMBOUpdate(
      builder,
      value.price,
      value.quantity,
      value.priority,
      encode(builder, static_cast<std::string_view>(value.order_id)),
      encode(builder,value.side),
      encode(builder, value.action),
      encode(builder, value.reason));
}

template <typename B>
auto encode(B &builder, roq::Parameter const &value) {
  return CreateParameter(
      builder,
      encode(builder, static_cast<std::string_view>(value.label)),
      encode(builder, static_cast<std::string_view>(value.account)),
      encode(builder, static_cast<std::string_view>(value.exchange)),
      encode(builder, static_cast<std::string_view>(value.symbol)),
      encode(builder, static_cast<std::string_view>(value.value)));
}

template <typename B>
auto encode(B &builder, roq::Position const &value) {
  return CreatePosition(
      builder,
      encode(builder, static_cast<std::string_view>(value.account)),
      encode(builder, static_cast<std::string_view>(value.exchange)),
      encode(builder, static_cast<std::string_view>(value.symbol)),
      value.long_quantity,
      value.short_quantity);
}

template <typename B>
auto encode(B &builder, roq::Statistics const &value) {
  return CreateStatistics(
      builder,
      encode(builder, value.type),
      value.value,
      encode(builder, value.begin_time_utc),
      encode(builder, value.end_time_utc));
}

template <typename B>
auto encode(B &builder, roq::Measurement const &value) {
  return CreateMeasurement(builder, encode(builder, static_cast<std::string_view>(value.name)), value.value);
}

template <typename B>
auto encode(B &builder, roq::Trade const &value) {
  return CreateTrade(
      builder,
      encode(builder, value.side),
      value.price,
      value.quantity,
      encode(builder, static_cast<std::string_view>(value.trade_id)),
      encode(builder, static_cast<std::string_view>(value.taker_order_id)),
      encode(builder, static_cast<std::string_view>(value.maker_order_id)));
}

// std::span

namespace detail {
template <typename U,  typename B, typename T>
auto encode_double_vector(B &builder, std::span<T const> const& value) {
  using result_type = U;
  std::vector<result_type> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value)
      result.emplace_back( item);
  }
  return builder.CreateVector(result);
}
}

namespace detail {
template <typename U,  typename B, typename T>
auto encode_string_vector(B &builder, std::span<T const> const& value) {
  using result_type = U;
  std::vector<flatbuffers::Offset<result_type>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value)
      result.emplace_back(encode(builder, item));
  }
  return builder.CreateVector(result);
}
}

template <typename B, typename T>
auto encode(B &builder, std::span<T const> const& value) {
  constexpr bool is_string_like = requires(T const& t) {
    t.operator std::string_view();
  };
  if constexpr (std::is_floating_point<T>::value) {
    using result_type = double;
    return detail::encode_double_vector<result_type>(builder, value);
  } else if constexpr (is_string_like) {
    using result_type = flatbuffers::String;
    return detail::encode_string_vector<result_type>(builder, value);
  } else {
    using result_type = typename detail::maps_to<typename std::decay<T>::type>::type;
    return detail::encode_string_vector<result_type>(builder, value);
  }
}

// structs

template <typename B>
auto encode(B &builder, roq::MessageInfo const &value) {
  return CreateSourceInfo(
      builder,
      value.source_seqno,
      encode(builder, value.receive_time_utc),
      encode(builder, value.origin_create_time_utc),
      encode(builder, value.receive_time),
      encode(builder, value.origin_create_time));
}

template <typename B>
auto encode(B &builder, roq::DownloadBegin const &value) {
  return CreateDownloadBegin(builder, encode(builder, value.account));
}

template <typename B>
auto encode(B &builder, roq::DownloadEnd const &value) {
  return CreateDownloadEnd(builder, encode(builder, value.account), value.max_order_id);
}

template <typename B>
auto encode(B &builder, roq::GatewaySettings const &value) {
  return CreateGatewaySettings(
      builder,
      value.supports.get(),
      value.mbp_max_depth,
      value.mbp_allow_price_inversion,
      value.mbp_allow_remove_non_existing,
      value.oms_download_has_state,
      value.oms_download_has_routing_id,
      value.mbp_tick_size_multiplier,
      value.mbp_min_trade_vol_multiplier,
      encode(builder, value.oms_request_id_type),
      value.mbp_checksum);
}

template <typename B>
auto encode(B &builder, roq::StreamStatus const &value) {
  return CreateStreamStatus(
      builder,
      value.stream_id,
      encode(builder, value.account),
      value.supports.get(),
      encode(builder, value.connection_status),
      encode(builder, value.protocol),
      encode(builder, value.priority),
      encode(builder, value.transport),
      value.encoding.get(),
      encode(builder, value.interface),
      encode(builder, value.authority),
      encode(builder, value.path),
      encode(builder, value.proxy));
}

template <typename B>
auto encode(B &builder, roq::ExternalLatency const &value) {
  return CreateExternalLatency(
      builder, value.stream_id, encode(builder, value.latency), encode(builder, value.account));
}

template <typename B>
auto encode(B &builder, roq::RateLimitTrigger const &value) {
  return CreateRateLimitTrigger(
      builder,
      encode(builder, value.name),
      encode(builder, value.origin),
      encode(builder, value.type),
      encode(builder, value.users),
      encode(builder, value.accounts),
      encode(builder, value.ban_expires),
      encode(builder, value.triggered_by),
      encode(builder, value.buffer_capacity),
      value.remaining_requests);
}

template <typename B>
auto encode(B &builder, roq::GatewayStatus const &value) {
  return CreateGatewayStatus(
      builder, encode(builder, value.account), value.supported.get(), value.available.get(), value.unavailable.get());
}

template <typename B>
auto encode(B &builder, roq::ReferenceData const &value) {
  return CreateReferenceData(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.description),
      encode(builder, value.security_type),
      encode(builder, value.quote_currency),
      encode(builder, value.base_currency),
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
      encode(builder, value.expiry_datetime_utc),
      value.max_trade_vol,
      value.trade_vol_step_size,
      encode(builder, value.margin_currency),
      value.discard,
      value.min_notional);
}

template <typename B>
auto encode(B &builder, roq::MarketStatus const &value) {
  return CreateMarketStatus(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.trading_status));
}

template <typename B>
auto encode(B &builder, roq::TopOfBook const &value) {
  return CreateTopOfBook(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.layer),
      encode(builder, value.update_type),
      encode(builder, value.exchange_time_utc),
      value.exchange_sequence,
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::MarketByPriceUpdate const &value) {
  return CreateMarketByPriceUpdate(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.bids),
      encode(builder, value.asks),
      encode(builder, value.update_type),
      encode(builder, value.exchange_time_utc),
      value.exchange_sequence,
      encode(builder, value.price_decimals),
      encode(builder, value.quantity_decimals),
      value.max_depth,
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::MarketByOrderUpdate const &value) {
  return CreateMarketByOrderUpdate(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.orders),
      encode(builder, value.update_type),
      encode(builder, value.exchange_time_utc),
      value.exchange_sequence,
      encode(builder, value.price_decimals),
      encode(builder, value.quantity_decimals),
      value.max_depth,
      value.checksum,
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::TradeSummary const &value) {
  return CreateTradeSummary(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.trades),
      encode(builder, value.exchange_time_utc),
      value.exchange_sequence,
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::StatisticsUpdate const &value) {
  return CreateStatisticsUpdate(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.statistics),
      encode(builder, value.update_type),
      encode(builder, value.exchange_time_utc),
      value.exchange_sequence,
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::CreateOrder const &value) {
  return CreateCreateOrder(
      builder,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.side),
      encode(builder, value.position_effect),
      value.max_show_quantity,
      encode(builder, value.order_type),
      encode(builder, value.time_in_force),
      {},  // deprecated
      encode(builder, value.request_template),
      value.quantity,
      value.price,
      value.stop_price,
      encode(builder, value.routing_id),
      value.execution_instructions.get());
}

template <typename B>
auto encode(B &builder, roq::ModifyOrder const &value) {
  return CreateModifyOrder(
      builder,
      encode(builder, value.account),
      value.order_id,
      value.quantity,
      value.price,
      encode(builder, value.routing_id),
      value.version,
      value.conditional_on_version,
      encode(builder, value.request_template));
}

template <typename B>
auto encode(B &builder, roq::CancelOrder const &value) {
  return CreateCancelOrder(
      builder,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.routing_id),
      value.version,
      value.conditional_on_version,
      encode(builder, value.request_template));
}

template <typename B>
auto encode(B &builder, roq::CancelAllOrders const &value) {
  return CreateCancelAllOrders(builder, encode(builder, value.account));
}

template <typename B>
auto encode(B &builder, roq::OrderAck const &value) {
  return CreateOrderAck(
      builder,
      value.stream_id,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.type),
      encode(builder, value.origin),
      encode(builder, value.status),
      encode(builder, value.error),
      encode(builder, value.text),
      encode(builder, value.request_id),
      encode(builder, value.external_account),
      encode(builder, value.external_order_id),
      encode(builder, value.routing_id),
      value.version,
      encode(builder, value.side),
      encode(builder, value.round_trip_latency),
      value.traded_quantity);
}

template <typename B>
auto encode(B &builder, roq::OrderUpdate const &value) {
  return CreateOrderUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.side),
      encode(builder, value.position_effect),
      value.max_show_quantity,
      encode(builder, value.order_type),
      encode(builder, value.time_in_force),
      {},  // deprecated
      {},  // deprecated
      encode(builder, value.create_time_utc),
      encode(builder, value.update_time_utc),
      encode(builder, value.external_account),
      encode(builder, value.external_order_id),
      encode(builder, value.status),
      value.quantity,
      value.price,
      value.stop_price,
      value.remaining_quantity,
      value.traded_quantity,
      value.average_traded_price,
      value.last_traded_quantity,
      value.last_traded_price,
      encode(builder, value.last_liquidity),
      encode(builder, value.routing_id),
      value.max_request_version,
      value.max_response_version,
      value.max_accepted_version,
      encode(builder, value.update_type),
      value.execution_instructions.get(),
      encode(builder, value.user),
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::TradeUpdate const &value) {
  return CreateTradeUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.side),
      encode(builder, value.position_effect),
      encode(builder, value.create_time_utc),
      encode(builder, value.update_time_utc),
      encode(builder, value.external_account),
      encode(builder, value.external_order_id),
      encode(builder, value.fills),
      encode(builder, value.routing_id),
      encode(builder, value.update_type),
      encode(builder, value.user),
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::PositionUpdate const &value) {
  return CreatePositionUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.external_account),
      value.long_quantity,
      value.short_quantity,
      encode(builder, value.update_type),
      encode(builder, value.exchange_time_utc),
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::FundsUpdate const &value) {
  return CreateFundsUpdate(
      builder,
      value.stream_id,
      encode(builder, value.account),
      encode(builder, value.currency),
      value.balance,
      value.hold,
      encode(builder, value.external_account),
      encode(builder, value.update_type),
      encode(builder, value.exchange_time_utc),
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::CustomMetrics const &value) {
  return CreateCustomMetrics(
      builder,
      encode(builder, value.label),
      encode(builder, value.account),
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.measurements),
      encode(builder, value.update_type));
}

template <typename B>
auto encode(B &builder, roq::CustomMetricsUpdate const &value) {
  return CreateCustomMetricsUpdate(
      builder,
      encode(builder, value.user),
      encode(builder, value.label),
      encode(builder, value.account),
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.measurements),
      encode(builder, value.update_type),
      encode(builder, value.sending_time_utc));
}

template <typename B>
auto encode(B &builder, roq::CustomMatrix const &value) {
  return CreateCustomMatrix(
      builder,
      encode(builder, value.label),
      encode(builder, value.account),
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.rows),
      encode(builder, value.columns),
      encode(builder, value.data),
      encode(builder, value.update_type),
      value.version);
}

template <typename B>
auto encode(B &builder, roq::CustomMatrixUpdate const &value) {
  return CreateCustomMatrixUpdate(
      builder,
      encode(builder, value.label),
      encode(builder, value.account),
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.rows),
      encode(builder, value.columns),
      encode(builder, value.data),
      encode(builder, value.update_type),
      value.version,
      encode(builder, value.sending_time_utc),
      encode(builder, value.user));
}

template <typename B>
auto encode(B &builder, roq::ParametersUpdate const &value) {
  return CreateParametersUpdate(
      builder,
      encode(builder, value.parameters),
      encode(builder, value.update_type),
      encode(builder, value.user));
}

template <typename B>
auto encode(B &builder, roq::PortfolioUpdate const &value) {
  return CreatePortfolioUpdate(
      builder,
      encode(builder, value.positions),
      encode(builder, value.update_type),
      encode(builder, value.exchange_time_utc),
      encode(builder, value.user));
}

// events

template <typename B, typename T>
flatbuffers::Offset<Event> encode(B &builder, roq::Event<T> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), detail::maps_to<T>::value, encode(builder, event.value).Union());
}

}  // namespace fbs
}  // namespace roq
