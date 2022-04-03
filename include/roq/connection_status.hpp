/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Enumeration of exchange connectivity status as seen from a gateway
enum class ConnectionStatus : uint8_t {
  UNDEFINED = 0,
  DISCONNECTED,
  CONNECTING,
  LOGIN_SENT,
  DOWNLOADING,
  READY,
  LOGGED_OUT,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ConnectionStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::ConnectionStatus &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
