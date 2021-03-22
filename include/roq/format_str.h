/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

// objectives:
// - require explicit identification of format strings
// - (eventually) make compile-time validation of format strings possible

#include <string_view>

namespace roq {

struct format_str final {
  explicit constexpr format_str(const std::string_view &fmt) : fmt_(fmt) {}

  constexpr format_str(format_str &&) = default;
  constexpr format_str(const format_str &) = delete;

  constexpr operator std::string_view() const { return fmt_; }

 private:
  const std::string_view fmt_;
};

}  // namespace roq
