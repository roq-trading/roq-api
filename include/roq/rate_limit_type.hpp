/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/compile.h>
#include <fmt/format.h>

namespace roq {

//! Rate limit type
enum class RateLimitType : uint8_t {
  UNDEFINED = 0,
  ORDER_ACTION,  //!< Any order action: create, modify, cancel, etc.
  CREATE_ORDER,  //!< Only create order
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RateLimitType> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RateLimitType const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
