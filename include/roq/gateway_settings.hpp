/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/event.hpp"
#include "roq/filter.hpp"
#include "roq/limits.hpp"
#include "roq/mask.hpp"
#include "roq/name.hpp"
#include "roq/request_id_type.hpp"
#include "roq/support_type.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Gateway settings
struct ROQ_PUBLIC GatewaySettings final {
  roq::Mask<roq::SupportType> supports;            //!< Supported update types
  uint16_t mbp_max_depth = {};                     //!< MBP max depth
  double mbp_tick_size_multiplier = roq::NaN;      //!< MBP multiplier used to manage prices as integer
  double mbp_min_trade_vol_multiplier = roq::NaN;  //!< MBP multiplier used to manage quantities as integer
  bool mbp_allow_remove_non_existing = false;      //!< MBP allow remove operation on non-existing level?
  bool mbp_allow_price_inversion = false;          //!< MBP allow price inversion?
  bool mbp_checksum = false;                       //!< MBP compute checksum?
  bool oms_download_has_state = false;             //!< OMS download includes state information?
  bool oms_download_has_routing_id = false;        //!< OMS download includes routing_id?
  roq::RequestIdType oms_request_id_type = {};     //!< OMS request identifier type
  roq::Mask<roq::Filter> oms_cancel_all_orders;    //!< Supported filters for CancelAllOrders
};

template <>
inline constexpr std::string_view get_name<GatewaySettings>() {
  using namespace std::literals;
  return "gateway_settings"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::GatewaySettings> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::GatewaySettings const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(supports={}, )"
        R"(mbp_max_depth={}, )"
        R"(mbp_tick_size_multiplier={}, )"
        R"(mbp_min_trade_vol_multiplier={}, )"
        R"(mbp_allow_remove_non_existing={}, )"
        R"(mbp_allow_price_inversion={}, )"
        R"(mbp_checksum={}, )"
        R"(oms_download_has_state={}, )"
        R"(oms_download_has_routing_id={}, )"
        R"(oms_request_id_type={}, )"
        R"(oms_cancel_all_orders={})"
        R"(}})"sv,
        value.supports,
        value.mbp_max_depth,
        value.mbp_tick_size_multiplier,
        value.mbp_min_trade_vol_multiplier,
        value.mbp_allow_remove_non_existing,
        value.mbp_allow_price_inversion,
        value.mbp_checksum,
        value.oms_download_has_state,
        value.oms_download_has_routing_id,
        value.oms_request_id_type,
        value.oms_cancel_all_orders);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::GatewaySettings>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::GatewaySettings> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(gateway_settings={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::GatewaySettings>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::GatewaySettings> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(gateway_settings={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
