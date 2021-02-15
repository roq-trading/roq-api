/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/span.h"

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
struct fmt::formatter<roq::MarketDataStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::MarketDataStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(status={})"
        R"(}})"_fmt,
        value.status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketDataStatus> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::MarketDataStatus> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_data_status={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
