/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define ROQ_PUBLIC [[gnu::visibility("default")]]
#define ROQ_HOT [[gnu::hot]]
#define ROQ_PACKED [[gnu::packed]]
#else  // not gcc or clang
#define ROQ_PUBLIC
#define ROQ_HOT
#define ROQ_PACKED
#endif

#if defined(__APPLE__)
#if defined(__arm64__)
#define ROQ_CACHELINE_SIZE 128
#define ROQ_PAGE_SIZE 16384
#else  // not __arm64__
#define ROQ_CACHELINE_SIZE 64
#define ROQ_PAGE_SIZE 4096
#endif
#else  // not __APPLE__
#define ROQ_CACHELINE_SIZE 64
#define ROQ_PAGE_SIZE 4096
#endif

// note!
//
// constexpr std::hardware_{constructive,destructive}_interference_size
//
// not supported by gcc:
//   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0154r1.html
//   https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2017
