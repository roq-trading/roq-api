/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Enumeration of order cancel policies
enum class OrderCancelPolicy : uint8_t {
  UNDEFINED = 0,
  MANAGED_ORDERS,
  BY_ACCOUNT,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderCancelPolicy> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::OrderCancelPolicy &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
