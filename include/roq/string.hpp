/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <absl/hash/hash.h>

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
class ROQ_PACKED String {
 public:
  using value_type = char;

  constexpr String() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr String(std::string_view const &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr String(std::string const &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr String(value_type const *text)  // NOLINT (allow implicit)
      : String(std::string_view{text}) {}

  constexpr String &operator=(std::string_view const &text) {
    copy(text);
    return *this;
  }

  constexpr String &operator=(std::string const &text) {
    copy(text);
    return *this;
  }

  // abseil hash (heterogeneous lookup)
  template <typename H>
  friend H AbslHashValue(H hash, String<N> const &rhs) {
    return H::combine(std::move(hash), static_cast<std::string_view>(rhs));
  }

  /* note! replace with this when all compilers support string_view spaceship
  template <std::size_t M>
  constexpr auto operator<=>(roq::String<M> const &rhs) const {
    return (*this) <=> static_cast<std::string_view>(rhs);
  }
  */
  // note! not sure why this is necessary
  template <std::size_t M>
  constexpr auto operator==(String<M> const &rhs) const {
    return (*this) == static_cast<std::string_view>(rhs);
  }
  template <std::size_t M>
  constexpr auto operator<(String<M> const &rhs) const {
    return (*this) < static_cast<std::string_view>(rhs);
  }
  template <std::size_t M>
  constexpr auto operator>(String<M> const &rhs) const {
    return (*this) > static_cast<std::string_view>(rhs);
  }

  constexpr value_type &operator[](size_t index) { return buffer_[index]; }

  constexpr value_type operator[](size_t index) const { return buffer_[index]; }

  constexpr std::size_t size() const { return N; }

  constexpr std::size_t length() const {
    auto tmp = buffer_[N - 1];
    if (buffer_[N - 2])
      return N - (tmp ? 0 : 1);
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
    if (N <= len) [[unlikely]]
      throw LengthError("String buffer is full"sv);
    buffer_[len] = value;
    set_length(++len);
  }

 protected:
  constexpr value_type *data() { return std::data(buffer_); }

  constexpr void copy(std::string_view const &text) {
    using namespace std::literals;
    auto len = std::size(text);
    if (N < len) [[unlikely]]
      throw LengthError(R"(can't copy: len(text="{}")={} exceeds size={})"sv, text, len, N);
    std::copy(std::begin(text), std::end(text), std::begin(buffer_));
    set_length(len);
  }

  constexpr void set_length(size_t len) {
    if (len < N) {
      value_type last;
      if (len < (N - 1)) {
        buffer_[N - 2] = '\0';
        last = len;
      } else {
        last = '\0';
      }
      buffer_[N - 1] = last;
    }
  }

 private:
  std::array<value_type, N> buffer_ = {};
};
}  // namespace roq

template <std::size_t N>
inline constexpr auto operator<=>(roq::String<N> const &lhs, std::string_view const &rhs) {
#ifdef __clang__
  // note! clang14 does not support spaceship operator for std::string_view
  // https://libcxx.llvm.org/Status/Spaceship.html
  auto sign = static_cast<std::string_view>(lhs).compare(rhs);
  //   https://stackoverflow.com/a/4609795
  auto sign_helper = [](auto value) {
    using value_type = decltype(value);
    return (value_type{} < value) - (value < value_type{});
  };
  const std::array lookup{
      std::strong_ordering::less,
      std::strong_ordering::equal,
      std::strong_ordering::greater,
  };
  return lookup[sign_helper(sign) + 1];
#else
  return static_cast<std::string_view>(lhs) <=> rhs;
#endif
}

template <std::size_t N>
inline constexpr auto operator<=>(std::string_view const &lhs, roq::String<N> const &rhs) {
  // https://stackoverflow.com/a/60087347
  return 0 <=> (rhs <=> lhs);
}

template <std::size_t N>
inline constexpr auto operator<=>(roq::String<N> const &lhs, std::string const &rhs) {
  return lhs <=> std::string_view{rhs};
}

template <std::size_t N>
inline constexpr auto operator<=>(std::string const &lhs, roq::String<N> const &rhs) {
  return std::string_view{lhs} <=> rhs;
}

template <size_t N>
struct fmt::formatter<roq::String<N> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::String<N> const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
