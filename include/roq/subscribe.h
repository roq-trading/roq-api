/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/ranges.h>

#include <map>
#include <set>
#include <string>

#include "roq/compat.h"
#include "roq/format.h"
#include "roq/literals.h"

#include "roq/cancel_policy.h"
#include "roq/event.h"
#include "roq/message_info.h"

namespace roq {

struct ROQ_PUBLIC Subscribe final {
  std::set<std::string> accounts;
  std::map<std::string, std::set<std::string> > symbols_by_exchange;
  CancelPolicy cancel_policy = {};
};

struct ROQ_PUBLIC SubscribeEvent final {
  const MessageInfo &message_info;
  const Subscribe &subscribe;
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Subscribe> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Subscribe &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        "{{"
        "accounts={}, "
        "symbols_by_exchange={}, "
        "cancel_policy={}"
        "}}"_sv,
        value.accounts,
        value.symbols_by_exchange,
        value.cancel_policy);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Subscribe> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::Subscribe> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        "{{"
        "message_info={}, "
        "subscribe={}"
        "}}"_sv,
        event.message_info,
        event.value);
  }
};
