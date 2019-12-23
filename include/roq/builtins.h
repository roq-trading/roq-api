/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <cstdint>

namespace roq {

// atomic

enum class Atomic : int {
  RELAXED = __ATOMIC_RELAXED,
  CONSUME = __ATOMIC_CONSUME,
  ACQUIRE = __ATOMIC_ACQUIRE,
  RELEASE = __ATOMIC_RELEASE,
  ACQUIRE_RELEASE = __ATOMIC_ACQ_REL,
  SEQUENTIAL_CONSISTENT = __ATOMIC_SEQ_CST,
};

inline std::uint32_t atomic_acquire(const std::uint32_t& value) {
  return __atomic_load_4(&value, static_cast<int>(Atomic::ACQUIRE));
}

inline std::uint64_t atomic_acquire(const std::uint64_t& value) {
  return __atomic_load_8(&value, static_cast<int>(Atomic::ACQUIRE));
}

inline void atomic_release(std::uint32_t& result, std::uint32_t value) {
  __atomic_store_4(&result, value, static_cast<int>(Atomic::RELEASE));
}

inline void atomic_release(std::uint64_t& result, std::uint64_t value) {
  __atomic_store_8(&result, value, static_cast<int>(Atomic::RELEASE));
}

// expect

inline bool expect(bool expr, bool res) {
  return __builtin_expect(expr, res);
}

inline bool likely(bool expr) {
  return __builtin_expect(expr, true);
}

inline bool unlikely(bool expr) {
  return __builtin_expect(expr, false);
}

// prefetch

enum class Access : int {
  READ = 0,
  WRITE = 1,
};

enum class TemporalLocality : int {
  NO = 0,
  LOW = 1,
  MODERATE = 2,
  HIGH = 3,
};

template <
  Access access,
  TemporalLocality temporal_locality,
  typename T>
inline void prefetch(T *address) {
  __builtin_prefetch(
      address,
      static_cast<int>(access),
      static_cast<int>(temporal_locality));
}

}  // namespace roq
