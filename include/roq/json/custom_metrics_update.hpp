/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <ranges>

#include "roq/custom_metrics_update.hpp"

#include "roq/json/measurement.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct CustomMetricsUpdate final {
  explicit CustomMetricsUpdate(roq::CustomMetricsUpdate const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("user":{},)"
        R"("label":{},)"
        R"("account":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("measurements":[{}],)"
        R"("update_type":{})"
        R"(}})"sv,
        String{value_.user},
        String{value_.label},
        String{value_.account},
        String{value_.exchange},
        String{value_.symbol},
        fmt::join(std::ranges::views::transform(value_.measurements, [](auto &v) { return Measurement{v}; }), ","sv),
        String{value_.update_type});
  }

 private:
  roq::CustomMetricsUpdate const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::CustomMetricsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::CustomMetricsUpdate const &value, format_context &context) const {
    return value.format_to(context);
  }
};
