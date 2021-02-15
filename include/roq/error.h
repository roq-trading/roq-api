/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"
#include "roq/format.h"
#include "roq/literals.h"

namespace roq {

//! Enumeration of error types
struct ROQ_PACKED Error final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    UNKNOWN,
    GATEWAY_NOT_READY,
    INVALID_ACCOUNT,
    INVALID_EXCHANGE,
    INVALID_SYMBOL,
    INVALID_ORDER_TYPE,
    INVALID_TIME_IN_FORCE,
    INVALID_POSITION_EFFECT,
    INVALID_ORDER_TEMPLATE,
    NETWORK_ERROR,
    UNKNOWN_ERROR_ID,
    UNKNOWN_EXCHANGE_ORDER_ID,
    MODIFY_ORDER_NOT_SUPPORTED,
    INVALID_ORDER_ID,
    EXECUTION_INSTRUCTION_NOT_SUPPORTED,
    MAX = EXECUTION_INSTRUCTION_NOT_SUPPORTED
  };

  Error() = default;

  // cppcheck-suppress noExplicitConstructor
  inline Error(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  inline explicit Error(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::UNKNOWN:
        return "UNKNOWN"_sv;
      case type_t::GATEWAY_NOT_READY:
        return "GATEWAY_NOT_READY"_sv;
      case type_t::INVALID_ACCOUNT:
        return "INVALID_ACCOUNT"_sv;
      case type_t::INVALID_EXCHANGE:
        return "INVALID_EXCHANGE"_sv;
      case type_t::INVALID_SYMBOL:
        return "INVALID_SYMBOL"_sv;
      case type_t::INVALID_ORDER_TYPE:
        return "INVALID_ORDER_TYPE"_sv;
      case type_t::INVALID_TIME_IN_FORCE:
        return "INVALID_TIME_IN_FORCE"_sv;
      case type_t::INVALID_POSITION_EFFECT:
        return "INVALID_POSITION_EFFECT"_sv;
      case type_t::INVALID_ORDER_TEMPLATE:
        return "INVALID_ORDER_TEMPLATE"_sv;
      case type_t::NETWORK_ERROR:
        return "NETWORK_ERROR"_sv;
      case type_t::UNKNOWN_ERROR_ID:
        return "UNKNOWN_ERROR_ID"_sv;
      case type_t::UNKNOWN_EXCHANGE_ORDER_ID:
        return "UNKNOWN_EXCHANGE_ORDER_ID"_sv;
      case type_t::MODIFY_ORDER_NOT_SUPPORTED:
        return "MODIFY_ORDER_NOT_SUPPORTED"_sv;
      case type_t::INVALID_ORDER_ID:
        return "INVALID_ORDER_ID"_sv;
      case type_t::EXECUTION_INSTRUCTION_NOT_SUPPORTED:
        return "EXECUTION_INSTRUCTION_NOT_SUPPORTED"_sv;
      default:
        assert(false);
    }
    return "UNDEFINED"_sv;
  }

  inline operator std::string_view() const { return name(); }

 protected:
  inline type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::UNKNOWN:
      case type_t::GATEWAY_NOT_READY:
      case type_t::INVALID_ACCOUNT:
      case type_t::INVALID_EXCHANGE:
      case type_t::INVALID_SYMBOL:
      case type_t::INVALID_ORDER_TYPE:
      case type_t::INVALID_TIME_IN_FORCE:
      case type_t::INVALID_POSITION_EFFECT:
      case type_t::INVALID_ORDER_TEMPLATE:
      case type_t::NETWORK_ERROR:
      case type_t::UNKNOWN_ERROR_ID:
      case type_t::UNKNOWN_EXCHANGE_ORDER_ID:
      case type_t::MODIFY_ORDER_NOT_SUPPORTED:
      case type_t::INVALID_ORDER_ID:
      case type_t::EXECUTION_INSTRUCTION_NOT_SUPPORTED:
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
struct std::is_enum<roq::Error> : std::true_type {};

template <>
struct std::underlying_type<roq::Error> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::Error> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Error &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
