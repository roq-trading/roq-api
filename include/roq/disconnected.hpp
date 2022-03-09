/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string_buffer.hpp"

#include "roq/order_cancel_policy.hpp"

namespace roq {

//! Disconnected
struct ROQ_PUBLIC Disconnected final {
  OrderCancelPolicy order_cancel_policy = {};  //!< Cancel orders on disconnect?
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Disconnected> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Disconnected &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(order_cancel_policy={})"
        R"(}})"sv,
        value.order_cancel_policy);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Disconnected> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::Disconnected> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(disconnected={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
