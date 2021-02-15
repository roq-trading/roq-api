/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/span.h"

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
struct fmt::formatter<roq::CancelOrder> : public roq::formatter {
  template <typename Context>
  auto format(const roq::CancelOrder &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={})"
        R"(}})"_fmt,
        value.account,
        value.order_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CancelOrder> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::CancelOrder> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(cancel_order={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
