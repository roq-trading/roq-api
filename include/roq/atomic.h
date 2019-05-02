/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#if defined(__APPLE__)
#include <cstdint>
// FIXME(thraneh): find suitable builtins...
inline void __atomic_store_8(
    uint64_t *target,
    uint64_t value,
    int memory_order) {
  *target = value;
}
inline uint64_t __atomic_load_8(
    const uint64_t *source,
    int memory_order) {
  return *source;
}
#endif
