/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Fields required to modify an existing order
struct ROQ_PUBLIC ModifyOrder final {
  std::string_view account;              //!< Account name
  uint32_t order_id = {};                //!< Order identifier
  double quantity = NaN;                 //!< New (total) quantity
  double price = NaN;                    //!< New limit price
  std::string_view routing_id;           //!< Routing identifier
  uint32_t version = {};                 //!< Version number (strictly increasing, optional)
  uint32_t conditional_on_version = {};  //!< Auto-reject if this version has positively failed (optional)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ModifyOrder> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::ModifyOrder &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(conditional_on_version={})"
        R"(}})"sv,
        value.account,
        value.order_id,
        value.quantity,
        value.price,
        value.routing_id,
        value.version,
        value.conditional_on_version);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::ModifyOrder> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::ModifyOrder> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(modify_order={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
