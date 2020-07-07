/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

#include "roq/compat.h"

namespace roq {

// fixed length string buffer

template <std::size_t N>
class ROQ_PACKED string final {
 public:
  using value_type = char;

  string() = default;

  string(const std::string_view& text) {  // NOLINT (allow implicit)
    copy(text);
  }

  string(const std::string& text) {  // NOLINT (allow implicit)
    copy(text);
  }

  string(const value_type *text)  // NOLINT (allow implicit)
      : string(std::string_view(text)) {
  }

  string& operator=(const std::string_view& text) {
    copy(text);
    return *this;
  }

  string& operator=(const std::string& text) {
    copy(text);
    return *this;
  }

  bool operator==(const string<N>& rhs) const {
    return static_cast<std::string_view>(*this).compare(
        static_cast<std::string_view>(rhs)) == 0;
  }

  value_type& operator[](size_t index) {
    return _buffer[index];
  }

  value_type operator[](size_t index) const {
    return _buffer[index];
  }

  constexpr std::size_t size() {
    return N;
  }

  inline std::size_t length() const {
    auto iter = std::find(
        _buffer.begin(),
        _buffer.end(),
        '\0');
    return iter - _buffer.begin();
  }

  inline bool empty() const {
    return _buffer[0] == '\0';
  }

  const value_type *data() const {
    return _buffer.data();
  }

  operator std::string_view() const {
    return std::string_view(
        data(),
        length());
  }

  void clear() {
    // note!
    // we prefer to clear the entire buffer (for security reasons)
    // even though it would be enough to only set the first element
    _buffer.fill('\0');
  }

 protected:
  value_type *data() {
    return _buffer.data();
  }

  void copy(const std::string_view& text) {
    auto length = text.length();
    if (ROQ_PREDICT_TRUE(length <= size())) {
      auto last = std::copy(
          text.begin(),
          text.end(),
          _buffer.begin());
      std::fill(
          last,
          _buffer.end(),
          '\0');
    } else {
      throw std::length_error(
          fmt::format(
              R"(can't copy: len(text="{}")={} exceeds size={})",
              text,
              length,
              size()));
    }
  }

 private:
  std::array<value_type, N> _buffer = {};
};

template <std::size_t N>
inline bool operator==(
    const string<N>& lhs,  // NOLINT (confused with std::string)
    const string<N>& rhs) {  // NOLINT (confused with std::string)
  return lhs.operator==()(rhs);
}

}  // namespace roq
