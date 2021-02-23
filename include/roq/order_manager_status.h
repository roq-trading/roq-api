/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"

#include "roq/gateway_status.h"

namespace roq {

//! Update relating to current status of order manager connectivity
struct ROQ_PUBLIC OrderManagerStatus final {
  std::string_view account;                         //!< Account name (as known to the gateway)
  GatewayStatus status = GatewayStatus::UNDEFINED;  //!< Gateway status
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderManagerStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderManagerStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(status={})"
        R"(}})"_fmt,
        value.account,
        value.status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::OrderManagerStatus> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::OrderManagerStatus> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_manager_status={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
