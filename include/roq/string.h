/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <algorithm>
#include <array>
#include <string_view>

#include "roq/compat.h"

namespace roq {

// fixed length string buffer

template <std::size_t N>
class ROQ_PACKED string final {
 public:
  using value_type = char;

  string() {
    set_length(0);
  }

  string(const std::string_view& text) {  // NOLINT (allow implicit)
    copy(text);
  }

  string(const value_type *text)  // NOLINT (allow implicit)
      : string(std::string_view(text)) {
  }

  string& operator=(const std::string_view& text) {
    copy(text);
    return *this;
  }

  bool operator==(const string<N>& rhs) const {
    auto len = length();
    return len == rhs.length()
      ? std::equal(
          data(),
          data() + len,
          rhs.data())
      : false;
  }

  constexpr std::size_t size() {
    return N-1;  // using 1 element for length
  }

  inline std::size_t length() const {
    return static_cast<std::size_t>(_buffer[0]);
  }

  inline bool empty() const {
    return length() == 0;
  }

  const value_type *data() const {
    return _buffer.data() + 1;
  }

  operator std::string_view() const {
    return std::string_view(
        data(),
        length());
  }

  void push_back(value_type value) {
    auto index = length();
    if (size() <= index)
      throw std::length_error("exceeded max length");
    _buffer[index + 1] = value;
    set_length(index + 1);
  }

  void clear() {
    set_length(0);
  }

 protected:
  value_type *data() {
    return _buffer.data() + 1;
  }

  void set_length(std::size_t length) {
    _buffer[0] = static_cast<value_type>(length);
  }

  void copy(const std::string_view& text) {
    auto length = text.length();
    if (size() < length)
      throw std::length_error("exceeding max length");
    std::copy(
        text.begin(),
        text.end(),
        data());
    set_length(length);
  }

 private:
  std::array<value_type, N> _buffer;
};

template <std::size_t N>
inline bool operator==(
    const string<N>& lhs,  // NOLINT (confused with std::string)
    const string<N>& rhs) {  // NOLINT (confused with std::string)
  return lhs.operator==()(rhs);
}

}  // namespace roq
