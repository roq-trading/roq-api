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

//! Fields required to cancel an existing order
struct ROQ_PUBLIC CancelOrder final {
  std::string_view account;              //!< Account name
  uint32_t order_id = {};                //!< Order identifier
  std::string_view routing_id;           //!< Routing identifier
  uint32_t version = {};                 //!< Version number (strictly increasing, optional)
  uint32_t conditional_on_version = {};  //!< Auto-reject if this version has positively failed (optional)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelOrder> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::CancelOrder &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(conditional_on_version={})"
        R"(}})"sv,
        value.account,
        value.order_id,
        value.routing_id,
        value.version,
        value.conditional_on_version);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CancelOrder> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::CancelOrder> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(cancel_order={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
