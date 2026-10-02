/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <cstdlib>

namespace roq {

//! Enumeration of trade conditions
enum class TradeCondition : uint32_t {  // NOLINT(performance-enum-size)
  UNDEFINED = 0,
  IMPLIED_MATCHING = 0x1,          //!< Matching implied from spread markets
  FORCED_LIQUIDATION = 0x2,        //!< Forced order liquidation
  RETAIL_PRICE_IMPROVEMENT = 0x4,  //!< Retail Price Improvement
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TradeCondition> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TradeCondition const &value, format_context &context) const {
    using namespace std::literals;
    auto name = [&]() -> std::string_view {
      switch (value) {
        using enum roq::TradeCondition;
        case UNDEFINED:
          return "UNDEFINED"sv;
        case IMPLIED_MATCHING:
          return "IMPLIED_MATCHING"sv;
        case FORCED_LIQUIDATION:
          return "FORCED_LIQUIDATION"sv;
        case RETAIL_PRICE_IMPROVEMENT:
          return "RETAIL_PRICE_IMPROVEMENT"sv;
      }
      std::abort();
    }();
    return fmt::format_to(context.out(), "{}"sv, name);
  }
};