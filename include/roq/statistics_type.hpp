/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {

//! Enumeration of a statistics type
enum class StatisticsType : uint8_t {
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

}  // namespace roq

template <>
struct fmt::formatter<roq::StatisticsType> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::StatisticsType const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
