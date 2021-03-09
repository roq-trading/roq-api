/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

// objective: prepare for using <span> (since C++20)

// using namespace envelope so we can more easily manage and replace
// dependencies when making the transition to C++20

#if __cplusplus >= 202002L
#include <span>
namespace roq {
using std::span;
}
#else
#include <nonstd/span.hpp>  // https://github.com/martinmoene/span-lite
namespace roq {
using nonstd::span;
}  // namespace roq
#endif
