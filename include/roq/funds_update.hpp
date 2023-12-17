/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/event.hpp"
#include "roq/margin_mode.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to available funds
struct ROQ_PUBLIC FundsUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view account;                         //!< Account name
  MarginMode margin_mode = {};                      //!< Margin mode
  std::string_view currency;                        //!< Currency
  double balance = NaN;                             //!< Current funds
  double hold = NaN;                                //!< Funds on hold
  std::string_view external_account;                //!< External account name
  UpdateType update_type = {};                      //!< Update type
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::FundsUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(margin_mode={}, )"
        R"(currency="{}", )"
        R"(balance={}, )"
        R"(hold={}, )"
        R"(external_account="{}", )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(sending_time_utc={})"
        R"(}})"_cf,
        value.stream_id,
        value.account,
        value.margin_mode,
        value.currency,
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::FundsUpdate> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(funds_update={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::FundsUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::FundsUpdate> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(funds_update={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
