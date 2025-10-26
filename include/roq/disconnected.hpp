/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/order_cancel_policy.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Disconnected
struct ROQ_PUBLIC Disconnected final {
  roq::OrderCancelPolicy order_cancel_policy = {};  //!< Cancel orders on disconnect?
};

template <>
constexpr std::string_view get_name<Disconnected>() {
  using namespace std::literals;
  return "disconnected"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Disconnected> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Disconnected const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_cancel_policy={})"
        R"(}})"sv,
        value.order_cancel_policy);
  }
};