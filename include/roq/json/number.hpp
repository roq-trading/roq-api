/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/numbers.hpp"

#include "roq/utils/number.hpp"

namespace roq {
namespace json {

struct Number final {
  explicit Number(double value) : number_{value, {}} {}
  explicit Number(utils::Number const &number) : number_{number} {}
  Number(double value, Decimals decimals) : number_{value, decimals} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    if (!std::isnan(number_.value)) {
      // note! we quote numbers to preserve all decimals
      return fmt::format_to(context.out(), R"("{}")"sv, number_);
    }
    return fmt::format_to(context.out(), "null"sv);
  }

 private:
  utils::Number const number_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Number> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::Number const &value, format_context &context) const { return value.format_to(context); }
};
