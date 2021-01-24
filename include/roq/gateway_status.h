/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"

namespace roq {

//! Enumeration of exchange connectivity status as seen from a gateway
struct ROQ_PACKED GatewayStatus final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    DISCONNECTED,
    CONNECTING,
    LOGIN_SENT,
    DOWNLOADING,
    READY,
    LOGGED_OUT,
    MAX = LOGGED_OUT
  };

  GatewayStatus() = default;

  // cppcheck-suppress noExplicitConstructor
  inline GatewayStatus(type_t type)  // NOLINT
      : type_(type) {}

  inline explicit GatewayStatus(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    constexpr std::string_view UNDEFINED("UNDEFINED");
    constexpr std::string_view DISCONNECTED("DISCONNECTED");
    constexpr std::string_view CONNECTING("CONNECTING");
    constexpr std::string_view LOGIN_SENT("LOGIN_SENT");
    constexpr std::string_view DOWNLOADING("DOWNLOADING");
    constexpr std::string_view READY("READY");
    constexpr std::string_view LOGGED_OUT("LOGGED_OUT");
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::DISCONNECTED:
        return DISCONNECTED;
      case type_t::CONNECTING:
        return CONNECTING;
      case type_t::LOGIN_SENT:
        return LOGIN_SENT;
      case type_t::DOWNLOADING:
        return DOWNLOADING;
      case type_t::READY:
        return READY;
      case type_t::LOGGED_OUT:
        return LOGGED_OUT;
      default:
        assert(false);
    }
    return UNDEFINED;
  }

  inline operator std::string_view() const { return name(); }

 protected:
  inline type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::DISCONNECTED:
      case type_t::CONNECTING:
      case type_t::LOGIN_SENT:
      case type_t::DOWNLOADING:
      case type_t::READY:
      case type_t::LOGGED_OUT:
        return result;
      default:
        assert(false);
        return type_t::UNDEFINED;
    }
  }

 private:
  type_t type_ = type_t::UNDEFINED;
};

}  // namespace roq

template <>
struct std::is_enum<roq::GatewayStatus> : std::true_type {};

template <>
struct std::underlying_type<roq::GatewayStatus> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::GatewayStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::GatewayStatus &value, Context &context) {
    return format_to(context.out(), "{}", value.name());
  }
};
