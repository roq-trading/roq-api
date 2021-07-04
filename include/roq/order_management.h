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

//! Enumeration of a specific order management style
struct ROQ_PACKED OrderManagement final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    FIX,  //!< Client provides a globally unique routing_id
  };

  constexpr OrderManagement() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr OrderManagement(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr OrderManagement(uint8_t value) : type_(validate(value)) {}

  explicit constexpr OrderManagement(const std::string_view &value) : type_(validate(value)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const { return magic_enum::enum_name(type_); }

  constexpr operator std::string_view() const { return name(); }

  static constexpr size_t count() { return magic_enum::enum_count<type_t>(); }

  static constexpr OrderManagement from_index(size_t index) { return magic_enum::enum_value<type_t>(index); }

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
struct std::is_enum<roq::OrderManagement> : std::true_type {};

template <>
struct std::underlying_type<roq::OrderManagement> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::OrderManagement> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderManagement &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, value.name());
  }
};
