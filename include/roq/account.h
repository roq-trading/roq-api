/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "roq/compat.h"

namespace roq {

struct ROQ_PUBLIC Account {
  uint8_t id;
  std::string name;
  std::string user;
  std::string login;
  std::string password;
  std::string secret;
  std::unordered_map<
    std::string,
    std::unordered_set<std::string> > symbols;
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Account> {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const roq::Account& value, T& ctx) {
    return format_to(
        ctx.out(),
        R"({{)"
        R"(id={}, )"
        R"(name="{}", )"
        R"(user="{}", )"
        R"(login="{}", )"
        R"(password=***, )"
        R"(secret=***, )"
        R"(symbols=...)"
        R"(}})",
        value.id,
        value.name,
        value.user,
        value.login);
        // fmt::join(value.symbols, R"(, )"));
  }
};
