/* Copyright (c) 2017-2020, Hans Erik Thrane */
#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/event.h"
#include "roq/span.h"
#include "roq/string.h"

#include "roq/message_info.h"

#include "roq/execution_instruction.h"
#include "roq/order_type.h"
#include "roq/position_effect.h"
#include "roq/side.h"
#include "roq/time_in_force.h"

namespace roq {
namespace client {

struct ROQ_PUBLIC CustomMessage final {
  CustomMessage() = default;
  CustomMessage(CustomMessage&&) = default;
  CustomMessage(const CustomMessage&) = delete;

  const void *message;
  size_t length;
};

}  // namespace client

}  // namespace roq

template <>
struct fmt::formatter<roq::client::CustomMessage> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::client::CustomMessage& value,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(length={})"
        R"(}})",
        value.length);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::client::CustomMessage> > {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::Event<roq::client::CustomMessage>& event,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_message={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
