/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Enumeration of request types
enum class RequestType : uint8_t {
  UNDEFINED = 0,
  CREATE_ORDER,
  MODIFY_ORDER,
  CANCEL_ORDER,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RequestType> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::RequestType &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
