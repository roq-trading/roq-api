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

//! Update relating to available funds
struct ROQ_PUBLIC FundsUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view account;                         //!< Account name
  std::string_view currency;                        //!< Currency
  roq::MarginMode margin_mode = {};                 //!< Margin mode
  double balance = roq::NaN;                        //!< Total funds
  double hold = roq::NaN;                           //!< Funds on hold (aka locked)
  double borrowed = roq::NaN;                       //!< Borrowed funds (margin trading)
  std::string_view external_account;                //!< External account name
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
constexpr std::string_view get_name<FundsUpdate>() {
  using namespace std::literals;
  return "funds_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::FundsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::FundsUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(currency="{}", )"
        R"(margin_mode={}, )"
        R"(balance={}, )"
        R"(hold={}, )"
        R"(borrowed={}, )"
        R"(external_account="{}", )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(sending_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.currency,
        value.margin_mode,
        value.balance,
        value.hold,
        value.borrowed,
        value.external_account,
        value.update_type,
        value.exchange_time_utc,
        value.exchange_sequence,
        value.sending_time_utc);
  }
};