/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

namespace roq {
namespace json {

template <typename T>
struct String final {
  explicit String(T const &value) : value_{value} {}

  auto format_to(auto &context) const {
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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::String<T> const &value, format_context &context) const { return value.format_to(context); }
};
