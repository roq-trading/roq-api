/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Request identifier
enum class RequestIdType : uint8_t {
  UNDEFINED = 0,
  BASE64,
  BASE32,
  SIMPLE,
  UUID,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RequestIdType> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RequestIdType const &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
