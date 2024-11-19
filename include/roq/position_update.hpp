/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <chrono>
#include <string_view>

#include "roq/event.hpp"
#include "roq/limits.hpp"
#include "roq/margin_mode.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to current position for a symbol/side/account
struct ROQ_PUBLIC PositionUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view account;                         //!< Account name
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  roq::MarginMode margin_mode = {};                 //!< Margin mode
  std::string_view external_account;                //!< External account name
  double long_quantity = roq::NaN;                  //!< Current long position (absolute)
  double short_quantity = roq::NaN;                 //!< Current short position (absolute)
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
inline constexpr std::string_view get_name<PositionUpdate>() {
  using namespace std::literals;
  return "position_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::PositionUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::PositionUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(margin_mode={}, )"
        R"(external_account="{}", )"
        R"(long_quantity={}, )"
        R"(short_quantity={}, )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(sending_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.exchange,
        value.symbol,
        value.margin_mode,
        value.external_account,
        value.long_quantity,
        value.short_quantity,
        value.update_type,
        value.exchange_time_utc,
        value.sending_time_utc);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::PositionUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::PositionUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(position_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::PositionUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::PositionUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(position_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
