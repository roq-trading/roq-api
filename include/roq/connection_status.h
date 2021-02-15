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

//! Enumeration of connection status types
struct ROQ_PACKED ConnectionStatus final {
  //! helper
  enum type_t : uint8_t { UNDEFINED = 0, DISCONNECTED, CONNECTED, MAX = CONNECTED };

  ConnectionStatus() = default;

  // cppcheck-suppress noExplicitConstructor
  inline ConnectionStatus(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  inline explicit ConnectionStatus(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::DISCONNECTED:
        return "DISCONNECTED"_sv;
      case type_t::CONNECTED:
        return "CONNECTED"_sv;
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
      case type_t::DISCONNECTED:
      case type_t::CONNECTED:
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
struct std::is_enum<roq::ConnectionStatus> : std::true_type {};

template <>
struct std::underlying_type<roq::ConnectionStatus> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::ConnectionStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::ConnectionStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
