/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <cstddef>

#if defined(__GNUC__)
#define ROQ_PUBLIC __attribute__((visibility("default")))
#define ROQ_HOT __attribute__((hot))
#define ROQ_PACKED __attribute__((packed))
#else  // not gnuc
#if defined(__clang__)
#error "expected __clang__ to also define __GNUC__"
#endif
#error "unsupported compiler"
#endif

#if defined(__APPLE__)
// sysctl -a | grep -e vm.pagesize -e hw.cachelinesize
#if defined(__arm64__)
static constexpr size_t const ROQ_CACHELINE_SIZE = 128;
static constexpr size_t const ROQ_PAGE_SIZE = 16384;
#else  // not __arm64__
static constexpr size_t const ROQ_CACHELINE_SIZE = 64;
static constexpr size_t const ROQ_PAGE_SIZE = 4096;
#endif
#else  // not __APPLE__
// getconf -a | grep -e PAGE_SIZE -e LEVEL1_DCACHE_LINESIZE
static constexpr size_t const ROQ_CACHELINE_SIZE = 64;
static constexpr size_t const ROQ_PAGE_SIZE = 4096;
#endif
