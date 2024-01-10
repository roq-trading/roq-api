/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/utils/date.hpp"

namespace roq {
namespace json {

template <typename T>
struct Date final {
  explicit Date(T value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    if (value_.count())
      return fmt::format_to(context.out(), R"("{}")"sv, utils::Date_iso8601{value_});
    return fmt::format_to(context.out(), "null"sv);
  }

 private:
  T const value_;
};

}  // namespace json
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::json::Date<T>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::Date<T> const &value, format_context &context) const { return value.format_to(context); }
};
