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
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string.hpp"

namespace roq {

//! Update relating to available funds
struct ROQ_PUBLIC FundsUpdate final {
  uint16_t stream_id = {};            //!< Stream identifier
  std::string_view account;           //!< Account name
  std::string_view currency;          //!< Currency
  double balance = NaN;               //!< Current funds
  double hold = NaN;                  //!< Funds on hold
  std::string_view external_account;  //!< External account name
};

}  // namespace roq

template <>
struct fmt::formatter<roq::FundsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::FundsUpdate &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(currency="{}", )"
        R"(balance={}, )"
        R"(hold={}, )"
        R"(external_account="{}")"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.currency,
        value.balance,
        value.hold,
        value.external_account);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::FundsUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::FundsUpdate> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(funds_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
