/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Gateway settings
struct ROQ_PUBLIC GatewaySettings final {
  uint64_t supports = {};                       //!< Support types (bit mask)
  uint32_t mbp_max_depth = {};                  //!< MBP max depth
  bool mbp_allow_price_inversion = false;       //!< MBP allow price inversion?
  bool mbp_allow_fractional_tick_size = false;  //!< MBP allow fractional tick-size?
  bool mbp_allow_remove_non_existing = false;   //!< MBP allow remove operation on non-existing level?
  bool oms_download_has_state = false;          //!< OMS download includes state information?
  bool oms_download_has_routing_id = false;     //!< OMS download includes routing_id?
};

}  // namespace roq

template <>
struct fmt::formatter<roq::GatewaySettings> : public roq::formatter {
  template <typename Context>
  auto format(const roq::GatewaySettings &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(supports={:#x}, )"
        R"(mbp_max_depth={}, )"
        R"(mbp_allow_price_inversion={}, )"
        R"(mbp_allow_fractional_tick_size={}, )"
        R"(mbp_allow_remove_non_existing={}, )"
        R"(oms_download_has_state={}, )"
        R"(oms_download_has_routing_id={})"
        R"(}})"_sv,
        value.supports,
        value.mbp_max_depth,
        value.mbp_allow_price_inversion,
        value.mbp_allow_fractional_tick_size,
        value.mbp_allow_remove_non_existing,
        value.oms_download_has_state,
        value.oms_download_has_routing_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::GatewaySettings> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::GatewaySettings> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(gateway_settings={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
