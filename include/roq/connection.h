/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"

#include "roq/connection_status.h"

namespace roq {

//! Update relating to current status of gateway connectivity
struct ROQ_PUBLIC Connection final {
  ConnectionStatus status = ConnectionStatus::UNDEFINED;  //!< Connection status
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Connection> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Connection &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(status={})"
        R"(}})"_fmt,
        value.status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Connection> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::Connection> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(connection={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
