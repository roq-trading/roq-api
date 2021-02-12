/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

namespace roq {

//! Identifies the end of a sequence of related messages
struct ROQ_PUBLIC BatchEnd final {
  BatchEnd() = default;
  BatchEnd(BatchEnd &&) = default;
  BatchEnd(const BatchEnd &) = delete;
};

}  // namespace roq

template <>
struct fmt::formatter<roq::BatchEnd> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::BatchEnd &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(context.out(), R"({{}})"sv);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::BatchEnd> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::BatchEnd> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(batch_end={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
