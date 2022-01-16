/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/numbers.h"
#include "roq/string_buffer.h"
#include "roq/uuid.h"

namespace roq {

//! A single measurement point
struct ROQ_PUBLIC Measurement final {
  roq::string_buffer<8> name;  //!< Key
  double value = NaN;          //!< Value
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Measurement> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Measurement &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(name="{}", )"
        R"(value={})"
        R"(}})"sv,
        value.name,
        value.value);
  }
};
