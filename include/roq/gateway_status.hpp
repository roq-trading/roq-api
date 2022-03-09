/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string_buffer.hpp"

namespace roq {

//! Update relating to current gateway service status
struct ROQ_PUBLIC GatewayStatus final {
  std::string_view account;   //!< Account name
  uint64_t supported = {};    //!< Supported update types (bit mask)
  uint64_t available = {};    //!< Update types (bit mask) available from all streams
  uint64_t unavailable = {};  //!< Update types (bit mask) unavailable from one or more streams
};

}  // namespace roq

template <>
struct fmt::formatter<roq::GatewayStatus> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::GatewayStatus &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(account="{}", )"
        R"(supported={:#x}, )"
        R"(available={:#x}, )"
        R"(unavailable={:#x})"
        R"(}})"sv,
        value.account,
        value.supported,
        value.available,
        value.unavailable);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::GatewayStatus> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::GatewayStatus> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(gateway_status={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
