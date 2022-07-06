/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>

namespace roq {

//! Enumeration of message encoding types (informational, only)
enum class Encoding : uint32_t {
  UNDEFINED = 0x0,
  FIX = 0x1,   //!< FIX
  JSON = 0x2,  //!< JSON
  SBE = 0x4,   //!< SBE
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Encoding> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Encoding const &value, Context &context) const {
    using namespace std::literals;
#if __cplusplus >= 202002L
    std::string_view name{[&]() {
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
        default:
          assert(false);
      }
      return "<UNKNOWN>"sv;
    }()};
#else
    std::string_view name{[&]() {
      switch (value) {
        case roq::Encoding::UNDEFINED:
          return "UNDEFINED"sv;
        case roq::Encoding::FIX:
          return "FIX"sv;
        case roq::Encoding::JSON:
          return "JSON"sv;
        case roq::Encoding::SBE:
          return "SBE"sv;
        default:
          assert(false);
      }
      return "<UNKNOWN>"sv;
    }()};
#endif
    return fmt::format_to(context.out(), "{}"sv, name);
  }
};
