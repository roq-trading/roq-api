/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/compile.h>
#include <fmt/format.h>

namespace roq {

//! Order life-time, aka time in force
enum class TimeInForce : uint8_t {
  UNDEFINED = 0,
  GFD,                    //!< Good for day, aka DAY
  GTC,                    //!< Good till canceled
  OPG,                    //!< At the open
  IOC,                    //!< Immediate or cancel
  FOK,                    //!< Fill or kill
  GTX,                    //!< Good till crossing
  GTD,                    //!< Good till date
  AT_THE_CLOSE,           //!< At the close
  GOOD_THROUGH_CROSSING,  //!< Good through crossing
  AT_CROSSING,            //!< At crossing
  GOOD_FOR_TIME,          //!< Good for time
  GFA,                    //!< Good for auction
  GFM,                    //!< Good for this month
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TimeInForce> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TimeInForce const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
