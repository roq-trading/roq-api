/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/utils/datetime.hpp"

namespace roq {
namespace json {

template <typename T>
struct DateTime final {
  explicit DateTime(T value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    if (value_.count())
      return fmt::format_to(context.out(), "{}"sv, utils::DateTime_iso8601{value_});
    return fmt::format_to(context.out(), "null"sv);
  }

 private:
  const T value_;
};

}  // namespace json
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::json::DateTime<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::DateTime<T> const &value, Context &context) {
    return value.format_to(context);
  }
};
