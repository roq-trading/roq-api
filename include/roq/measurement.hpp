/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include "roq/numbers.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! A single measurement point
struct ROQ_PUBLIC Measurement final {
  MeasurementKey name;  //!< Key
  double value = NaN;   //!< Value
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Measurement> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Measurement const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(name="{}", )"
        R"(value={})"
        R"(}})"sv,
        value.name,
        value.value);
  }
};
