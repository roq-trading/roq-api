/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/statistics.h"

namespace roq {

//! Update relating to statistics published by the exchange
struct ROQ_PUBLIC StatisticsUpdate final {
  uint16_t stream_id = {};           //!< Stream identifier
  std::string_view exchange;         //!< Exchange name
  std::string_view symbol;           //!< Symbol
  roq::span<Statistics> statistics;  //!< List of statistics
  bool snapshot = false;             //!< Full update (possibly old) if true and otherwise an incremental update
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::StatisticsUpdate> : public roq::formatter {
  template <typename Context>
  auto format(const roq::StatisticsUpdate &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(statistics=[{}], )"
        R"(snapshot={}, )"
        R"(exchange_time_utc={})"
        R"(}})"_fmt,
        value.stream_id,
        value.exchange,
        value.symbol,
        roq::join(value.statistics, ", "_sv),
        value.snapshot,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::StatisticsUpdate> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::StatisticsUpdate> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(statistics_update={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
