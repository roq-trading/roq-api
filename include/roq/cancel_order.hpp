/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Fields required to cancel an existing order
struct ROQ_PUBLIC CancelOrder final {
  std::string_view account;              //!< Account name
  uint64_t order_id = {};                //!< Order identifier
  std::string_view request_template;     //!< Request template (gateway configured)
  std::string_view routing_id;           //!< Routing identifier
  uint32_t version = {};                 //!< Version number (strictly increasing, optional)
  uint32_t conditional_on_version = {};  //!< Auto-reject if this version has positively failed (optional)
};

template <>
inline constexpr std::string_view get_name<CancelOrder>() {
  using namespace std::literals;
  return "cancel_order"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelOrder> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CancelOrder const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(request_template="{}", )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(conditional_on_version={})"
        R"(}})"sv,
        value.account,
        value.order_id,
        value.request_template,
        value.routing_id,
        value.version,
        value.conditional_on_version);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::CancelOrder>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::CancelOrder> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_order={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CancelOrder>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::CancelOrder> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_order={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
