/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

namespace roq {

//! Enumeration of update types
enum class UpdateType : uint8_t {
  UNDEFINED = 0,
  SNAPSHOT,     //!< Full snapshot
  INCREMENTAL,  //!< Incremental change
  STALE,        //!< Stale awaiting full snapshot
};

}  // namespace roq
