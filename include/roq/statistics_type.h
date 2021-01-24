/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"

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
    MAX = MARGIN_RATE
  };

  StatisticsType() = default;

  // cppcheck-suppress noExplicitConstructor
  inline StatisticsType(type_t type)  // NOLINT
      : type_(type) {}

  inline explicit StatisticsType(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    constexpr std::string_view UNDEFINED("UNDEFINED");
    constexpr std::string_view OPEN_PRICE("OPEN_PRICE");
    constexpr std::string_view SETTLEMENT_PRICE("SETTLEMENT_PRICE");
    constexpr std::string_view CLOSE_PRICE("CLOSE_PRICE");
    constexpr std::string_view OPEN_INTEREST("OPEN_INTEREST");
    constexpr std::string_view PRE_OPEN_INTEREST("PRE_OPEN_INTEREST");
    constexpr std::string_view PRE_SETTLEMENT_PRICE("PRE_SETTLEMENT_PRICE");
    constexpr std::string_view PRE_CLOSE_PRICE("PRE_CLOSE_PRICE");
    constexpr std::string_view HIGHEST_TRADED_PRICE("HIGHEST_TRADED_PRICE");
    constexpr std::string_view LOWEST_TRADED_PRICE("LOWEST_TRADED_PRICE");
    constexpr std::string_view UPPER_LIMIT_PRICE("UPPER_LIMIT_PRICE");
    constexpr std::string_view LOWER_LIMIT_PRICE("LOWER_LIMIT_PRICE");
    constexpr std::string_view INDEX_VALUE("INDEX_VALUE");
    constexpr std::string_view MARGIN_RATE("MARGIN_RATE");
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::OPEN_PRICE:
        return OPEN_PRICE;
      case type_t::SETTLEMENT_PRICE:
        return SETTLEMENT_PRICE;
      case type_t::CLOSE_PRICE:
        return CLOSE_PRICE;
      case type_t::OPEN_INTEREST:
        return OPEN_INTEREST;
      case type_t::PRE_OPEN_INTEREST:
        return PRE_OPEN_INTEREST;
      case type_t::PRE_SETTLEMENT_PRICE:
        return PRE_SETTLEMENT_PRICE;
      case type_t::PRE_CLOSE_PRICE:
        return PRE_CLOSE_PRICE;
      case type_t::HIGHEST_TRADED_PRICE:
        return HIGHEST_TRADED_PRICE;
      case type_t::LOWEST_TRADED_PRICE:
        return LOWEST_TRADED_PRICE;
      case type_t::UPPER_LIMIT_PRICE:
        return UPPER_LIMIT_PRICE;
      case type_t::LOWER_LIMIT_PRICE:
        return LOWER_LIMIT_PRICE;
      case type_t::INDEX_VALUE:
        return INDEX_VALUE;
      case type_t::MARGIN_RATE:
        return MARGIN_RATE;
      default:
        assert(false);
    }
    return UNDEFINED;
  }

  inline operator std::string_view() const { return name(); }

 protected:
  inline type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::OPEN_PRICE:
      case type_t::SETTLEMENT_PRICE:
      case type_t::CLOSE_PRICE:
      case type_t::OPEN_INTEREST:
      case type_t::PRE_OPEN_INTEREST:
      case type_t::PRE_SETTLEMENT_PRICE:
      case type_t::PRE_CLOSE_PRICE:
      case type_t::HIGHEST_TRADED_PRICE:
      case type_t::LOWEST_TRADED_PRICE:
      case type_t::UPPER_LIMIT_PRICE:
      case type_t::LOWER_LIMIT_PRICE:
      case type_t::INDEX_VALUE:
      case type_t::MARGIN_RATE:
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
struct std::is_enum<roq::StatisticsType> : std::true_type {};

template <>
struct std::underlying_type<roq::StatisticsType> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::StatisticsType> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::StatisticsType &value, Context &context) {
    return format_to(context.out(), "{}", value.name());
  }
};
