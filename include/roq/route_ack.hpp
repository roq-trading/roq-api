/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/route.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Response to route request
struct ROQ_PUBLIC RouteAck final {
  std::span<Route> routes;  //!< Response per strategy_id
  std::string_view user;    //!< User name (optional, only relevant for drop-copy)
};

template <>
inline constexpr std::string_view get_name<RouteAck>() {
  using namespace std::literals;
  return "route_ack"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RouteAck> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RouteAck const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(routes=[{}], )"
        R"(user="{}")"
        R"(}})"_cf,
        fmt::join(value.routes, ", "sv),
        value.user);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::RouteAck>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::RouteAck> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(route_ack={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::RouteAck>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::RouteAck> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(route_ack={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
