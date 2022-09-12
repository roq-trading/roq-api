/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <range/v3/view.hpp>

#include "roq/custom_metrics_update.hpp"

#include "roq/json/measurement.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct CustomMetricsUpdate final {
  explicit CustomMetricsUpdate(roq::CustomMetricsUpdate const &value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
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
        fmt::join(ranges::views::transform(value_.measurements, [](auto const &v) { return Measurement(v); }), ","),
        String{value_.update_type});
  }

 private:
  roq::CustomMetricsUpdate const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::CustomMetricsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::CustomMetricsUpdate const &value, Context &context) const {
    return value.format_to(context);
  }
};
