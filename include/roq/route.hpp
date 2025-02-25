/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/route_request_status.hpp"

namespace roq {

//! Route
struct ROQ_PUBLIC Route final {
  uint32_t strategy_id = {};            //!< Strategy identifier (optional)
  roq::RouteRequestStatus status = {};  //!< Response to the request
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Route> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Route const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(strategy_id={}, )"
        R"(status={})"
        R"(}})"sv,
        value.strategy_id,
        value.status);
  }
};
