/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

#include "roq/gateway_status.h"

namespace roq {

//! Update relating to current status of market data connectivity
struct ROQ_PUBLIC MarketDataStatus final {
  MarketDataStatus() = default;
  MarketDataStatus(MarketDataStatus &&) = default;
  MarketDataStatus(const MarketDataStatus &) = delete;

  GatewayStatus status = GatewayStatus::UNDEFINED;  //!< Gateway status
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketDataStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::MarketDataStatus &value, Context &context) {
    return format_to(
        context.out(),

        R"({{)"
        R"(status={})"

        R"(}})",
        value.status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketDataStatus> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::MarketDataStatus> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_data_status={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
