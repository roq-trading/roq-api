/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>

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
  double balance = roq::NaN;                        //!< Current funds
  double hold = roq::NaN;                           //!< Funds on hold
  std::string_view external_account;                //!< External account name
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
inline constexpr std::string_view get_name<FundsUpdate>() {
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
        R"(external_account="{}", )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(sending_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.currency,
        value.margin_mode,
        value.balance,
        value.hold,
        value.external_account,
        value.update_type,
        value.exchange_time_utc,
        value.sending_time_utc);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::FundsUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::FundsUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(funds_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::FundsUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::FundsUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(funds_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
