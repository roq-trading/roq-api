/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <compare>
#include <string>
#include <string_view>
#include <utility>

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
struct ROQ_PACKED String {
  using value_type = char;

  constexpr String() = default;

  constexpr String(String &&) = default;
  constexpr String(String const &) = default;

  constexpr String &operator=(String &&) = default;
  constexpr String &operator=(String const &) = default;

  constexpr String(std::string_view const &text) { copy(text); }

  constexpr String(std::string const &text) { copy(text); }

  constexpr String(value_type const *text) : String{std::string_view{text}} {}

  constexpr String &operator=(std::string_view const &text) {
    copy(text);
    return *this;
  }

  constexpr String &operator=(std::string const &text) {
    copy(text);
    return *this;
  }

  template <std::size_t M>
  constexpr auto operator<=>(String<M> const &rhs) const {
    return static_cast<std::string_view>(*this) <=> static_cast<std::string_view>(rhs);
  }

  template <std::size_t M>
  constexpr bool operator==(String<M> const &rhs) const {
    return (*this) == static_cast<std::string_view>(rhs);
  }

  constexpr auto operator<=>(std::string_view const &rhs) const { return static_cast<std::string_view>(*this) <=> rhs; }

  constexpr bool operator==(std::string_view const &rhs) const { return static_cast<std::string_view>(*this) == rhs; }

  constexpr value_type &operator[](size_t index) { return buffer_[index]; }

  constexpr value_type operator[](size_t index) const { return buffer_[index]; }

  constexpr std::size_t size() const { return N; }

  constexpr std::size_t length() const {
    auto tmp = static_cast<size_t>(static_cast<unsigned char>(buffer_[N - 1]));
    if (buffer_[N - 2]) {
      return N - (tmp != 0 ? 0 : 1);
    }
    return tmp;
  }

  constexpr bool empty() const { return length() == 0; }

  constexpr value_type const *data() const { return std::data(buffer_); }

  constexpr operator std::string_view() const { return {data(), length()}; }

  constexpr void clear() {
    // note!
    // we prefer to clear the entire buffer (for security reasons)
    // even though it would be enough to only set the first element
    std::fill(std::begin(buffer_), std::end(buffer_), '\0');
  }

  constexpr void push_back(value_type value) {
    using namespace std::literals;
    auto len = length();
    if (N <= len) [[unlikely]] {
      throw LengthError{"String buffer is full"sv};
    }
    buffer_[len] = value;
    set_length(++len);
  }

 protected:
  constexpr value_type *data() { return std::data(buffer_); }

  constexpr void copy(std::string_view const &text) {
    using namespace std::literals;
    auto len = std::size(text);
    if (N < len) [[unlikely]] {
      throw LengthError{R"(can't copy: len(text="{}")={} exceeds size={})"sv, text, len, N};
    }
    std::copy(std::begin(text), std::end(text), std::begin(buffer_));
    set_length(len);
  }

  constexpr void set_length(size_t len) {
    if (len < N) {
      size_t last;
      if (len < (N - 1)) {
        buffer_[N - 2] = {};
        last = len;
      } else {
        last = {};
      }
      buffer_[N - 1] = static_cast<value_type>(last);
    }
  }

 private:
  std::array<value_type, N> buffer_ = {};
};
}  // namespace roq

template <size_t N>
struct fmt::formatter<roq::String<N>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::String<N> const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
