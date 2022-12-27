/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/gateway_status.hpp"

#include "roq/json/date.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct GatewayStatus final {
  explicit GatewayStatus(roq::GatewayStatus const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("account":{},)"
        R"("supported":{},)"
        R"("available":{},)"
        R"("unavailable":{})"
        R"(}})"_cf,
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::GatewayStatus const &value, Context &context) const {
    return value.format_to(context);
  }
};
