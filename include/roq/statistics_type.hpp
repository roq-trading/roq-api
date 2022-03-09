/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include <magic_enum.hpp>

#include "roq/compat.hpp"

namespace roq {

//! Enumeration of a statistics type
struct ROQ_PACKED StatisticsType final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    OPEN_PRICE,
    SETTLEMENT_PRICE,
    CLOSE_PRICE,
    OPEN_INTEREST,
    PRE_OPEN_INTEREST,
    PRE_SETTLEMENT_PRICE,
    PRE_CLOSE_PRICE,
    HIGHEST_TRADED_PRICE,
    LOWEST_TRADED_PRICE,
    UPPER_LIMIT_PRICE,
    LOWER_LIMIT_PRICE,
    INDEX_VALUE,
    MARGIN_RATE,
    FUNDING_RATE,
    FUNDING_RATE_PREDICTION,
    TRADE_VOLUME,
  };

  constexpr StatisticsType() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr StatisticsType(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr StatisticsType(uint8_t value)
      : type_(magic_enum::enum_cast<type_t>(value).value_or(type_t::UNDEFINED)) {}

  explicit constexpr StatisticsType(const std::string_view &value)
      : type_(magic_enum::enum_cast<type_t>(value).value_or(type_t::UNDEFINED)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const { return magic_enum::enum_name(type_); }

  constexpr operator std::string_view() const { return name(); }

  static constexpr auto values() { return magic_enum::enum_values<type_t>(); }

  static constexpr size_t count() { return magic_enum::enum_count<type_t>(); }

  static constexpr StatisticsType from_index(size_t index) { return magic_enum::enum_value<type_t>(index); }

  constexpr size_t to_index() const {
    auto result = magic_enum::enum_index(type_);  // std::optional
    return result.value();                        // note! could throw
  }

 private:
  type_t type_ = type_t::UNDEFINED;
};

}  // namespace roq

template <>
struct std::is_enum<roq::StatisticsType> : std::true_type {};

template <>
struct std::underlying_type<roq::StatisticsType> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::StatisticsType> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::StatisticsType &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, value.name());
  }
};
