/* Copyright (c) 2017-2025, Hans Erik Thrane */

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
#if defined(__arm64__)
static_assert(std::hardware_destructive_interference_size == 256);  // note! since gcc12
#else  // not __arm64__
static_assert(std::hardware_constructive_interference_size == 64);
#endif
#endif

#endif

}  // namespace roq
