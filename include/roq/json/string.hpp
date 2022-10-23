/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

namespace roq {
namespace json {

template <typename T>
struct String final {
  explicit String(T const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    if constexpr (std::is_enum<T>::value) {
      if (value_ != T{})
        return fmt::format_to(context.out(), R"("{}")"sv, value_);
    } else {
      if (!std::empty(value_))
        return fmt::format_to(context.out(), R"("{}")"sv, value_);
    }
    return fmt::format_to(context.out(), "null"sv);
  }

 private:
  T const &value_;
};

}  // namespace json
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::json::String<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::String<T> const &value, Context &context) const {
    return value.format_to(context);
  }
};
