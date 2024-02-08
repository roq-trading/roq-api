/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {

//! Enumeration of transport types (layer 4)
enum class Transport : uint8_t {
  UNDEFINED = 0,
  TCP,  //!< TCP
  UDP,  //!< UDP
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Transport> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Transport const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
