/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>

#include <chrono>

#include "roq/numbers.hpp"
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
  auto format(roq::Statistics const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(type={}, )"
        R"(value={}, )"
        R"(begin_time_utc={}, )"
        R"(end_time_utc={})"
        R"(}})"_cf,
        value.type,
        value.value,
        value.begin_time_utc,
        value.end_time_utc);
  }
};
