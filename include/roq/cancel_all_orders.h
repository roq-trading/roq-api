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

//! Fields required to cancel all orders
struct ROQ_PUBLIC CancelAllOrders final {
  std::string_view account;  //!< Account name
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelAllOrders> : public roq::formatter {
  template <typename Context>
  auto format(const roq::CancelAllOrders &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}")"
        R"(}})"_sv,
        value.account);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CancelAllOrders> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::CancelAllOrders> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(cancel_all_orders={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
