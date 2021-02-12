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

//! Update relating to external latency
struct ROQ_PUBLIC ExternalLatency final {
  ExternalLatency() = default;
  ExternalLatency(ExternalLatency &&) = default;
  ExternalLatency(const ExternalLatency &) = delete;

  std::string_view name;                  //!< connection name
  std::chrono::nanoseconds latency = {};  //!< latency measurement (1-way)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ExternalLatency> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::ExternalLatency &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(name="{}", )"
        R"(latency={})"
        R"(}})"sv,
        value.name,
        value.latency);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::ExternalLatency> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::ExternalLatency> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(external_latency={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
