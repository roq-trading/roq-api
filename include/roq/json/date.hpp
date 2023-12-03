/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/utils/date.hpp"

namespace roq {
namespace json {

template <typename T>
struct Date final {
  explicit Date(T value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace fmt::literals;
    if (value_.count())
      return fmt::format_to(context.out(), R"("{}")"_cf, utils::Date_iso8601{value_});
    return fmt::format_to(context.out(), "null"_cf);
  }

 private:
  T const value_;
};

}  // namespace json
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::json::Date<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Date<T> const &value, Context &context) const {
    return value.format_to(context);
  }
};
