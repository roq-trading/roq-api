/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

namespace roq {

//! Fields required to cancel an existing order
struct ROQ_PUBLIC CancelOrder final {
  CancelOrder() = default;
  CancelOrder(CancelOrder &&) = default;
  CancelOrder(const CancelOrder &) = delete;

  std::string_view account;  //!< Account name (as known to the gateway)
  uint32_t order_id = 0;     //!< Order identifier (as known to client)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelOrder> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::CancelOrder &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={})"
        R"(}})",
        value.account,
        value.order_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CancelOrder> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::CancelOrder> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(cancel_order={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
