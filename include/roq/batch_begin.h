/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Identifies the beginning of a sequence of related messages
struct ROQ_PUBLIC BatchBegin final {};

}  // namespace roq

template <>
struct fmt::formatter<roq::BatchBegin> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::BatchBegin &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), R"({{}})"_sv);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::BatchBegin> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::BatchBegin> &event, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(batch_begin={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
