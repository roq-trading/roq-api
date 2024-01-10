/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Measurement const &value, format_context &context) const {
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
