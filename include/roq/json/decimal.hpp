/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/decimal.hpp"

namespace roq {
namespace json {

struct Decimal final {
  using value_type = roq::Decimal;

  explicit Decimal(double value) : value_{value, {}} {}

  explicit Decimal(roq::Decimal const &number) : value_{number} {}

  Decimal(double value, Precision precision) : value_{value, precision} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    if (!std::isnan(value_.value)) {
      // note! we quote numbers to preserve all decimals
      return fmt::format_to(context.out(), R"("{}")"sv, value_);
    }
    return fmt::format_to(context.out(), "null"sv);
  }

 private:
  value_type const value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Decimal> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::Decimal const &value, format_context &context) const { return value.format_to(context); }
};
