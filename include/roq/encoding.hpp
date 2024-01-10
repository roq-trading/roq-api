/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <cassert>

namespace roq {

//! Enumeration of message encoding types (informational, only)
enum class Encoding : uint32_t {
  UNDEFINED = 0x0,
  FIX = 0x1,   //!< FIX
  JSON = 0x2,  //!< JSON
  SBE = 0x4,   //!< SBE
  FBS = 0x8,   //!< FlatBuffers
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Encoding> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Encoding const &value, format_context &context) const {
    using namespace std::literals;
    auto name{[&]() {
      switch (value) {
        using enum roq::Encoding;
        case UNDEFINED:
          return "UNDEFINED"sv;
        case FIX:
          return "FIX"sv;
        case JSON:
          return "JSON"sv;
        case SBE:
          return "SBE"sv;
        case FBS:
          return "FBS"sv;
        default:
          assert(false);
      }
      return "<UNKNOWN>"sv;
    }()};
    return fmt::format_to(context.out(), "{}"sv, name);
  }
};
