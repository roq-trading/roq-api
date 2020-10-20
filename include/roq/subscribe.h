/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <map>
#include <set>
#include <string>

#include "roq/compat.h"

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
struct fmt::formatter<roq::Subscribe> {
  template <typename T>
  constexpr auto parse(T &ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Subscribe &value, T &ctx) {
    return format_to(
        ctx.out(),
        R"({{)"
        R"(accounts={}, )"
        R"(symbols_by_exchange={})"
        R"(}})",
        value.accounts,
        value.symbols_by_exchange);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Subscribe> > {
  template <typename T>
  constexpr auto parse(T &ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Event<roq::Subscribe> &event, T &ctx) {
    return format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(subscribe={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
