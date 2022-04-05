/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/numbers.hpp"
#include "roq/string_types.hpp"
#include "roq/uuid.hpp"

#include "roq/statistics_type.hpp"

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
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Statistics &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(type={}, )"
        R"(value={}, )"
        R"(begin_time_utc={}, )"
        R"(end_time_utc={})"
        R"(}})"sv,
        value.type,
        value.value,
        value.begin_time_utc,
        value.end_time_utc);
  }
};
