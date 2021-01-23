/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <cstddef>
#include <stdexcept>

#include "roq/compat.h"

namespace roq {

//! A non-owning contiguous array of objects.
/*!
 * This is useful for providing access to raw data without leaking how data is
 * managed.
 *
 * The interface is a subset of \ref std::vector and
 * \ref std::array.
 *
 * The implementation closely follows the design of
 * `std::span <https://en.cppreference.com/w/cpp/container/span>`_ (since
 * C++20).
 */
template <typename T>
class span final {
 public:
  using value_type = T;  // XXX std::remove_cv_t<T>
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = pointer;

  span() noexcept : array_(nullptr), length_(0) {}

  span(pointer array, size_t length) noexcept
      : array_(array), length_(length) {}

  template <typename C>
  // cppcheck-suppress noExplicitConstructor
  span(C &container)  // NOLINT
      : span(container.data(), container.size()) {}

  span(const span &) = default;
  span(span &&) = default;

  span &operator=(const span &) = default;
  span &operator=(span &&) = default;

  pointer data() { return array_; }
  const_pointer data() const { return array_; }

  bool empty() const { return length_ == 0; }

  size_t size() const { return length_; }

  size_t size_bytes() const { return length_ * sizeof(value_type); }

  reference operator[](size_t index) { return array_[index]; }
  const_reference operator[](size_t index) const { return array_[index]; }

  reference at(size_t index) {
    if (length_ <= index)
      throw std::out_of_range();
    return array_[index];
  }
  const_reference at(size_t index) const {
    if (length_ <= index)
      throw std::out_of_range();
    return array_[index];
  }

  iterator begin() const { return array_; }
  iterator end() const { return array_ + length_; }
  iterator cbegin() const { return array_; }
  iterator cend() const { return array_ + length_; }

  auto subspan(size_t offset, size_t count) const {
    // according to standard: undefined behavior if offset/count is out of range
    return span(array_ + offset, count);
  }
  auto subspan(size_t offset) const {
    return subspan(offset, length_ - offset);
  }

 private:
  pointer array_;
  size_t length_;
};

}  // namespace roq
