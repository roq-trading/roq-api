/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/mbp_update.hpp"

#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Measurement final {
  explicit Measurement(roq::Measurement const &value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("name":"{}",)"
        R"("value":{})"
        R"(}})"sv,
        String{value_.name},
        Number{value_.value});
  }

 private:
  roq::Measurement const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Measurement> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Measurement const &value, Context &context) const {
    return value.format_to(context);
  }
};
