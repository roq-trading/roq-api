/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <ranges>

#include "roq/parameters_update.hpp"

#include "roq/json/parameter.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct ParametersUpdate final {
  explicit ParametersUpdate(roq::ParametersUpdate const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("parameters":{{{}}},)"
        R"("update_type":{},)"
        R"("user":{})"
        R"(}})"sv,
        fmt::join(std::ranges::views::transform(value_.parameters, [](auto &v) { return Parameter{v}; }), ","sv),
        String{value_.update_type},
        String{value_.user});
  }

 private:
  roq::ParametersUpdate const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::ParametersUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::ParametersUpdate const &value, format_context &context) const {
    return value.format_to(context);
  }
};
