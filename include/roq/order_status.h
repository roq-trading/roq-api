/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"
#include "roq/format.h"
#include "roq/literals.h"

namespace roq {

//! Enumeration of order status types
struct ROQ_PACKED OrderStatus final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    SENT,
    REJECTED,
    ACCEPTED,
    PENDING,
    WORKING,
    COMPLETED,
    CANCELED,
  };

  constexpr OrderStatus() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr OrderStatus(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr OrderStatus(uint8_t type) : type_(validate(type)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::SENT:
        return "SENT"_sv;
      case type_t::REJECTED:
        return "REJECTED"_sv;
      case type_t::ACCEPTED:
        return "ACCEPTED"_sv;
      case type_t::PENDING:
        return "PENDING"_sv;
      case type_t::WORKING:
        return "WORKING"_sv;
      case type_t::COMPLETED:
        return "COMPLETED"_sv;
      case type_t::CANCELED:
        return "CANCELED"_sv;
      default:
        assert(false);
    }
    return "UNDEFINED"_sv;
  }

  constexpr operator std::string_view() const { return name(); }

 protected:
  constexpr type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::SENT:
      case type_t::REJECTED:
      case type_t::ACCEPTED:
      case type_t::PENDING:
      case type_t::WORKING:
      case type_t::COMPLETED:
      case type_t::CANCELED:
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
struct std::is_enum<roq::OrderStatus> : std::true_type {};

template <>
struct std::underlying_type<roq::OrderStatus> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::OrderStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
