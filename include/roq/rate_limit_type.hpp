/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RateLimitType const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
