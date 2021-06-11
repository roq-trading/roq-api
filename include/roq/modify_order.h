/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Fields required to modify an existing order
struct ROQ_PUBLIC ModifyOrder final {
  std::string_view account;             //!< Account name
  uint32_t order_id = {};               //!< Order identifier
  double quantity = NaN;                //!< New (total) quantity
  double price = NaN;                   //!< New limit price
  std::string_view routing_id;          //!< Routing identifier
  uint8_t version = {};                 //!< Version number (strictly increasing, optional)
  uint8_t conditional_on_version = {};  //!< Auto-reject if this version has positively failed (optional)
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
        R"(price={}, )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(conditional_on_version={})"
        R"(}})"_fmt,
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
