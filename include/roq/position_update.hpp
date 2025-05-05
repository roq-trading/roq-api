/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

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
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
constexpr std::string_view get_name<PositionUpdate>() {
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
        R"(exchange_sequence={}, )"
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
        value.exchange_sequence,
        value.sending_time_utc);
  }
};
