/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/order_cancel_policy.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Disconnected
struct ROQ_PUBLIC Disconnected final {
  roq::OrderCancelPolicy order_cancel_policy = {};  //!< Cancel orders on disconnect?
};

template <>
inline constexpr std::string_view get_name<Disconnected>() {
  using namespace std::literals;
  return "disconnected"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Disconnected> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Disconnected const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_cancel_policy={})"
        R"(}})"sv,
        value.order_cancel_policy);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Disconnected>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::Disconnected> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(disconnected={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Disconnected>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::Disconnected> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(disconnected={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
