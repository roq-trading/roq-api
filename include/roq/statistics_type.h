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
    DAILY_FUNDING_RATE,
  };

  constexpr StatisticsType() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr StatisticsType(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr StatisticsType(uint8_t type) : type_(validate(type)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::OPEN_PRICE:
        return "OPEN_PRICE"_sv;
      case type_t::SETTLEMENT_PRICE:
        return "SETTLEMENT_PRICE"_sv;
      case type_t::CLOSE_PRICE:
        return "CLOSE_PRICE"_sv;
      case type_t::OPEN_INTEREST:
        return "OPEN_INTEREST"_sv;
      case type_t::PRE_OPEN_INTEREST:
        return "PRE_OPEN_INTEREST"_sv;
      case type_t::PRE_SETTLEMENT_PRICE:
        return "PRE_SETTLEMENT_PRICE"_sv;
      case type_t::PRE_CLOSE_PRICE:
        return "PRE_CLOSE_PRICE"_sv;
      case type_t::HIGHEST_TRADED_PRICE:
        return "HIGHEST_TRADED_PRICE"_sv;
      case type_t::LOWEST_TRADED_PRICE:
        return "LOWEST_TRADED_PRICE"_sv;
      case type_t::UPPER_LIMIT_PRICE:
        return "UPPER_LIMIT_PRICE"_sv;
      case type_t::LOWER_LIMIT_PRICE:
        return "LOWER_LIMIT_PRICE"_sv;
      case type_t::INDEX_VALUE:
        return "INDEX_VALUE"_sv;
      case type_t::MARGIN_RATE:
        return "MARGIN_RATE"_sv;
      case type_t::FUNDING_RATE:
        return "FUNDING_RATE"_sv;
      case type_t::DAILY_FUNDING_RATE:
        return "DAILY_FUNDING_RATE"_sv;
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
      case type_t::FUNDING_RATE:
      case type_t::DAILY_FUNDING_RATE:
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
struct fmt::formatter<roq::StatisticsType> : public roq::formatter {
  template <typename Context>
  auto format(const roq::StatisticsType &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
