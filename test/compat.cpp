/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include <new>

#if defined(__APPLE__)
#if defined(__arm64__)
static_assert(std::hardware_destructive_interference_size == 256);
static_assert(std::hardware_constructive_interference_size == 64);
#else  // not __arm64__
static_assert(std::hardware_destructive_interference_size == 64);
static_assert(std::hardware_constructive_interference_size == 64);
#endif
#else  // not __APPLE__
#if defined(__arm64__)
static_assert(std::hardware_destructive_interference_size == 256);  // note! since gcc12
static_assert(std::hardware_constructive_interference_size == 64);
#else  // not __arm64__
static_assert(std::hardware_destructive_interference_size == 256);
static_assert(std::hardware_constructive_interference_size == 64);
#endif
#endif
