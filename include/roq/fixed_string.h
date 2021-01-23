/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

#include "roq/compat.h"

namespace roq {

// fixed length string buffer
//! A fixed-length string buffer with automatic conversion to/from
//! \ref std::string_view.
/*!
 * This is useful for managing fixed length arrays as if they were string-like.
 * Typically used to avoid heap allocations, e.g. stack allocated structs used
 * for messaging.

 * This is a higher level abstraction than \ref std::array to provide
 * more * string-specific features.
 * The interface is a subset of \ref std::string and \ref std::string_view.
 */
template <std::size_t N>
class ROQ_PACKED fixed_string final {
 public:
  using value_type = char;

  fixed_string() = default;

  // cppcheck-suppress noExplicitConstructor
  fixed_string(const std::string_view &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  fixed_string(const std::string &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  fixed_string(const value_type *text)  // NOLINT (allow implicit)
      : fixed_string(std::string_view(text)) {}

  fixed_string &operator=(const std::string_view &text) {
    copy(text);
    return *this;
  }

  fixed_string &operator=(const std::string &text) {
    copy(text);
    return *this;
  }

  bool operator==(const fixed_string<N> &rhs) const {
    return static_cast<std::string_view>(*this).compare(
               static_cast<std::string_view>(rhs)) == 0;
  }

  value_type &operator[](size_t index) { return buffer_[index]; }

  value_type operator[](size_t index) const { return buffer_[index]; }

  constexpr std::size_t size() { return N; }

  inline std::size_t length() const {
    auto iter = std::find(buffer_.begin(), buffer_.end(), '\0');
    return iter - buffer_.begin();
  }

  inline bool empty() const { return buffer_[0] == '\0'; }

  const value_type *data() const { return buffer_.data(); }

  operator std::string_view() const {
    return std::string_view(data(), length());
  }

  void clear() {
    // note!
    // we prefer to clear the entire buffer (for security reasons)
    // even though it would be enough to only set the first element
    buffer_.fill('\0');
  }

 protected:
  value_type *data() { return buffer_.data(); }

  void copy(const std::string_view &text) {
    auto len = text.length();
    if (ROQ_LIKELY(len <= size())) {
      auto last = std::copy(text.begin(), text.end(), buffer_.begin());
      std::fill(last, buffer_.end(), '\0');
    } else {
      throw std::length_error(fmt::format(
          R"(can't copy: len(text="{}")={} exceeds size={})",
          text,
          len,
          size()));
    }
  }

 private:
  std::array<value_type, N> buffer_ = {};
};

template <std::size_t N>
inline bool operator==(const fixed_string<N> &lhs, const fixed_string<N> &rhs) {
  return lhs.operator==()(rhs);
}

}  // namespace roq
