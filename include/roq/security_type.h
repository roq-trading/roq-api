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

//! Enumeration of security/instrument types
struct ROQ_PACKED SecurityType final {
  //! helper
  enum type_t : uint8_t { UNDEFINED = 0, SPOT, FUTURES, OPTION, MAX = OPTION };

  SecurityType() = default;

  // cppcheck-suppress noExplicitConstructor
  inline SecurityType(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  inline explicit SecurityType(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::SPOT:
        return "SPOT"_sv;
      case type_t::FUTURES:
        return "FUTURES"_sv;
      case type_t::OPTION:
        return "OPTION"_sv;
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
      case type_t::SPOT:
      case type_t::FUTURES:
      case type_t::OPTION:
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
struct std::is_enum<roq::SecurityType> : std::true_type {};

template <>
struct std::underlying_type<roq::SecurityType> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::SecurityType> : public roq::formatter {
  template <typename Context>
  auto format(const roq::SecurityType &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
