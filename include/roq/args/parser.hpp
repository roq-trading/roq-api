/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <span>
#include <string_view>

namespace roq {
namespace args {

struct ROQ_PUBLIC Parser {
  virtual ~Parser() = default;

  // note! c-style with first element being the program name
  virtual operator std::span<std::string_view const>() const = 0;

  auto program_name() const { return static_cast<std::span<std::string_view const>>(*this)[0]; }
  auto params() const { return static_cast<std::span<std::string_view const>>(*this).subspan(1); }
};

}  // namespace args
}  // namespace roq
