/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <cassert>

namespace roq {

//! Enumeration of filter fields
enum class Filter : uint64_t {
  UNDEFINED = 0x0,
  ACCOUNT = 0x1,      //!< By account
  EXCHANGE = 0x2,     //!< By exchange
  SYMBOL = 0x4,       //!< By symbol
  STRATEGY_ID = 0x8,  //!< By strategy_id
  SIDE = 0x10,        //!< By side
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Filter> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Filter const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    auto name{[&]() {
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
        default:
          assert(false);
      }
      return "<UNKNOWN>"sv;
    }()};
    return fmt::format_to(context.out(), "{}"_cf, name);
  }
};
