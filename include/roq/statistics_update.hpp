/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/statistics.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to statistics published by the exchange
struct ROQ_PUBLIC StatisticsUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  std::span<roq::Statistics const> statistics;      //!< List of statistics
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
inline constexpr std::string_view get_name<StatisticsUpdate>() {
  using namespace std::literals;
  return "statistics_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::StatisticsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::StatisticsUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(statistics=[{}], )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(sending_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        fmt::join(value.statistics, ", "sv),
        value.update_type,
        value.exchange_time_utc,
        value.exchange_sequence,
        value.sending_time_utc);
  }
};
