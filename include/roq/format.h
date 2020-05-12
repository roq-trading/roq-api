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
      return format_to(
          ctx.out(),
          FMT_STRING(
              R"({{)"
              R"(}})"));
    return format_to(
        ctx.out(),
        FMT_STRING(
            R"({{)"
            R"("{}")"
            R"(}})"),
        fmt::join(
            value,
            R"(", ")"));
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
        FMT_STRING(R"("{}"={})"),
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
        FMT_STRING(
            R"({{)"
            R"({})"
            R"(}})"),
        fmt::join(
            value.begin(),
            value.end(),
            R"(, )"));
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
        EnumNameTimeInForce(value));
  }
};

template <>
struct fmt::formatter<roq::ExecutionInstruction> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::ExecutionInstruction value, T& ctx) {
    return format_to(
        ctx.out(),
        FMT_STRING(R"({})"),
        EnumNameExecutionInstruction(value));
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(R"({})"),
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
        FMT_STRING(
            R"({{)"
            R"(price={}, )"
            R"(quantity={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(price={}, )"
            R"(remaining_quantity={},)"
            R"(action={}, )"
            R"(priority={}, )"
            R"(order_id="{}")"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(price={}, )"
            R"(quantity={}, )"
            R"(side={}, )"
            R"(trade_id="{}")"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(source={}, )"
            R"(source_name="{}", )"
            R"(source_session_id="{}", )"
            R"(source_seqno={}, )"
            R"(receive_time_utc={}, )"
            R"(receive_time={}, )"
            R"(source_send_time={}, )"
            R"(source_receive_time={}, )"
            R"(origin_create_time={}, )"
            R"(is_last={}, )"
            R"(opaque={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(accounts={}, )"
            R"(symbols_by_exchange={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}")"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(max_order_id={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(status={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(status={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(security_type={}, )"
            R"(currency="{}", )"
            R"(settlement_currency="{}", )"
            R"(commission_currency="{}", )"
            R"(tick_size={}, )"
            R"(limit_up={}, )"
            R"(limit_down={}, )"
            R"(multiplier={}, )"
            R"(min_trade_vol={}, )"
            R"(option_type={}, )"
            R"(strike_currency="{}", )"
            R"(strike_price={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(trading_status={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(layer={}, )"
            R"(snapshot={}, )"
            R"(exchange_time_utc={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(bids=[{}], )"
            R"(asks=[{}], )"
            R"(snapshot={}, )"
            R"(exchange_time_utc={})"
            R"(}})"),
        value.exchange,
        value.symbol,
        fmt::join(
            value.bids,
            R"(, )"),
        fmt::join(
            value.asks,
            R"(, )"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(bids=[{}], )"
            R"(asks=[{}], )"
            R"(snapshot={}, )"
            R"(exchange_time_utc={})"
            R"(}})"),
        value.exchange,
        value.symbol,
        fmt::join(
            value.bids,
            R"(, )"),
        fmt::join(
            value.asks,
            R"(, )"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(trades=[{}], )"
            R"(exchange_time_utc={})"
            R"(}})"),
        value.exchange,
        value.symbol,
        fmt::join(
            value.trades,
            R"(, )"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(pre_open_interest={}, )"
            R"(pre_settlement_price={}, )"
            R"(pre_close_price={}, )"
            R"(highest_traded_price={}, )"
            R"(lowest_traded_price={}, )"
            R"(upper_limit_price={}, )"
            R"(lower_limit_price={}, )"
            R"(exchange_time_utc={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(open_price={}, )"
            R"(settlement_price={}, )"
            R"(close_price={}, )"
            R"(open_interest={}, )"
            R"(exchange_time_utc={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(order_id={}, )"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(side={}, )"
            R"(quantity={}, )"
            R"(order_type={}, )"
            R"(price={}, )"
            R"(time_in_force={}, )"
            R"(position_effect={}, )"
            R"(execution_instruction={}, )"
            R"(stop_price={}, )"
            R"(max_show_quantity={}, )"
            R"(order_template="{}")"
            R"(}})"),
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
        value.execution_instruction,
        value.stop_price,
        value.max_show_quantity,
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(order_id={}, )"
            R"(quantity={}, )"
            R"(price={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(order_id={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(order_id={}, )"
            R"(type={}, )"
            R"(origin={}, )"
            R"(status={}, )"
            R"(error={}, )"
            R"(text="{}", )"
            R"(gateway_order_id={}, )"
            R"(external_order_id="{}", )"
            R"(request_id="{}")"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(order_id={}, )"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(status={}, )"
            R"(side={}, )"
            R"(price={}, )"
            R"(remaining_quantity={}, )"
            R"(traded_quantity={}, )"
            R"(position_effect={}, )"
            R"(order_template="{}", )"
            R"(create_time_utc={}, )"
            R"(update_time_utc={}, )"
            R"(gateway_order_id={}, )"
            R"(external_order_id="{}")"
            R"(}})"),
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
        value.gateway_order_id,
        value.external_order_id);
  }
};

