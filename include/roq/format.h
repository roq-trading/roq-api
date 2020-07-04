/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "roq/api.h"


// NOTE!
// we're extending the fmt namespace with formatter specializations
// this is the recommended practice

// helper

// XXX make generic
template <>
struct fmt::formatter<std::set<std::string> > {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(const std::set<std::string>& value, T& ctx) {
    if (value.empty())
      return format_to(
          ctx.out(),
          R"({{)"
          R"(}})");
    return format_to(
        ctx.out(),
        R"({{)"
        R"("{}")"
        R"(}})",
        fmt::join(
            value,
            R"(", ")"));
  }
};

// XXX make generic
template <>
struct fmt::formatter<std::pair<const std::string, std::set<std::string> > > {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(
      const std::pair<const std::string, std::set<std::string> >& value,
      T& ctx) {
    return format_to(
        ctx.out(),
        R"("{}"={})",
        value.first,
        value.second);
  }
};

// XXX make generic
template <>
struct fmt::formatter<std::map<std::string, std::set<std::string> > > {
  template <typename T>
  constexpr auto parse(T& ctx) {
    return ctx.begin();
  }
  template <typename T>
  auto format(
      const std::map<std::string, std::set<std::string> >& value,
      T& ctx) {
    return format_to(
        ctx.out(),
        R"({{)"
        R"({})"
        R"(}})",
        fmt::join(
            value.begin(),
            value.end(),
            R"(, )"));
  }
};
