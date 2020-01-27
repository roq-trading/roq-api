/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "roq/api.h"


// NOTE!
// we're extending the fmt namespace with formatter specializations
// this is the recommended practice

// helper

// XXX make generic
template <>
struct fmt::formatter<std::set<std::string> > {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const std::set<std::string>& value, T& ctx) {
    if (value.empty())
      return format_to(ctx.out(), "{{}}");
    return format_to(
        ctx.out(),
        "{{\"{}\"}}",
        fmt::join(value, "\", \""));
  }
};

// XXX make generic
template <>
struct fmt::formatter<std::pair<const std::string, std::set<std::string> > > {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(
      const std::pair<const std::string, std::set<std::string> >& value,
      T& ctx) {
    return format_to(
        ctx.out(),
        "\"{}\"={}",
        value.first,
        value.second);
  }
};

// XXX make generic
template <>
struct fmt::formatter<std::map<std::string, std::set<std::string> > > {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(
      const std::map<std::string, std::set<std::string> >& value,
      T& ctx) {
    return format_to(
        ctx.out(),
        "{{{}}}",
        fmt::join(value.begin(), value.end(), ", "));
  }
};

// enums

template <>
struct fmt::formatter<roq::ConnectionStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::ConnectionStatus value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameConnectionStatus(value));
  }
};

template <>
struct fmt::formatter<roq::GatewayStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::GatewayStatus value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameGatewayStatus(value));
  }
};

template <>
struct fmt::formatter<roq::Side> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Side value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameSide(value));
  }
};

template <>
struct fmt::formatter<roq::OrderType> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderType value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameOrderType(value));
  }
};

template <>
struct fmt::formatter<roq::TimeInForce> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TimeInForce value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameTimeInForce(value));
  }
};

template <>
struct fmt::formatter<roq::PositionEffect> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::PositionEffect value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNamePositionEffect(value));
  }
};

template <>
struct fmt::formatter<roq::TradingStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TradingStatus value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameTradingStatus(value));
  }
};

template <>
struct fmt::formatter<roq::OrderStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderStatus value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameOrderStatus(value));
  }
};

template <>
struct fmt::formatter<roq::RequestType> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::RequestType value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameRequestType(value));
  }
};

template <>
struct fmt::formatter<roq::RequestStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::RequestStatus value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameRequestStatus(value));
  }
};

template <>
struct fmt::formatter<roq::Origin> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Origin value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameOrigin(value));
  }
};

template <>
struct fmt::formatter<roq::SecurityType> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::SecurityType value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameSecurityType(value));
  }
};

template <>
struct fmt::formatter<roq::OptionType> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OptionType value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameOptionType(value));
  }
};

template <>
struct fmt::formatter<roq::OrderUpdateAction> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderUpdateAction value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameOrderUpdateAction(value));
  }
};

template <>
struct fmt::formatter<roq::Error> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Error value, T& ctx) {
    return format_to(
        ctx.out(),
        "{}",
        EnumNameError(value));
  }
};

// helpers

template <>
struct fmt::formatter<roq::MBPUpdate> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MBPUpdate& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "price={}, "
        "quantity={}"
        "}}",
        value.price,
        value.quantity);
  }
};

template <>
struct fmt::formatter<roq::MBOUpdate> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MBOUpdate& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "price={}, "
        "remaining_quantity={},"
        "action={}, "
        "priority={}, "
        "order_id=\"{}\""
        "}}",
        value.price,
        value.remaining_quantity,
        value.action,
        value.priority,
        value.order_id);
  }
};

template <>
struct fmt::formatter<roq::Trade> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Trade& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "price={}, "
        "quantity={}, "
        "side={}, "
        "trade_id=\"{}\""
        "}}",
        value.price,
        value.quantity,
        value.side,
        get_trade_id_as_string_view(value));
  }
};

template <>
struct fmt::formatter<roq::MessageInfo> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MessageInfo& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "source={}, "
        "source_name=\"{}\", "
        "source_session_id=\"{}\", "
        "source_seqno={}, "
        "receive_time_utc={}, "
        "receive_time={}, "
        "source_send_time={}, "
        "source_receive_time={}, "
        "origin_create_time={}, "
        "is_last={}, "
        "opaque={}"
        "}}",
        value.source,
        value.source_name,
        value.source_session_id,
        value.source_seqno,
        value.receive_time_utc,
        value.receive_time,
        value.source_send_time,
        value.source_receive_time,
        value.origin_create_time,
        value.is_last,
        value.opaque);
  }
};

// messages

template <>
struct fmt::formatter<roq::Subscribe> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Subscribe& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "accounts={}, "
        "symbols_by_exchange={}"
        "}}",
        value.accounts,
        value.symbols_by_exchange);
  }
};

