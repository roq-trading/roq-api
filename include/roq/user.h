/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <chrono>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "roq/compat.h"

namespace roq {

struct ROQ_PUBLIC User {
  uint8_t id;
  std::string name;
  std::string password;
  std::unordered_set<std::string> accounts;
  std::unordered_map<
    std::string,
    std::unordered_set<std::string> > symbols;
  struct Limits final {
    struct CreateOrder final {
      uint32_t max;
      std::chrono::seconds monitor_period;
      std::chrono::seconds ban_period;
    } create_order;
  } limits;
};

}  // namespace roq

template <>
struct fmt::formatter<roq::User> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::User& value, T& ctx) {
    return format_to(
        ctx.out(),
        FMT_STRING(
            R"({{)"
            R"(id={}, )"
            R"(name="{}", )"
            R"(password=***, )"
            R"(accounts=[{}], )"
            R"(symbols=..., )"
            R"(limits=...)"
            R"(}})"),
        value.id,
        value.name,
        fmt::join(
            value.accounts,
            R"(, )"));
        // fmt::join(value.symbols, R"(, )"));
  }
};
