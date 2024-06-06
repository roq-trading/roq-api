/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>

#include "roq/statistics_type.hpp"

namespace roq {

//! Represents a single statistic
struct ROQ_PUBLIC Statistics final {
  roq::StatisticsType type = {};                            //!< Statistics type
  double value = std::numeric_limits<double>::quiet_NaN();  //!< Value
  std::chrono::seconds begin_time_utc = {};                 //!< Sample period begin time
  std::chrono::seconds end_time_utc = {};                   //!< Sample period end time
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Statistics> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Statistics const &value, format_context &context) const {
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
