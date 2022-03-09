/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <utility>

#include "roq/compat.hpp"
#include "roq/exceptions.hpp"

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
class ROQ_PACKED string_buffer final {
 public:
  using value_type = char;

  constexpr string_buffer() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr string_buffer(const std::string_view &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr string_buffer(const std::string &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr string_buffer(value_type const *text)  // NOLINT (allow implicit)
      : string_buffer(std::string_view(text)) {}

  constexpr string_buffer &operator=(const std::string_view &text) {
    copy(text);
    return *this;
  }

  constexpr string_buffer &operator=(const std::string &text) {
    copy(text);
    return *this;
  }

  constexpr bool operator==(const string_buffer<N> &rhs) const {
    return static_cast<std::string_view>(*this).compare(static_cast<std::string_view>(rhs)) == 0;
  }

  constexpr bool operator<(const string_buffer<N> &rhs) const {
    return static_cast<std::string_view>(*this).compare(static_cast<std::string_view>(rhs)) < 0;
  }

  template <typename... Args>
  constexpr int compare(Args &&...args) const {
    return static_cast<std::string_view>(*this).compare(std::forward<Args>(args)...);
  }

  template <typename T>
  constexpr bool operator<(const T &rhs) const {
    return compare(rhs) < 0;
  }

  template <typename T>
  constexpr bool operator>(const T &rhs) const {
    return compare(rhs) > 0;
  }

  constexpr value_type &operator[](size_t index) { return buffer_[index]; }

  constexpr value_type operator[](size_t index) const { return buffer_[index]; }

  constexpr std::size_t size() { return N; }

  inline constexpr std::size_t length() const {
    if (buffer_[N - 2] == '\0')
      return static_cast<std::size_t>(buffer_[N - 1]);
    return N - (buffer_[N - 1] == '\0' ? 1 : 0);
  }

  inline constexpr bool empty() const { return buffer_[0] == '\0'; }

  constexpr value_type const *data() const { return std::data(buffer_); }

  constexpr operator std::string_view() const { return std::string_view(data(), length()); }

  constexpr void clear() {
    // note!
    // we prefer to clear the entire buffer (for security reasons)
    // even though it would be enough to only set the first element
    buffer_.fill('\0');
  }

  constexpr void push_back(value_type value) {
    using namespace std::literals;
    auto len = length();
    if (N <= len) [[unlikely]] {
      throw LengthError("String buffer is full"sv);
    }
    buffer_[len] = value;
    ++len;
    if (len < (N - 1)) {
      buffer_[N - 1] = len;
    } else if (len < N) {
      buffer_[N - 1] = '\0';
    }
  }

 protected:
  constexpr value_type *data() { return std::data(buffer_); }

  constexpr void copy(const std::string_view &text) {
    using namespace std::literals;
    auto len = std::size(text);
    if (len <= size()) [[likely]] {
      auto last = std::copy(std::begin(text), std::end(text), std::begin(buffer_));
      std::fill(last, std::end(buffer_), '\0');  // convenient, but we don't need to write the last byte
      if (len < (N - 1))
        buffer_[N - 1] = len;
    } else {
      throw LengthError(R"(can't copy: len(text="{}")={} exceeds size={})"sv, text, len, size());
    }
  }

 private:
  std::array<value_type, N> buffer_ = {};
};

template <std::size_t N>
inline bool operator==(const string_buffer<N> &lhs, const string_buffer<N> &rhs) {
  return lhs.operator==()(rhs);
}

}  // namespace roq

template <size_t N>
struct fmt::formatter<roq::string_buffer<N> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::string_buffer<N> &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