template <>
struct fmt::formatter<roq::Fill> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Fill& value, T& ctx) {
    return format_to(
        ctx.out(),
        FMT_STRING(
            R"({{)"
            R"(trade_id={}, )"
            R"(quantity={}, )"
            R"(price={}, )"
            R"(gateway_trade_id={}, )"
            R"(external_trade_id="{}")"
            R"(}})"),
        value.trade_id,
        value.quantity,
        value.price,
        value.gateway_trade_id,
        value.external_trade_id);
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(order_id={}, )"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(side={}, )"
            R"(position_effect={}, )"
            R"(order_template="{}", )"
            R"(create_time_utc={}, )"
            R"(update_time_utc={}, )"
            R"(gateway_order_id={}, )"
            R"(external_order_id="{}", )"
            R"(fills=[{}])"
            R"(}})"),
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.order_template,
        value.create_time_utc,
        value.update_time_utc,
        value.gateway_order_id,
        value.external_order_id,
        fmt::join(
            value.fills,
            R"(, )"));
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(exchange="{}", )"
            R"(symbol="{}", )"
            R"(side={}, )"
            R"(position={}, )"
            R"(last_trade_id={}, )"
            R"(position_cost={}, )"
            R"(position_yesterday={}, )"
            R"(position_cost_yesterday={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(account="{}", )"
            R"(currency="{}", )"
            R"(balance={}, )"
            R"(hold={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(length={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(}})"));
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
        FMT_STRING(
            R"({{)"
            R"(}})"));
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
        FMT_STRING(
            R"({{)"
            R"(now={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(connection_status={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(subscribe={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(download_begin={})"
            R"(}})"),
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
        FMT_STRING(
          R"({{)"
          R"(message_info={}, )"
          R"(download_end={})"
          R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(market_data_status={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(order_manager_status={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(reference_data={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(market_status={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(top_of_book={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(market_by_price={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(market_by_order={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(trade_summary={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(session_statistics={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(daily_statistics={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(create_order={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(modify_order={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(cancel_order={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(order_ack={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(order_update={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(trade_update={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(position_update={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(funds_update={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(message_info={}, )"
            R"(custom_message={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(bid_price={}, )"
            R"(bid_quantity={}, )"
            R"(ask_price={}, )"
            R"(ask_quantity={})"
            R"(}})"),
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
        FMT_STRING(
            R"({{)"
            R"(id={}, )"
            R"(name="{}", )"
            R"(user="{}", )"
            R"(login="{}", )"
            R"(password=***, )"
            R"(secret=***, )"
            R"(symbols=...)"
            R"(}})"),
        value.id,
        value.name,
        value.user,
        value.login);
        // fmt::join(value.symbols, R"(, )"));
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
        FMT_STRING(
            R"({{)"
            R"(id={}, )"
            R"(name="{}", )"
            R"(password=***, )"
            R"(accounts=[{}], )"
            R"(symbols=..., )"
            R"(limits=...)"
            R"(}})"),
        value.id,
        value.name,
        fmt::join(
            value.accounts,
            R"(, )"));
        // fmt::join(value.symbols, R"(, )"));
  }
};
