/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/parameter.hpp"

#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Parameter final {
  explicit Parameter(roq::Parameter const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("label":{},)"
        R"("account":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("value":{})"
        R"(}})"sv,
        String{value_.label},
        String{value_.account},
        String{value_.exchange},
        String{value_.symbol},
        String{value_.value});
  }

 private:
  roq::Parameter const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Parameter> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::Parameter const &value, format_context &context) const { return value.format_to(context); }
};
