/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"

namespace roq {

//! Enumeration of request status types
struct ROQ_PACKED RequestStatus final {
  //! helper
  enum type_t : uint8_t { UNDEFINED = 0, FORWARDED, ACCEPTED, REJECTED, TIMEOUT, MAX = TIMEOUT };

  RequestStatus() = default;

  // cppcheck-suppress noExplicitConstructor
  inline RequestStatus(type_t type)  // NOLINT
      : type_(type) {}

  inline explicit RequestStatus(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    using namespace std::literals;  // NOLINT
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::FORWARDED:
        return "FORWARDED"sv;
      case type_t::ACCEPTED:
        return "ACCEPTED"sv;
      case type_t::REJECTED:
        return "REJECTED"sv;
      case type_t::TIMEOUT:
        return "TIMEOUT"sv;
      default:
        assert(false);
    }
    return "UNDEFINED"sv;
  }

  inline operator std::string_view() const { return name(); }

 protected:
  inline type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::FORWARDED:
      case type_t::ACCEPTED:
      case type_t::REJECTED:
      case type_t::TIMEOUT:
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
struct std::is_enum<roq::RequestStatus> : std::true_type {};

template <>
struct std::underlying_type<roq::RequestStatus> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::RequestStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::RequestStatus &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(context.out(), "{}"sv, value.name());
  }
};
