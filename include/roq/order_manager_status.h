/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

#include "roq/gateway_status.h"

namespace roq {

//! Update relating to current status of order manager connectivity
struct ROQ_PUBLIC OrderManagerStatus final {
  OrderManagerStatus() = default;
  OrderManagerStatus(OrderManagerStatus &&) = default;
  OrderManagerStatus(const OrderManagerStatus &) = delete;

  std::string_view account;                         //!< Account name (as known to the gateway)
  GatewayStatus status = GatewayStatus::UNDEFINED;  //!< Gateway status
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderManagerStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::OrderManagerStatus &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(status={})"
        R"(}})"sv,
        value.account,
        value.status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::OrderManagerStatus> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::OrderManagerStatus> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_manager_status={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
