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

namespace roq {

//! Identifies the beginning of a sequence of related messages
struct ROQ_PUBLIC BatchBegin final {
  BatchBegin() = default;
  BatchBegin(BatchBegin &&) = default;
  BatchBegin(const BatchBegin &) = delete;
};

}  // namespace roq

template <>
struct fmt::formatter<roq::BatchBegin> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::BatchBegin &value, Context &context) {
    return format_to(
        context.out(),

        R"({{}})");
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
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(batch_begin={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
