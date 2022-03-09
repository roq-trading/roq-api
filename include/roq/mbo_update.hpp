/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/numbers.hpp"
#include "roq/string_buffer.hpp"
#include "roq/uuid.hpp"

#include "roq/order_update_action.hpp"

namespace roq {

//! Represents the update status of a single order in the order book
struct ROQ_PUBLIC MBOUpdate final {
  double price = NaN;               //!< Order price
  double remaining_quantity = {};   //!< Remaining order quantity
  OrderUpdateAction action = {};    //!< Order update action
  uint32_t priority = {};           //!< Queue priority
  roq::string_buffer<40> order_id;  //!< Order identifier
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBOUpdate> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::MBOUpdate &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(price={}, )"
        R"(remaining_quantity={}, )"
        R"(action={}, )"
        R"(priority={}, )"
        R"(order_id="{}")"
        R"(}})"sv,
        value.price,
        value.remaining_quantity,
        value.action,
        value.priority,
        value.order_id);
  }
};