template <>
struct fmt::formatter<roq::DownloadBegin> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::DownloadBegin& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\""
        "}}",
        value.account);
  }
};

template <>
struct fmt::formatter<roq::DownloadEnd> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::DownloadEnd& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "max_order_id={}"
        "}}",
        value.account,
        value.max_order_id);
  }
};

template <>
struct fmt::formatter<roq::MarketDataStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketDataStatus& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "status={}"
        "}}",
        value.status);
  }
};

template <>
struct fmt::formatter<roq::OrderManagerStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderManagerStatus& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "status={}"
        "}}",
        value.account,
        value.status);
  }
};

template <>
struct fmt::formatter<roq::ReferenceData> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::ReferenceData& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "security_type={}, "
        "currency=\"{}\", "
        "settlement_currency=\"{}\", "
        "commission_currency=\"{}\", "
        "tick_size={}, "
        "limit_up={}, "
        "limit_down={}, "
        "multiplier={}, "
        "min_trade_vol={}, "
        "option_type={}, "
        "strike_currency=\"{}\", "
        "strike_price={}"
        "}}",
        value.exchange,
        value.symbol,
        value.security_type,
        value.currency,
        value.settlement_currency,
        value.commission_currency,
        value.tick_size,
        value.limit_up,
        value.limit_down,
        value.multiplier,
        value.min_trade_vol,
        value.option_type,
        value.strike_currency,
        value.strike_price);
  }
};

template <>
struct fmt::formatter<roq::MarketStatus> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketStatus& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "trading_status={}"
        "}}",
        value.exchange,
        value.symbol,
        value.trading_status);
  }
};

template <>
struct fmt::formatter<roq::TopOfBook> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TopOfBook& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "layer={}, "
        "snapshot={}, "
        "exchange_time_utc={}"
        "}}",
        value.exchange,
        value.symbol,
        value.layer,
        value.snapshot,
        value.exchange_time_utc);
  }
};

template <>
struct fmt::formatter<roq::MarketByPrice> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketByPrice& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "bids=[{}], "
        "asks=[{}], "
        "snapshot={}, "
        "exchange_time_utc={}"
        "}}",
        value.exchange,
        value.symbol,
        fmt::join(value.bids, ", "),
        fmt::join(value.asks, ", "),
        value.snapshot,
        value.exchange_time_utc);
  }
};

template <>
struct fmt::formatter<roq::MarketByOrder> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketByOrder& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "bids=[{}], "
        "asks=[{}], "
        "snapshot={}, "
        "exchange_time_utc={}"
        "}}",
        value.exchange,
        value.symbol,
        fmt::join(value.bids, ", "),
        fmt::join(value.asks, ", "),
        value.snapshot,
        value.exchange_time_utc);
  }
};

template <>
struct fmt::formatter<roq::TradeSummary> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TradeSummary& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "trades=[{}], "
        "exchange_time_utc={}"
        "}}",
        value.exchange,
        value.symbol,
        fmt::join(value.trades, ", "),
        value.exchange_time_utc);
  }
};

template <>
struct fmt::formatter<roq::SessionStatistics> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::SessionStatistics& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "pre_open_interest={}, "
        "pre_settlement_price={}, "
        "pre_close_price={}, "
        "highest_traded_price={}, "
        "lowest_traded_price={}, "
        "upper_limit_price={}, "
        "lower_limit_price={}, "
        "exchange_time_utc={}"
        "}}",
        value.exchange,
        value.symbol,
        value.pre_open_interest,
        value.pre_settlement_price,
        value.pre_close_price,
        value.highest_traded_price,
        value.lowest_traded_price,
        value.upper_limit_price,
        value.lower_limit_price,
        value.exchange_time_utc);
  }
};

template <>
struct fmt::formatter<roq::DailyStatistics> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::DailyStatistics& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "open_price={}, "
        "settlement_price={}, "
        "close_price={}, "
        "open_interest={}, "
        "exchange_time_utc={}"
        "}}",
        value.exchange,
        value.symbol,
        value.open_price,
        value.settlement_price,
        value.close_price,
        value.open_interest,
        value.exchange_time_utc);
  }
};

template <>
struct fmt::formatter<roq::CreateOrder> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::CreateOrder& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "order_id={}, "
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "side={}, "
        "quantity={}, "
        "order_type={}, "
        "price={}, "
        "time_in_force={}, "
        "position_effect={}, "
        "order_template=\"{}\""
        "}}",
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.quantity,
        value.order_type,
        value.price,
        value.time_in_force,
        value.position_effect,
        value.order_template);
  }
};

