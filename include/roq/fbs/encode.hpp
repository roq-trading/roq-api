/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string_view>
#include <type_traits>
#include <vector>

#include "roq/api.hpp"
#include "roq/fbs/api.hpp"

namespace roq {
namespace fbs {

template <typename B, typename T>
auto encode(B &b, const T &);

// std::string_view

template <typename B>
auto encode(B &builder, std::string_view const &value) {
  return builder.CreateString(value);
}

// std::chrono::duration

template <typename B>
auto encode(B &, std::chrono::nanoseconds const &value) {
  return value.count();
}

template <typename B>
auto encode(B &, std::chrono::milliseconds const &value) {
  return value.count();
}

template <typename B>
auto encode(B &, std::chrono::seconds const &value) {
  return value.count();
}

template <typename B>
auto encode(B &, std::chrono::days const &value) {
  return value.count();
}

// enums

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::BufferCapacity const &value) {
  using result_type = BufferCapacity;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::ConnectionStatus const &value) {
  using result_type = ConnectionStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Decimals const &value) {
  using result_type = Decimals;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Encoding const &value) {
  using result_type = Encoding;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Error const &value) {
  using result_type = Error;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Liquidity const &value) {
  using result_type = Liquidity;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::ExecutionInstruction const &value) {
  using result_type = ExecutionInstruction;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::OptionType const &value) {
  using result_type = OptionType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::OrderStatus const &value) {
  using result_type = OrderStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::OrderType const &value) {
  using result_type = OrderType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::OrderUpdateAction const &value) {
  using result_type = OrderUpdateAction;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Origin const &value) {
  using result_type = Origin;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::PositionEffect const &value) {
  using result_type = PositionEffect;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Priority const &value) {
  using result_type = Priority;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Protocol const &value) {
  using result_type = Protocol;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::RateLimitType const &value) {
  using result_type = RateLimitType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::RequestIdType const &value) {
  using result_type = RequestIdType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::RequestStatus const &value) {
  using result_type = RequestStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::RequestType const &value) {
  using result_type = RequestType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::SecurityType const &value) {
  using result_type = SecurityType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Side const &value) {
  using result_type = Side;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::StatisticsType const &value) {
  using result_type = StatisticsType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

// note! not used directly... redundant, really
template <typename B>
auto encode([[maybe_unused]] B &builder, roq::SupportType const &value) {
  using result_type = SupportType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::TimeInForce const &value) {
  using result_type = TimeInForce;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::TradingStatus const &value) {
  using result_type = TradingStatus;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::Transport const &value) {
  using result_type = Transport;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::UpdateAction const &value) {
  using result_type = UpdateAction;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

template <typename B>
auto encode([[maybe_unused]] B &builder, roq::UpdateType const &value) {
  using result_type = UpdateType;
  using value_type = std::underlying_type_t<result_type>;
  return static_cast<result_type>(static_cast<value_type>(value));
}

// helper

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
      value.remaining_quantity,
      encode(builder, value.action),
      value.priority,
      encode(builder, static_cast<std::string_view>(value.order_id)));
}

template <typename B>
auto encode(B &builder, roq::Trade const &value) {
  return CreateTrade(
      builder,
      encode(builder, value.side),
      value.price,
      value.quantity,
      encode(builder, static_cast<std::string_view>(value.trade_id)));
}

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

// std::span

template <typename B>
auto encode(B &builder, std::span<roq::MBPUpdate> const &value) {
  std::vector<flatbuffers::Offset<MBPUpdate>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<roq::MBOUpdate> const &value) {
  std::vector<flatbuffers::Offset<MBOUpdate>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<roq::Trade> const &value) {
  std::vector<flatbuffers::Offset<Trade>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<roq::Fill> const &value) {
  std::vector<flatbuffers::Offset<Fill>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<roq::Statistics> const &value) {
  std::vector<flatbuffers::Offset<Statistics>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<roq::Measurement> const &value) {
  std::vector<flatbuffers::Offset<Measurement>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, item));
    }
  }
  return builder.CreateVector(result);
}

