/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Update relating to current position for a symbol/side/account
struct ROQ_PUBLIC PositionUpdate final {
  uint16_t stream_id = {};            //!< Stream identifier
  std::string_view account;           //!< Account name
  std::string_view exchange;          //!< Exchange
  std::string_view symbol;            //!< Symbol
  std::string_view external_account;  //!< External account name
  double long_quantity = NaN;         //!< Current long position
  double short_quantity = NaN;        //!< Current short position
  double long_quantity_begin = NaN;   //!< Long position at period begin
  double short_quantity_begin = NaN;  //!< Short position at period begin
};

template <>
inline constexpr std::string_view get_name<PositionUpdate>() {
  using namespace std::literals;
  return "position_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::PositionUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::PositionUpdate const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(external_account="{}", )"
        R"(long_quantity={}, )"
        R"(short_quantity={}, )"
        R"(long_quantity_begin={}, )"
        R"(short_quantity_begin={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.exchange,
        value.symbol,
        value.external_account,
        value.long_quantity,
        value.short_quantity,
        value.long_quantity_begin,
        value.short_quantity_begin);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::PositionUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::PositionUpdate> const &event, Context &context) const {
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
struct fmt::formatter<roq::Trace<roq::PositionUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::PositionUpdate> const &event, Context &context) const {
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