template <>
struct fmt::formatter<roq::ModifyOrder> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::ModifyOrder& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "order_id={}, "
        "quantity={}, "
        "price={}, "
        "}}",
        value.account,
        value.order_id,
        value.quantity,
        value.price);
  }
};

template <>
struct fmt::formatter<roq::CancelOrder> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::CancelOrder& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "order_id={}"
        "}}",
        value.account,
        value.order_id);
  }
};

template <>
struct fmt::formatter<roq::OrderAck> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderAck& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "order_id={}, "
        "type={}, "
        "origin={}, "
        "status={}, "
        "error={}, "
        "text=\"{}\", "
        "gateway_order_id={}, "
        "external_order_id=\"{}\", "
        "request_id=\"{}\""
        "}}",
        value.account,
        value.order_id,
        value.type,
        value.origin,
        value.status,
        value.error,
        value.text,
        value.gateway_order_id,
        value.external_order_id,
        value.request_id);
  }
};

template <>
struct fmt::formatter<roq::OrderUpdate> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderUpdate& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "order_id={}, "
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "status={}, "
        "side={}, "
        "price={}, "
        "remaining_quantity={}, "
        "traded_quantity={}, "
        "position_effect={}, "
        "order_template=\"{}\", "
        "create_time_utc={}, "
        "update_time_utc={}, "
        "commissions={}, "
        "gateway_order_id={}, "
        "external_order_id=\"{}\""
        "}}",
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.status,
        value.side,
        value.price,
        value.remaining_quantity,
        value.traded_quantity,
        value.position_effect,
        value.order_template,
        value.create_time_utc,
        value.update_time_utc,
        value.commissions,
        value.gateway_order_id,
        value.external_order_id);
  }
};

template <>
struct fmt::formatter<roq::TradeUpdate> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TradeUpdate& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "trade_id={}, "
        "order_id={}, "
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "side={}, "
        "quantity={}, "
        "price={}, "
        "position_effect={}, "
        "order_template=\"{}\", "
        "create_time_utc={}, "
        "update_time_utc={}, "
        "gateway_order_id={}, "
        "gateway_trade_id={}, "
        "external_order_id=\"{}\", "
        "external_trade_id=\"{}\""
        "}}",
        value.account,
        value.trade_id,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.quantity,
        value.price,
        value.position_effect,
        value.order_template,
        value.create_time_utc,
        value.update_time_utc,
        value.gateway_order_id,
        value.gateway_trade_id,
        value.external_order_id,
        value.external_trade_id);
  }
};

template <>
struct fmt::formatter<roq::PositionUpdate> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::PositionUpdate& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "exchange=\"{}\", "
        "symbol=\"{}\", "
        "side={}, "
        "position={}, "
        "last_trade_id={}, "
        "position_cost={}, "
        "position_yesterday={}, "
        "position_cost_yesterday={}"
        "}}",
        value.account,
        value.exchange,
        value.symbol,
        value.side,
        value.position,
        value.last_trade_id,
        value.position_cost,
        value.position_yesterday,
        value.position_cost_yesterday);
  }
};

template <>
struct fmt::formatter<roq::FundsUpdate> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::FundsUpdate& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "account=\"{}\", "
        "currency=\"{}\", "
        "balance={}, "
        "hold={}"
        "}}",
        value.account,
        value.currency,
        value.balance,
        value.hold);
  }
};

template <>
struct fmt::formatter<roq::CustomMessage> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::CustomMessage& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "length={}"
        "}}",
        value.length);
  }
};

// events

template <>
struct fmt::formatter<roq::StartEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::StartEvent&, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "}}");
  }
};

template <>
struct fmt::formatter<roq::StopEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::StopEvent&, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "}}");
  }
};

template <>
struct fmt::formatter<roq::TimerEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TimerEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "now={}"
        "}}",
        value.now);
  }
};

template <>
struct fmt::formatter<roq::ConnectionStatusEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::ConnectionStatusEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "connection_status={}"
        "}}",
        value.message_info,
        value.connection_status);
  }
};

template <>
struct fmt::formatter<roq::BatchBeginEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::BatchBeginEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}"
        "}}",
        value.message_info);
  }
};

template <>
struct fmt::formatter<roq::BatchEndEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::BatchEndEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}"
        "}}",
        value.message_info);
  }
};

template <>
struct fmt::formatter<roq::SubscribeEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::SubscribeEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "subscribe={}"
        "}}",
        value.message_info,
        value.subscribe);
  }
};

template <>
struct fmt::formatter<roq::DownloadBeginEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::DownloadBeginEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "download_begin={}"
        "}}",
        value.message_info,
        value.download_begin);
  }
};