template <typename B, std::size_t N>
auto encode(B &builder, std::span<String<N> const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

// XXX following is too much -- we need some templating here

template <typename B>
auto encode(B &builder, std::span<Source const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<User const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<Account const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<Exchange const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<Symbol const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<Currency const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<OrderTemplate const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<ExternalAccount const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<ExternalOrderId const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<ExternalTradeId const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<RoutingId const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<ClOrdId const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<RequestId const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<Label const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<MeasurementKey const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<Description const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
}

template <typename B>
auto encode(B &builder, std::span<TimeZone const> const &value) {
  std::vector<flatbuffers::Offset<flatbuffers::String>> result;
  auto size = std::size(value);
  if (size) {
    result.reserve(size);
    for (auto const &item : value) {
      result.emplace_back(encode(builder, static_cast<std::string_view>(item)));
    }
  }
  return builder.CreateVector(result);
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
      encode(builder, value.oms_request_id_type));
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
      value.encoding.get());
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
      value.exchange_sequence);
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
      value.checksum);
}

template <typename B>
auto encode(B &builder, roq::MarketByOrderUpdate const &value) {
  return CreateMarketByOrderUpdate(
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
      value.checksum);
}

template <typename B>
auto encode(B &builder, roq::TradeSummary const &value) {
  return CreateTradeSummary(
      builder,
      value.stream_id,
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.trades),
      encode(builder, value.exchange_time_utc));
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
      encode(builder, value.exchange_time_utc));
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
      encode(builder, value.order_template),
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
      value.conditional_on_version);
}

template <typename B>
auto encode(B &builder, roq::CancelOrder const &value) {
  return CreateCancelOrder(
      builder,
      encode(builder, value.account),
      value.order_id,
      encode(builder, value.routing_id),
      value.version,
      value.conditional_on_version);
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
      encode(builder, value.round_trip_latency));
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
      encode(builder, value.order_template),
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
      encode(builder, value.user));
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
      encode(builder, value.user));
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
      value.long_quantity_begin,
      value.short_quantity_begin);
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
      encode(builder, value.external_account));
}

template <typename B>
auto encode(B &builder, roq::CustomMetrics const &value) {
  return CreateCustomMetrics(
      builder,
      encode(builder, value.label),
      encode(builder, value.account),
      encode(builder, value.exchange),
      encode(builder, value.symbol),
      encode(builder, value.measurements));
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
      encode(builder, value.measurements));
}

// events

template <typename B>
flatbuffers::Offset<Event> encode(B &builder, roq::Event<roq::DownloadBegin> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::DownloadBegin, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::DownloadEnd> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::DownloadEnd, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::GatewaySettings> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::GatewaySettings, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::StreamStatus> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::StreamStatus, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::ExternalLatency> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::ExternalLatency, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::RateLimitTrigger> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::RateLimitTrigger, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::GatewayStatus> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::GatewayStatus, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::ReferenceData> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::ReferenceData, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::MarketStatus> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::MarketStatus, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::TopOfBook> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::TopOfBook, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::MarketByPriceUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::MarketByPriceUpdate, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::MarketByOrderUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::MarketByOrderUpdate, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::TradeSummary> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::TradeSummary, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::StatisticsUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::StatisticsUpdate, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::CreateOrder> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::CreateOrder, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::ModifyOrder> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::ModifyOrder, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::CancelOrder> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::CancelOrder, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::CancelAllOrders> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::CancelAllOrders, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::OrderAck> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::OrderAck, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::OrderUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::OrderUpdate, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::TradeUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::TradeUpdate, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::PositionUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::PositionUpdate, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::FundsUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::FundsUpdate, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::CustomMetrics> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::CustomMetrics, encode(builder, event.value).Union());
}

template <typename B>
auto encode(B &builder, roq::Event<roq::CustomMetricsUpdate> const &event) {
  return CreateEvent(
      builder, encode(builder, event.message_info), Message::CustomMetricsUpdate, encode(builder, event.value).Union());
}

}  // namespace fbs
}  // namespace roq
