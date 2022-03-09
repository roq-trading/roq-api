/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include <new>

#include "roq/compat.hpp"

using namespace std::literals;

namespace roq {

// note! not yet supported
#ifdef __cpp_lib_hardware_interference_size

#if defined(__APPLE__)
#if defined(__arm64__)
static_assert(std::hardware_destructive_interference_size == 128);
static_assert(std::hardware_constructive_interference_size == 128);
#else  // not __arm64__
static_assert(std::hardware_destructive_interference_size == 64);
static_assert(std::hardware_constructive_interference_size == 64);
#endif
#else  // not __APPLE__
static_assert(std::hardware_destructive_interference_size == 64);
static_assert(std::hardware_constructive_interference_size == 64);
#endif

#endif

}  // namespace roq
