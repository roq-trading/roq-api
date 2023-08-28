/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#if defined(__GNUC__)
#define ROQ_PUBLIC __attribute__((visibility("default")))
#define ROQ_HOT [[gnu::hot]]
#define ROQ_PACKED [[gnu::packed]]
#else  // not gnuc
#if defined(__clang__)
#error "expected __clang__ to also define __GNUC__"
#endif
#define ROQ_PUBLIC
#define ROQ_HOT
#define ROQ_PACKED
#endif

#if defined(__APPLE__)
// https://stackoverflow.com/a/73090806
#define FMT_USE_NONTYPE_TEMPLATE_ARGS 1
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

// note!
//
// linux:
// $ getconf -a | grep -e PAGE_SIZE -e LEVEL1_DCACHE_LINESIZE
//
// macos:
// $ sysctl -a | grep -e vm.pagesize -e hw.cachelinesize
