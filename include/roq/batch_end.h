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

//! Identifies the end of a sequence of related messages
struct ROQ_PUBLIC BatchEnd final {};

}  // namespace roq

template <>
struct fmt::formatter<roq::BatchEnd> : public roq::formatter {
  template <typename Context>
  auto format(const roq::BatchEnd &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), R"({{}})"_sv);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::BatchEnd> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::BatchEnd> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(batch_end={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
