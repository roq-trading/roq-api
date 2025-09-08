/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat/fmt.hpp"

#include <fmt/format.h>

#include <cstdint>
#include <cstdlib>

namespace roq {

//! Enumeration of filter fields
enum class Filter : uint64_t {  // NOLINT(performance-enum-size)
  UNDEFINED = 0,
  ACCOUNT = 0x1,      //!< By account
  EXCHANGE = 0x2,     //!< By exchange
  SYMBOL = 0x4,       //!< By symbol
  STRATEGY_ID = 0x8,  //!< By strategy_id
  SIDE = 0x10,        //!< By side
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Filter> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Filter const &value, format_context &context) const {
    using namespace std::literals;
    auto name = [&]() -> std::string_view {
      switch (value) {
        using enum roq::Filter;
        case UNDEFINED:
          return "UNDEFINED"sv;
        case ACCOUNT:
          return "ACCOUNT"sv;
        case EXCHANGE:
          return "EXCHANGE"sv;
        case SYMBOL:
          return "SYMBOL"sv;
        case STRATEGY_ID:
          return "STRATEGY_ID"sv;
        case SIDE:
          return "SIDE"sv;
      }
      std::abort();
    }();
    return fmt::format_to(context.out(), "{}"sv, name);
  }
};