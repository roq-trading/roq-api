/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/cancel_policy.h"

namespace roq {

//! Disconnected
struct ROQ_PUBLIC Disconnected final {
  CancelPolicy cancel_policy = {};  //!< Cancel orders on disconnect?
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Disconnected> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Disconnected &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(cancel_policy={})"
        R"(}})"_fmt,
        value.cancel_policy);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Disconnected> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::Disconnected> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(disconnected={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
