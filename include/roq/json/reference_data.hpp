/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/reference_data.hpp"

#include "roq/json/date.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct ReferenceData final {
  explicit ReferenceData(roq::ReferenceData const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("description":{},)"
        R"("security_type":{},)"
        R"("base_currency":{},)"
        R"("quote_currency":{},)"
        R"("margin_currency":{},)"
        R"("commission_currency":{},)"
        R"("tick_size":{},)"
        R"("multiplier":{},)"
        R"("min_notional":{},)"
        R"("min_trade_vol":{},)"
        R"("max_trade_vol":{},)"
        R"("trade_vol_step_size":{},)"
        R"("option_type":{},)"
        R"("strike_currency":{},)"
        R"("strike_price":{},)"
        R"("underlying":{},)"
        R"("time_zone":{},)"
        R"("issue_date":{},)"
        R"("settlement_date":{},)"
        R"("expiry_datetime":{},)"
        R"("expiry_datetime_utc":{},)"
        R"("discard":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        String{value_.description},
        String{value_.security_type},
        String{value_.base_currency},
        String{value_.quote_currency},
        String{value_.margin_currency},
        String{value_.commission_currency},
        Number{value_.tick_size},
        Number{value_.multiplier},
        Number{value_.min_notional},
        Number{value_.min_trade_vol},
        Number{value_.max_trade_vol},
        Number{value_.trade_vol_step_size},
        String{value_.option_type},
        String{value_.strike_currency},
        Number{value_.strike_price},
        String{value_.underlying},
        String{value_.time_zone},
        Date{value_.issue_date},
        Date{value_.settlement_date},
        DateTime{value_.expiry_datetime},  // XXX wrong: using UTC
        DateTime{value_.expiry_datetime_utc},
        value_.discard);
  }

 private:
  roq::ReferenceData const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::ReferenceData> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::ReferenceData const &value, Context &context) const {
    return value.format_to(context);
  }
};
