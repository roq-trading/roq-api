/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/gateway_status.hpp"

#include "roq/json/date.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/decimal.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct GatewayStatus final {
  explicit GatewayStatus(roq::GatewayStatus const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("account":{},)"
        R"("supported":{},)"
        R"("available":{},)"
        R"("unavailable":{})"
        R"(}})"sv,
        String{value_.account},
        String{value_.supported},
        String{value_.available},
        String{value_.unavailable});
  }

 private:
  roq::GatewayStatus const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::GatewayStatus> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::GatewayStatus const &value, format_context &context) const { return value.format_to(context); }
};
