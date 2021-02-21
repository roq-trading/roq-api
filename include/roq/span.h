/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

// objective: prepare for using <span> (since C++20)

// https://github.com/martinmoene/span-lite
#include <nonstd/span.hpp>

// namespace envelope so we can more easily manage and replace
// dependencies when making the transition to C++20

namespace roq {

using nonstd::span;

}  // namespace roq
