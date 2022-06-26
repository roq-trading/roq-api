/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <range/v3/view.hpp>

#include "roq/statistics_update.hpp"

#include "roq/json/statistics.hpp"

#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct StatisticsUpdate final {
  explicit StatisticsUpdate(roq::StatisticsUpdate const &value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("statistics":[{}],)"
        R"("update_type":{},)"
        R"("exchange_time_utc":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        fmt::join(ranges::views::transform(value_.statistics, [](auto const &v) { return Statistics(v); }), ","),
        String{value_.update_type},
        DateTime{value_.exchange_time_utc});
  }

 private:
  roq::StatisticsUpdate const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::StatisticsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::StatisticsUpdate const &value, Context &context) {
    return value.format_to(context);
  }
};
