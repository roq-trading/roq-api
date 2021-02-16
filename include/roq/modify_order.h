/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"

namespace roq {

//! Fields required to modify an existing order
struct ROQ_PUBLIC ModifyOrder final {
  ModifyOrder() = default;
  ModifyOrder(ModifyOrder &&) = default;
  ModifyOrder(const ModifyOrder &) = delete;

  std::string_view account;  //!< Account name (as known to the gateway)
  uint32_t order_id = {};    //!< Order identifier (as known to client)
  double quantity = NaN;     //!< New quantity
  double price = NaN;        //!< New limit price
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ModifyOrder> : public roq::formatter {
  template <typename Context>
  auto format(const roq::ModifyOrder &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(quantity={}, )"
        R"(price={})"
        R"(}})"_fmt,
        value.account,
        value.order_id,
        value.quantity,
        value.price);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::ModifyOrder> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::ModifyOrder> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(modify_order={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
