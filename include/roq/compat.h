/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define ROQ_PUBLIC __attribute__((visibility("default")))
#define ROQ_HOT __attribute__((hot))
#define ROQ_PACKED __attribute__((__packed__))
#else
#define ROQ_PUBLIC
#define ROQ_HOT
#define ROQ_PACKED
#endif

// https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2017
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0154r1.html
#define ROQ_CACHELINE_SIZE 64

// XXX need runtime detection
#define ROQ_PAGE_SIZE 4096

#define ROQ_PREDICT_TRUE(x) (__builtin_expect(false || (x), true))
#define ROQ_PREDICT_FALSE(x) (__builtin_expect(x, false))
