/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <vector>

#include "roq/platform.h"

namespace roq {
namespace logging {

template <typename T, size_t alignment>
struct aligned_allocator {
  typedef T value_type;
  aligned_allocator() = default;
  T *allocate(size_t size) {
#if defined(__APPLE__)
    void *pointer;
    if (posix_memalign(&pointer, alignment, size) != 0)
      throw std::bad_alloc();
    return reinterpret_cast<T *>(pointer);
#else
    void *pointer = aligned_alloc(alignment, size);
#endif
    return reinterpret_cast<T *>(pointer);
  }
  void deallocate(T *pointer, size_t size) noexcept {
    ::free(pointer);
  }
};

template <typename T>
struct cache_line_aligned_allocator
    : public aligned_allocator<T, cache_line_size()> {
};

template <typename T>
struct page_aligned_allocator
    : public aligned_allocator<T, page_size()> {
};

template <typename T>
struct cache_line_aligned_vector
    : public std::vector<T, cache_line_aligned_allocator<T> > {
  typedef std::vector<T, cache_line_aligned_allocator<T> > base_type;
  using base_type::base_type;
};

template <typename T>
struct page_aligned_vector
    : public std::vector<T, page_aligned_allocator<T> > {
  typedef std::vector<T, page_aligned_allocator<T> > base_type;
  using base_type::base_type;
};

}  // namespace logging
}  // namespace roq
