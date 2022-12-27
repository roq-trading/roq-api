/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <range/v3/view.hpp>

#include "roq/parameters_update.hpp"

#include "roq/json/parameter.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct ParametersUpdate final {
  explicit ParametersUpdate(roq::ParametersUpdate const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("parameters":{{{}}},)"
        R"("update_type":{},)"
        R"("user":{})"
        R"(}})"_cf,
        fmt::join(ranges::views::transform(value_.parameters, [](auto const &v) { return Parameter{v}; }), ","sv),
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::ParametersUpdate const &value, Context &context) const {
    return value.format_to(context);
  }
};
