/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/fixed_string.h"
#include "roq/span.h"
#include "roq/uuid.h"

#include "roq/statistics_type.h"

namespace roq {

//! Represents a single statistic
struct ROQ_PUBLIC Statistics final {
  StatisticsType type = StatisticsType::UNDEFINED;          //!< Statistics type
  double value = std::numeric_limits<double>::quiet_NaN();  //!< Value
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
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(type={}, )"
        R"(value={})"
        R"(}})"sv,
        value.type,
        value.value);
  }
};
