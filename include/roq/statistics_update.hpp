/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string.hpp"

#include "roq/statistics.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to statistics published by the exchange
struct ROQ_PUBLIC StatisticsUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  std::span<Statistics> statistics;                 //!< List of statistics
  UpdateType update_type;                           //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::StatisticsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::StatisticsUpdate &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(statistics=[{}], )"
        R"(update_type={}, )"
        R"(exchange_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        fmt::join(value.statistics, ", "sv),
        value.update_type,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::StatisticsUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::StatisticsUpdate> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(statistics_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
