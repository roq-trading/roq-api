/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/ranges.h>

#include <map>
#include <set>
#include <string>

#include "roq/compat.h"
#include "roq/format.h"
#include "roq/literals.h"

#include "roq/event.h"
#include "roq/message_info.h"

namespace roq {

struct ROQ_PUBLIC Subscribe final {
  std::set<std::string> accounts;
  std::map<std::string, std::set<std::string> > symbols_by_exchange;
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
        R"({{)"
        R"(accounts={}, )"
        R"(symbols_by_exchange={})"
        R"(}})"_fmt,
        value.accounts,
        value.symbols_by_exchange);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Subscribe> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::Subscribe> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(subscribe={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
