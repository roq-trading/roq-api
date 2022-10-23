/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/statistics.hpp"

#include "roq/json/datetime.hpp"
#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Statistics final {
  explicit Statistics(roq::Statistics const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("type":{},)"
        R"("value":{},)"
        R"("begin_time_utc":{},)"
        R"("end_time_utc":{})"
        R"(}})"sv,
        String{value_.type},
        Number{value_.value},
        DateTime{value_.begin_time_utc},
        DateTime{value_.end_time_utc});
  }

 private:
  roq::Statistics const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Statistics> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Statistics const &value, Context &context) const {
    return value.format_to(context);
  }
};
