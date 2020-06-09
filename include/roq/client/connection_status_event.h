/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>

#include "roq/compat.h"
#include "roq/event.h"

#include "roq/message_info.h"

#include "roq/connection_status.h"

namespace roq {
namespace client {

struct ROQ_PUBLIC ConnectionStatusEvent final {
  const MessageInfo& message_info;
  ConnectionStatus connection_status;
};

}  // namespace client

template <>
struct event_value_helper<client::ConnectionStatusEvent> final {
  using type = ConnectionStatus;
  explicit event_value_helper(const client::ConnectionStatusEvent& event)
      : _event(event) {
  }
  operator const type&() const {
    return _event.connection_status;
  }

 private:
  const client::ConnectionStatusEvent& _event;
};
}  // namespace roq

template <>
struct fmt::formatter<roq::client::ConnectionStatusEvent> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::client::ConnectionStatusEvent& value,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(connection_status={})"
        R"(}})",
        value.message_info,
        value.connection_status);
  }
};
