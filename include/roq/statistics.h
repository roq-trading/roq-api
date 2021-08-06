/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/literals.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"
#include "roq/uuid.h"

#include "roq/statistics_type.h"

namespace roq {

//! Represents a single statistic
struct ROQ_PUBLIC Statistics final {
  StatisticsType type = {};                  //!< Statistics type
  double value = NaN;                        //!< Value
  std::chrono::seconds begin_time_utc = {};  //!< Sample period begin time
  std::chrono::seconds end_time_utc = {};    //!< Sample period end time
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Statistics> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Statistics &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(type={}, )"
        R"(value={}, )"
        R"(begin_time_utc={}, )"
        R"(end_time_utc={})"
        R"(}})"_sv,
        value.type,
        value.value,
        value.begin_time_utc,
        value.end_time_utc);
  }
};
