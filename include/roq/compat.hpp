/* Copyright (c) 2017-2024, Hans Erik Thrane */

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

// how to find the actual cache-line size
// linux: getconf -a | grep -e PAGE_SIZE -e LEVEL1_DCACHE_LINESIZE
// macos: sysctl -a | grep -e vm.pagesize -e hw.cachelinesize
