/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <string_view>
#include <tuple>

#include "roq/source_location.h"

namespace roq {

struct format_str final {
  template <typename T>
  constexpr format_str(const T &fmt, const source_location &loc = source_location::current()) : fmt_(fmt), loc_(loc) {}

  constexpr format_str(format_str &&) = default;
  constexpr format_str(const format_str &) = delete;

  constexpr operator const std::string_view &() const { return fmt_; }
  constexpr operator const source_location &() const { return loc_; }

 private:
  const std::string_view fmt_;
  const source_location loc_;
};

}  // namespace roq
