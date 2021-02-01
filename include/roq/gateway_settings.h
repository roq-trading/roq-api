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

namespace roq {

//! Gateway settings
struct ROQ_PUBLIC GatewaySettings final {
  GatewaySettings() = default;
  GatewaySettings(GatewaySettings &&) = default;
  GatewaySettings(const GatewaySettings &) = delete;

  uint32_t mbp_max_depth = 0;              //!< MBP max depth
  bool mbp_allow_price_inversion = false;  //!< MBP allow price inversion?
};

}  // namespace roq

template <>
struct fmt::formatter<roq::GatewaySettings> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::GatewaySettings &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(mbp_max_depth={}, )"
        R"(mbp_allow_price_inversion={})"
        R"(}})",
        value.mbp_max_depth,
        value.mbp_allow_price_inversion);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::GatewaySettings> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::GatewaySettings> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(gateway_settings={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
