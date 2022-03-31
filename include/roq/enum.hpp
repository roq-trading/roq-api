/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <string_view>
#include <type_traits>

#include <magic_enum.hpp>

#include "roq/compat.hpp"

namespace roq {

// +++ EXPERIMENTAL DO NOT USE +++
//
// The idea is to have a single template class useful for all enums
// However, current approach requires a derived class and may cause internal
// compiler error (using gcc 11.2)

// note!
// this class must be derived
// better would be "using MyEnum = Enum<T>", but this is not possible due to
// https://stackoverflow.com/a/67000446

template <typename T>
struct ROQ_PACKED Enum {
  using type_t = T;
  using value_t = typename std::underlying_type<T>::type;

  constexpr Enum() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr Enum(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  constexpr explicit Enum(value_t value) : type_(magic_enum::enum_cast<type_t>(value).value_or(type_t::UNDEFINED)) {}

  constexpr explicit Enum(const std::string_view &value)
      : type_(magic_enum::enum_cast<type_t>(value).value_or(type_t::UNDEFINED)) {}

  /*
  constexpr Enum &operator=(type_t type) {
    type_ = type;
    return *this;
  }
  */

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const { return magic_enum::enum_name(type_); }

  constexpr operator std::string_view() const { return name(); }

  static constexpr auto values() { return magic_enum::enum_values<type_t>(); }

  static constexpr size_t count() { return magic_enum::enum_count<type_t>(); }

  static constexpr Enum from_index(size_t index) { return magic_enum::enum_value<type_t>(index); }

  constexpr size_t to_index() const {
    auto result = magic_enum::enum_index(type_);  // std::optional
    return result.value();                        // note! could throw
  }

 protected:
  using base_t = Enum;
  type_t type_ = type_t::UNDEFINED;
};

}  // namespace roq

template <typename T>
struct std::is_enum<roq::Enum<T>> : std::true_type {};

template <typename T>
struct std::underlying_type<roq::Enum<T>> {
  using type = typename roq::Enum<T>::type_t;
};

template <typename T>
struct fmt::formatter<roq::Enum<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Enum<T> &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, value.name());
  }
};
