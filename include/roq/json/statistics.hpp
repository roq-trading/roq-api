/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/statistics.hpp"

#include "roq/utils/common.hpp"

#include "roq/json/context.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Statistics final {
  Statistics(Context const &context, roq::Statistics const &value) : context_{context}, value_{value} {}

  auto format_to(auto &context) const {
    using namespace fmt::literals;
    auto decimals = [&]() -> Decimals {
      if (utils::is_price(value_.type))
        return context_.price_decimals;
      if (utils::is_quantity(value_.type))
        return context_.quantity_decimals;
      return {};
    }();
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("type":{},)"
        R"("value":{},)"
        R"("begin_time_utc":{},)"
        R"("end_time_utc":{})"
        R"(}})"_cf,
        String{value_.type},
        Number{value_.value, decimals},
        DateTime{value_.begin_time_utc},
        DateTime{value_.end_time_utc});
  }

 private:
  Context const &context_;
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
