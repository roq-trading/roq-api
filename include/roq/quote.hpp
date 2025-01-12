/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/event.hpp"
#include "roq/limits.hpp"
#include "roq/name.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Quote
struct ROQ_PUBLIC Quote final {
  roq::Exchange exchange;        //!< Exchange
  roq::Symbol symbol;            //!< Symbol
  double bid_price = roq::NaN;   //!< Bid price level
  double bid_quantity = {};      //!< Total quantity available at bid
  double ask_price = roq::NaN;   //!< Ask price level
  double ask_quantity = {};      //!< Total quantity available at ask
  uint16_t quote_set_id = {};    //!< Quote Set ID (can be used to cancel quotes)
  uint32_t quote_entry_id = {};  //!< Quote ID (when supported by exchange)
};

template <>
inline constexpr std::string_view get_name<Quote>() {
  using namespace std::literals;
  return "quote"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Quote> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Quote const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(bid_price={}, )"
        R"(bid_quantity={}, )"
        R"(ask_price={}, )"
        R"(ask_quantity={}, )"
        R"(quote_set_id={}, )"
        R"(quote_entry_id={})"
        R"(}})"sv,
        value.exchange,
        value.symbol,
        value.bid_price,
        value.bid_quantity,
        value.ask_price,
        value.ask_quantity,
        value.quote_set_id,
        value.quote_entry_id);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Quote>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::Quote> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(quote={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Quote>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::Quote> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(quote={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
