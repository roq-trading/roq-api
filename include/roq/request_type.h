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
  inline RequestType(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  inline explicit RequestType(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::CREATE_ORDER:
        return "CREATE_ORDER"_sv;
      case type_t::MODIFY_ORDER:
        return "MODIFY_ORDER"_sv;
      case type_t::CANCEL_ORDER:
        return "CANCEL_ORDER"_sv;
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
struct fmt::formatter<roq::RequestType> : public roq::formatter {
  template <typename Context>
  auto format(const roq::RequestType &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