template <>
struct fmt::formatter<roq::DownloadEndEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::DownloadEndEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "download_end={}"
        "}}",
        value.message_info,
        value.download_end);
  }
};

template <>
struct fmt::formatter<roq::MarketDataStatusEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketDataStatusEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "market_data_status={}"
        "}}",
        value.message_info,
        value.market_data_status);
  }
};

template <>
struct fmt::formatter<roq::OrderManagerStatusEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderManagerStatusEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "order_manager_status={}"
        "}}",
        value.message_info,
        value.order_manager_status);
  }
};

template <>
struct fmt::formatter<roq::ReferenceDataEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::ReferenceDataEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "reference_data={}"
        "}}",
        value.message_info,
        value.reference_data);
  }
};

template <>
struct fmt::formatter<roq::MarketStatusEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketStatusEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "market_status={}"
        "}}",
        value.message_info,
        value.market_status);
  }
};

template <>
struct fmt::formatter<roq::TopOfBookEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TopOfBookEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "top_of_book={}"
        "}}",
        value.message_info,
        value.top_of_book);
  }
};

template <>
struct fmt::formatter<roq::MarketByPriceEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketByPriceEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "market_by_price={}"
        "}}",
        value.message_info,
        value.market_by_price);
  }
};

template <>
struct fmt::formatter<roq::MarketByOrderEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::MarketByOrderEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "market_by_order={}"
        "}}",
        value.message_info,
        value.market_by_order);
  }
};

template <>
struct fmt::formatter<roq::TradeSummaryEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TradeSummaryEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "trade_summary={}"
        "}}",
        value.message_info,
        value.trade_summary);
  }
};

template <>
struct fmt::formatter<roq::SessionStatisticsEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::SessionStatisticsEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "session_statistics={}"
        "}}",
        value.message_info,
        value.session_statistics);
  }
};

template <>
struct fmt::formatter<roq::DailyStatisticsEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::DailyStatisticsEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "daily_statistics={}"
        "}}",
        value.message_info,
        value.daily_statistics);
  }
};

template <>
struct fmt::formatter<roq::CreateOrderEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::CreateOrderEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "create_order={}"
        "}}",
        value.message_info,
        value.create_order);
  }
};

template <>
struct fmt::formatter<roq::ModifyOrderEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::ModifyOrderEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "modify_order={}"
        "}}",
        value.message_info,
        value.modify_order);
  }
};

template <>
struct fmt::formatter<roq::CancelOrderEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::CancelOrderEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "cancel_order={}"
        "}}",
        value.message_info,
        value.cancel_order);
  }
};

template <>
struct fmt::formatter<roq::OrderAckEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderAckEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "order_ack={}"
        "}}",
        value.message_info,
        value.order_ack);
  }
};

template <>
struct fmt::formatter<roq::OrderUpdateEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::OrderUpdateEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "order_update={}"
        "}}",
        value.message_info,
        value.order_update);
  }
};

template <>
struct fmt::formatter<roq::TradeUpdateEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::TradeUpdateEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "trade_update={}"
        "}}",
        value.message_info,
        value.trade_update);
  }
};

template <>
struct fmt::formatter<roq::PositionUpdateEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::PositionUpdateEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "position_update={}"
        "}}",
        value.message_info,
        value.position_update);
  }
};

template <>
struct fmt::formatter<roq::FundsUpdateEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::FundsUpdateEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "funds_update={}"
        "}}",
        value.message_info,
        value.funds_update);
  }
};

template <>
struct fmt::formatter<roq::CustomMessageEvent> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::CustomMessageEvent& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "message_info={}, "
        "custom_message={}"
        "}}",
        value.message_info,
        value.custom_message);
  }
};

template <>
struct fmt::formatter<roq::Layer> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Layer& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "bid_price={}, "
        "bid_quantity={}, "
        "ask_price={}, "
        "ask_quantity={}"
        "}}",
        value.bid_price,
        value.bid_quantity,
        value.ask_price,
        value.ask_quantity);
  }
};

template <>
struct fmt::formatter<roq::Account> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Account& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "id={}, "
        "name=\"{}\", "
        "login=\"{}\", "
        "password=***, "
        "secret=***, "
        "symbols=..."
        "}}",
        value.id,
        value.name,
        value.login);
        // fmt::join(value.symbols, ", "));
  }
};

template <>
struct fmt::formatter<roq::User> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::User& value, T& ctx) {
    return format_to(
        ctx.out(),
        "{{"
        "id={}, "
        "name=\"{}\", "
        "password=***, "
        "accounts=[{}], "
        "symbols=..., "
        "limits=..."
        "}}",
        value.id,
        value.name,
        fmt::join(value.accounts, ", "));
        // fmt::join(value.symbols, ", "));
  }
};
