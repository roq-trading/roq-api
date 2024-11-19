/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <span>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Remove route(s)
struct ROQ_PUBLIC RemoveRoutes final {
  std::span<uint32_t const> strategy_ids;  //!< List of strategy_id's to remove
};

template <>
inline constexpr std::string_view get_name<RemoveRoutes>() {
  using namespace std::literals;
  return "remove_routes"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RemoveRoutes> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RemoveRoutes const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(strategy_ids=[{}])"
        R"(}})"sv,
        fmt::join(value.strategy_ids, ", "sv));
  }
};

template <>
struct fmt::formatter<roq::Event<roq::RemoveRoutes>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::RemoveRoutes> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(remove_routes={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::RemoveRoutes>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::RemoveRoutes> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(remove_routes={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
