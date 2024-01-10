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
#include "roq/layer.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to top of book (aggregate price)
struct ROQ_PUBLIC TopOfBook final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  Layer layer;                                      //!< Top of book
  UpdateType update_type = {};                      //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
inline constexpr std::string_view get_name<TopOfBook>() {
  using namespace std::literals;
  return "top_of_book"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::TopOfBook> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TopOfBook const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(layer={}, )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(sending_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.layer,
        value.update_type,
        value.exchange_time_utc,
        value.exchange_sequence,
        value.sending_time_utc);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::TopOfBook>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::TopOfBook> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(top_of_book={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::TopOfBook>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::TopOfBook> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(top_of_book={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
