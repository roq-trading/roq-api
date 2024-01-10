/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/route_request_status.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Route
struct ROQ_PUBLIC Route final {
  uint32_t strategy_id = {};       //!< Strategy identifier (optional)
  RouteRequestStatus status = {};  //!< Response to the request
};

template <>
inline constexpr std::string_view get_name<Route>() {
  using namespace std::literals;
  return "route"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Route> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Route const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(strategy_id={}, )"
        R"(status={})"
        R"(}})"sv,
        value.strategy_id,
        value.status);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Route>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::Route> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(route={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Route>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::Route> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(route={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
