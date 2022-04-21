/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Enumeration of stream types
enum class StreamType : uint8_t {
  UNDEFINED = 0,
  FIX,         //!< FIX
  WEB_SOCKET,  //!< Web-Socket
  REST,        //!< REST
  SBE,         //!< Simple Binary Encoding
};

}  // namespace roq

template <>
struct fmt::formatter<roq::StreamType> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::StreamType &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
