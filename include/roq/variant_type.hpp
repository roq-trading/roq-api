/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <magic_enum.hpp>

#include <fmt/core.h>

namespace roq {

//! Enumeration of variant types
enum class VariantType : uint8_t {
  UNDEFINED = 0,
  STRING,    //!< ASCII string
  BOOL,      //!< Boolean
  INT8,      //!< Signed 8 bit integer
  UINT8,     //!< Unsigned 8 bit integer
  INT16,     //!< Signed 16 bit integer
  UINT16,    //!< Unsigned 16 bit integer
  INT32,     //!< Signed 32 bit integer
  UINT32,    //!< Unsigned 32 bit integer
  INT64,     //!< Signed 64 bit integer
  UINT64,    //!< Unsigned 64 bit integer
  FLOAT,     //!< 32 bit floating point
  DOUBLE,    //!< 64 bit floating point
  ENUM,      //!< Enumeration
  DATETIME,  //!< Nanoseconds relative to Unix epoch
};

}  // namespace roq

template <>
struct fmt::formatter<roq::VariantType> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::VariantType const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
