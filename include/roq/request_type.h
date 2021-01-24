/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"

namespace roq {

//! Enumeration of request types
struct ROQ_PACKED RequestType final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    CREATE_ORDER,
    MODIFY_ORDER,
    CANCEL_ORDER,
    MAX = CANCEL_ORDER
  };

  RequestType() = default;

  // cppcheck-suppress noExplicitConstructor
  inline RequestType(type_t type)  // NOLINT
      : type_(type) {}

  inline explicit RequestType(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    constexpr std::string_view UNDEFINED("UNDEFINED");
    constexpr std::string_view CREATE_ORDER("CREATE_ORDER");
    constexpr std::string_view MODIFY_ORDER("MODIFY_ORDER");
    constexpr std::string_view CANCEL_ORDER("CANCEL_ORDER");
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::CREATE_ORDER:
        return CREATE_ORDER;
      case type_t::MODIFY_ORDER:
        return MODIFY_ORDER;
      case type_t::CANCEL_ORDER:
        return CANCEL_ORDER;
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
      case type_t::CREATE_ORDER:
      case type_t::MODIFY_ORDER:
      case type_t::CANCEL_ORDER:
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
struct std::is_enum<roq::RequestType> : std::true_type {};

template <>
struct std::underlying_type<roq::RequestType> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::RequestType> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::RequestType &value, Context &context) {
    return format_to(context.out(), "{}", value.name());
  }
};
