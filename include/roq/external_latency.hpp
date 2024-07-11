/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <chrono>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Update relating to external latency
struct ROQ_PUBLIC ExternalLatency final {
  uint16_t stream_id = {};                //!< Stream identifier
  std::string_view account;               //!< Account name
  std::chrono::nanoseconds latency = {};  //!< latency measurement (1-way)
};

template <>
inline constexpr std::string_view get_name<ExternalLatency>() {
  using namespace std::literals;
  return "external_latency"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ExternalLatency> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ExternalLatency const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(latency={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.latency);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::ExternalLatency>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::ExternalLatency> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(external_latency={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::ExternalLatency>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::ExternalLatency> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(external_latency={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
