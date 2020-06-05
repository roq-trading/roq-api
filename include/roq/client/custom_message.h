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
  const void *message;
  size_t length;
};

struct ROQ_PUBLIC CustomMessageEvent final {
  const MessageInfo& message_info;
  const CustomMessage& custom_message;
};

}  // namespace client

template <>
inline client::CustomMessageEvent create_event(
    const MessageInfo& message_info,
    const client::CustomMessageEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .custom_message = event.custom_message,
  };
}

template <>
struct event_value_helper<client::CustomMessageEvent> final {
  using type = client::CustomMessage;
  explicit event_value_helper(const client::CustomMessageEvent& event)
      : _event(event) {
  }
  operator const type&() const {
    return _event.custom_message;
  }
 private:
  const client::CustomMessageEvent& _event;
};

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
struct fmt::formatter<roq::client::CustomMessageEvent> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::client::CustomMessageEvent& value,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_message={})"
        R"(}})",
        value.message_info,
        value.custom_message);
  }
};
