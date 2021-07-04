/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cassert>
#include <string_view>
#include <type_traits>

#include <magic_enum.hpp>

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

  explicit constexpr Error(uint8_t value) : type_(validate(value)) {}

  explicit constexpr Error(const std::string_view &value) : type_(validate(value)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const { return magic_enum::enum_name(type_); }

  constexpr operator std::string_view() const { return name(); }

  static constexpr size_t count() { return magic_enum::enum_count<type_t>(); }

  static constexpr Error from_index(size_t index) { return magic_enum::enum_value<type_t>(index); }

  constexpr size_t to_index() const {
    auto result = magic_enum::enum_index(type_);  // std::optional
    return result.value();                        // note! could throw
  }

 protected:
  template <typename T>
  constexpr type_t validate(T value) {
    auto result = magic_enum::enum_cast<type_t>(value);
    return result.has_value() ? result.value() : type_t::UNDEFINED;
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
    return roq::format_to(context.out(), "{}"_sv, value.name());
  }
};
