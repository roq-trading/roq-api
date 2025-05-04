/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <cstdlib>

namespace roq {

//! Enumeration of message encoding types (informational, only)
enum class Encoding : uint32_t {
  UNDEFINED = 0,
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
    auto name = [&]() -> std::string_view {
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
      }
      std::abort();
    }();
    return fmt::format_to(context.out(), "{}"sv, name);
  }
};
