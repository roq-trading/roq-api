/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

#include "roq/connection_status.h"

namespace roq {

//! Update relating to current status of gateway connectivity
struct ROQ_PUBLIC Connection final {
  Connection() = default;
  Connection(Connection &&) = default;
  Connection(const Connection &) = delete;

  ConnectionStatus status = ConnectionStatus::UNDEFINED;  //!< Connection status
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Connection> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Connection &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(status={})"
        R"(}})",
        value.status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Connection> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::Connection> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(connection={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
