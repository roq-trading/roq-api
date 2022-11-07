/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/parameter.hpp"

#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Parameter final {
  explicit Parameter(roq::Parameter const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}:{}"sv, String{value_.key}, String{value_.value});
  }

 private:
  roq::Parameter const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Parameter> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Parameter const &value, Context &context) const {
    return value.format_to(context);
  }
};
