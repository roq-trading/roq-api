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

namespace roq {

//! Signals a stop event
struct ROQ_PUBLIC Stop final {};

}  // namespace roq

template <>
struct fmt::formatter<roq::Stop> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Stop &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), R"({{}})"_sv);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Stop> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::Stop> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(stop={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
