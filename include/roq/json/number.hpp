/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/numbers.hpp"

#include "roq/utils/number.hpp"

namespace roq {
namespace json {

struct Number final {
  explicit Number(double value) : number_{value, {}} {}
  Number(double value, Decimals decimals) : number_{value, decimals} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    if (!std::isnan(number_.value)) {
      // note! we quote numbers to preserve all decimals
      return fmt::format_to(context.out(), R"("{}")"_cf, number_);
    }
    return fmt::format_to(context.out(), "null"_cf);
  }

 private:
  utils::Number const number_;
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
