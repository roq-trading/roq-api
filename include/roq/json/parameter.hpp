/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/parameter.hpp"

#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Parameter final {
  explicit Parameter(roq::Parameter const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("label":{},)"
        R"("account":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("value":{})"
        R"(}})"_cf,
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Parameter const &value, Context &context) const {
    return value.format_to(context);
  }
};
