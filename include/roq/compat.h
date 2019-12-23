/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <stdexcept>

namespace roq {

// transition to C++20

template <typename T>
class span final {
 public:
  using value_type = T;

  span() noexcept
      : _array(nullptr),
        _length(0) {
  }

  span(const T *array, size_t length) noexcept
      : _array(array),
        _length(length) {
  }

  const T *data() const {
    return _array;
  }

  bool empty() const {
    return _length == 0;
  }

  size_t size() const {
    return _length;
  }

  size_t size_bytes() const {
    return _length * sizeof(value_type);
  }

  const T& operator[](size_t index) const {
    return _array[index];
  }

  const T& at(size_t index) const {
    if (_length <= index)
      throw std::out_of_range();
    return _array[index];
  }

  const T *begin() const {
    return _array;
  }

  const T *end() const {
    return _array + _length;
  }

 private:
  const T *_array;
  const size_t _length;
};

}  // namespace roq
