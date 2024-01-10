/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <algorithm>
#include <ranges>

#include "roq/statistics_update.hpp"

#include "roq/cache/statistics.hpp"

#include "roq/json/statistics.hpp"

#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct StatisticsUpdate final {
  StatisticsUpdate(Context const &context, roq::StatisticsUpdate const &value) : context_{context}, value_{value} {}
  StatisticsUpdate(Context const &context, roq::StatisticsUpdate const &value, cache::Statistics const &cache)
      : context_{context}, value_{value}, cache_{&cache} {}

  auto format_to(auto &context) const {
    if (cache_) {
      std::span statistics{std::data((*cache_).statistics), std::size((*cache_).statistics)};
      return helper(context, statistics, UpdateType::SNAPSHOT);
    } else {
      return helper(context, value_.statistics, value_.update_type);
    }
  }

 protected:
  auto helper(auto &context, std::span<roq::Statistics const> const &statistics, auto update_type) const {
    using namespace std::literals;
    // FIXME this workaround shouldn't be necessary...
    std::span tmp{const_cast<roq::Statistics *>(std::data(statistics)), std::size(statistics)};
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
        fmt::join(
            std::ranges::views::transform(
                std::ranges::remove_if(tmp, [](auto const &v) { return !std::isfinite(v.value); }),
                [this](auto const &v) {
                  return Statistics{context_, v};
                }),
            ","sv),
        String{update_type},
        DateTime{value_.exchange_time_utc});
  }

 private:
  Context const &context_;
  roq::StatisticsUpdate const &value_;
  cache::Statistics const *const cache_ = nullptr;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::StatisticsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::StatisticsUpdate const &value, format_context &context) const {
    return value.format_to(context);
  }
};
