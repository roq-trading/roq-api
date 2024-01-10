/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/measurement.hpp"

#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Measurement final {
  explicit Measurement(roq::Measurement const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("name":{},)"
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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::Measurement const &value, format_context &context) const { return value.format_to(context); }
};
