/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

namespace roq {

//! Enumeration of a specific order management style
enum class OrderManagement : uint8_t {
  UNDEFINED = 0,
  FIX,  //!< Client provides a globally unique routing_id
};

}  // namespace roq
