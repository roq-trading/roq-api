/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"

namespace roq {

//! Enumeration of the tradig status of a symbol
struct ROQ_PACKED TradingStatus final {
  //! helper
  enum type_t : uint8_t { UNDEFINED = 0, CLOSED, OPEN, MAX = OPEN };

  TradingStatus() = default;

  // cppcheck-suppress noExplicitConstructor
  inline TradingStatus(type_t type)  // NOLINT
      : type_(type) {}

  inline explicit TradingStatus(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    constexpr std::string_view UNDEFINED("UNDEFINED");
    constexpr std::string_view CLOSED("CLOSED");
    constexpr std::string_view OPEN("OPEN");
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::CLOSED:
        return CLOSED;
      case type_t::OPEN:
        return OPEN;
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
      case type_t::CLOSED:
      case type_t::OPEN:
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
struct std::is_enum<roq::TradingStatus> : std::true_type {};

template <>
struct std::underlying_type<roq::TradingStatus> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::TradingStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::TradingStatus &value, Context &context) {
    return format_to(context.out(), "{}", value.name());
  }
};
