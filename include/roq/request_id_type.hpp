/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/compile.h>
#include <fmt/format.h>

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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RequestIdType const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
