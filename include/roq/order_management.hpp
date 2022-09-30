/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {

//! Enumeration of a specific order management style
enum class OrderManagement : uint8_t {
  UNDEFINED = 0,
  FIX,  //!< Client provides a globally unique routing_id
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderManagement> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::OrderManagement const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
