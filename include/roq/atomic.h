/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

// workaround: conda uses very old version of clang
#if defined(__APPLE__) && defined(__clang__) && (__clang_major__ < 5)
#include <cstdint>
inline void __atomic_store_8(
    uint64_t *target,
    uint64_t value,
    int memory_order) {
  *target = value;  // FIXME(thraneh): *not* atomic!
}
inline uint64_t __atomic_load_8(
    const uint64_t *source,
    int memory_order) {
  return *source;  // FIXME(thraneh): *not* atomic!
}
#endif
