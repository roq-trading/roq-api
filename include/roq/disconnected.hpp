/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/order_cancel_policy.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Disconnected
struct ROQ_PUBLIC Disconnected final {
  OrderCancelPolicy order_cancel_policy = {};  //!< Cancel orders on disconnect?
};

template <>
inline constexpr std::string_view get_name<Disconnected>() {
  using namespace std::literals;
  return "disconnected"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Disconnected> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Disconnected const &value, Context &context) const {
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::Disconnected> const &event, Context &context) const {
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::Disconnected> const &event, Context &context) const {
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
