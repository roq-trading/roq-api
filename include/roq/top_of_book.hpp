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
  roq::Layer layer = {};                            //!< Top of book
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
constexpr std::string_view get_name<TopOfBook>() {
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
