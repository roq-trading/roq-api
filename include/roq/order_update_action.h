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

//! Enumeration of order update action types
struct ROQ_PACKED OrderUpdateAction final {
  //! helper
  enum type_t : uint8_t { UNDEFINED = 0, NEW, MODIFY, REMOVE, MAX = REMOVE };

  OrderUpdateAction() = default;

  // cppcheck-suppress noExplicitConstructor
  inline OrderUpdateAction(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  inline explicit OrderUpdateAction(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::NEW:
        return "NEW"_sv;
      case type_t::MODIFY:
        return "MODIFY"_sv;
      case type_t::REMOVE:
        return "REMOVE"_sv;
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
      case type_t::NEW:
      case type_t::MODIFY:
      case type_t::REMOVE:
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
struct std::is_enum<roq::OrderUpdateAction> : std::true_type {};

template <>
struct std::underlying_type<roq::OrderUpdateAction> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::OrderUpdateAction> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderUpdateAction &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
