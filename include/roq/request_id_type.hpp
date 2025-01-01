/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

namespace roq {

//! Request identifier
enum class RequestIdType : uint8_t {
  UNDEFINED = 0,
  BASE64,
  BASE32,
  SIMPLE,
  UUID,
};

}  // namespace roq
