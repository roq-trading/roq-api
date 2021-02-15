/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

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

//! Update relating to available funds
struct ROQ_PUBLIC FundsUpdate final {
  FundsUpdate() = default;
  FundsUpdate(FundsUpdate &&) = default;
  FundsUpdate(const FundsUpdate &) = delete;

  std::string_view account;   //!< Account name (as known to the gateway)
  std::string_view currency;  //!< Currency name
  double balance = std::numeric_limits<double>::quiet_NaN();  //!< Current funds
  double hold = std::numeric_limits<double>::quiet_NaN();     //!< Funds on hold
  std::string_view external_account;  //!< External account name (as known to broker or exchange)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::FundsUpdate> : public roq::formatter {
  template <typename Context>
  auto format(const roq::FundsUpdate &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(currency="{}", )"
        R"(balance={}, )"
        R"(hold={}, )"
        R"(external_account="{}")"
        R"(}})"_fmt,
        value.account,
        value.currency,
        value.balance,
        value.hold,
        value.external_account);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::FundsUpdate> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::FundsUpdate> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(funds_update={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
