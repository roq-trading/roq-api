/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Enumeration of request status types
enum class RequestStatus : uint8_t {
  UNDEFINED = 0,
  FORWARDED,
  ACCEPTED,
  REJECTED,
  DISCONNECTED,
  TIMEOUT,
  FAILED,  //!< Generic failure
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RequestStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RequestStatus const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
