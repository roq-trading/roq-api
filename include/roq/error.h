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

//! Enumeration of error types
struct ROQ_PACKED Error final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    UNKNOWN,
    NOT_SUPPORTED,  //!< Unsupported request type
    INVALID_ACCOUNT,
    INVALID_ORDER_ID,
    INVALID_EXCHANGE,
    INVALID_SYMBOL,
    INVALID_SIDE,
    INVALID_POSITION_EFFECT,
    INVALID_QUANTITY,
    INVALID_MAX_SHOW_QUANTITY,
    INVALID_ORDER_TYPE,
    INVALID_TIME_IN_FORCE,
    INVALID_EXECUTION_INSTRUCTION,
    INVALID_ORDER_TEMPLATE,
    INVALID_PRICE,
    INVALID_STOP_PRICE,
    INVALID_ROUTING_ID,
    INVALID_REQUEST_VERSION,
    INVALID_REQUEST_ID,
    INVALID_REQUEST_TYPE,
    INVALID_REQUEST_STATUS,
    INVALID_REQUEST_ARGS,       //!< Request arguments did not meet validation rules
    UNKNOWN_EXTERNAL_ORDER_ID,  //!< An external order identifier has not yet been received
    NOT_AUTHORIZED,             //!< User is not authorized
    GATEWAY_NOT_READY,          //!< Gateway not ready (could be not connected or currently downloading)
    NETWORK_ERROR,              //!< Network connection is down (message is likely to be lost)
  };

  constexpr Error() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr Error(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr Error(uint8_t type) : type_(validate(type)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::UNKNOWN:
        return "UNKNOWN"_sv;
      case type_t::NOT_SUPPORTED:
        return "NOT_SUPPORTED"_sv;
      case type_t::INVALID_ACCOUNT:
        return "INVALID_ACCOUNT"_sv;
      case type_t::INVALID_ORDER_ID:
        return "INVALID_ORDER_ID"_sv;
      case type_t::INVALID_EXCHANGE:
        return "INVALID_EXCHANGE"_sv;
      case type_t::INVALID_SYMBOL:
        return "INVALID_SYMBOL"_sv;
      case type_t::INVALID_SIDE:
        return "INVALID_SIDE"_sv;
      case type_t::INVALID_POSITION_EFFECT:
        return "INVALID_POSITION_EFFECT"_sv;
      case type_t::INVALID_QUANTITY:
        return "INVALID_QUANTITY"_sv;
      case type_t::INVALID_MAX_SHOW_QUANTITY:
        return "INVALID_MAX_SHOW_QUANTITY"_sv;
      case type_t::INVALID_ORDER_TYPE:
        return "INVALID_ORDER_TYPE"_sv;
      case type_t::INVALID_TIME_IN_FORCE:
        return "INVALID_TIME_IN_FORCE"_sv;
      case type_t::INVALID_EXECUTION_INSTRUCTION:
        return "INVALID_EXECUTION_INSTRUCTION"_sv;
      case type_t::INVALID_ORDER_TEMPLATE:
        return "INVALID_ORDER_TEMPLATE"_sv;
      case type_t::INVALID_PRICE:
        return "INVALID_PRICE"_sv;
      case type_t::INVALID_STOP_PRICE:
        return "INVALID_STOP_PRICE"_sv;
      case type_t::INVALID_ROUTING_ID:
        return "INVALID_ROUTING_ID"_sv;
      case type_t::INVALID_REQUEST_VERSION:
        return "INVALID_REQUEST_VERSION"_sv;
      case type_t::INVALID_REQUEST_ID:
        return "INVALID_REQUEST_ID"_sv;
      case type_t::INVALID_REQUEST_TYPE:
        return "INVALID_REQUEST_TYPE"_sv;
      case type_t::INVALID_REQUEST_STATUS:
        return "INVALID_REQUEST_STATUS"_sv;
      case type_t::INVALID_REQUEST_ARGS:
        return "INVALID_REQUEST_ARGS"_sv;
      case type_t::UNKNOWN_EXTERNAL_ORDER_ID:
        return "UNKNOWN_EXTERNAL_ORDER_ID"_sv;
      case type_t::NOT_AUTHORIZED:
        return "NOT_AUTHORIZED"_sv;
      case type_t::GATEWAY_NOT_READY:
        return "GATEWAY_NOT_READY"_sv;
      case type_t::NETWORK_ERROR:
        return "NETWORK_ERROR"_sv;
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
      case type_t::UNKNOWN:
      case type_t::NOT_SUPPORTED:
      case type_t::INVALID_ACCOUNT:
      case type_t::INVALID_ORDER_ID:
      case type_t::INVALID_EXCHANGE:
      case type_t::INVALID_SYMBOL:
      case type_t::INVALID_SIDE:
      case type_t::INVALID_POSITION_EFFECT:
      case type_t::INVALID_QUANTITY:
      case type_t::INVALID_MAX_SHOW_QUANTITY:
      case type_t::INVALID_ORDER_TYPE:
      case type_t::INVALID_TIME_IN_FORCE:
      case type_t::INVALID_EXECUTION_INSTRUCTION:
      case type_t::INVALID_ORDER_TEMPLATE:
      case type_t::INVALID_PRICE:
      case type_t::INVALID_STOP_PRICE:
      case type_t::INVALID_ROUTING_ID:
      case type_t::INVALID_REQUEST_VERSION:
      case type_t::INVALID_REQUEST_ID:
      case type_t::INVALID_REQUEST_TYPE:
      case type_t::INVALID_REQUEST_STATUS:
      case type_t::INVALID_REQUEST_ARGS:
      case type_t::UNKNOWN_EXTERNAL_ORDER_ID:
      case type_t::NOT_AUTHORIZED:
      case type_t::GATEWAY_NOT_READY:
      case type_t::NETWORK_ERROR:
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
