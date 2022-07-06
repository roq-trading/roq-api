/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/numbers.hpp"

namespace roq {
namespace json {

struct Number final {
  explicit Number(double value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    if (!std::isnan(value_))
      return fmt::format_to(context.out(), "{}"sv, value_);
    return fmt::format_to(context.out(), "null"sv);
  }

 private:
  double const value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Number> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Number const &value, Context &context) const {
    return value.format_to(context);
  }
};
