/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Enumeration of protocol types (layer 7)
enum class Protocol : uint8_t {
  UNDEFINED = 0,
  FIX,   //!< FIX
  WS,    //!< Web-Socket
  HTTP,  //!< HTTP (REST)
  SBE,   //!< Simple Binary Encoding
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Protocol> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Protocol const &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
