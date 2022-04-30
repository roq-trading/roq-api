/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/span.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

#include "roq/order_cancel_policy.hpp"

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
  auto format(const roq::Disconnected &value, Context &context) {
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
struct fmt::formatter<roq::Event<roq::Disconnected> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::Disconnected> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(disconnected={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Disconnected const> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Trace<roq::Disconnected const> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(trace_info={}, )"
        R"(disconnected={})"
        R"(}})"sv,
        event.trace_info,
        event.value);
  }
};
