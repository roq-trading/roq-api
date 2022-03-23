/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/hash/hash.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <utility>

#include "roq/compat.hpp"
#include "roq/exceptions.hpp"
#include "roq/limits.hpp"

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
  constexpr String(const std::string_view &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr String(const std::string &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr String(value_type const *text)  // NOLINT (allow implicit)
      : String(std::string_view(text)) {}

  constexpr String &operator=(const std::string_view &text) {
    copy(text);
    return *this;
  }

  constexpr String &operator=(const std::string &text) {
    copy(text);
    return *this;
  }

  // hash
  template <typename H>
  friend H AbslHashValue(H hash, const String<N> &rhs) {
    return H::combine(std::move(hash), static_cast<std::string_view>(rhs));
  }

  // comparison
  template <typename... Args>
  constexpr int compare(Args &&...args) const {
    return static_cast<std::string_view>(*this).compare(std::forward<Args>(args)...);
  }

  constexpr bool operator==(const String<N> &rhs) const { return compare(static_cast<std::string_view>(rhs)) == 0; }
  constexpr bool operator!=(const String<N> &rhs) const { return compare(static_cast<std::string_view>(rhs)) != 0; }
  constexpr bool operator<(const String<N> &rhs) const { return compare(static_cast<std::string_view>(rhs)) < 0; }
  constexpr bool operator>(const String<N> &rhs) const { return compare(static_cast<std::string_view>(rhs)) > 0; }
  constexpr bool operator<=(const String<N> &rhs) const { return compare(static_cast<std::string_view>(rhs)) <= 0; }
  constexpr bool operator>=(const String<N> &rhs) const { return compare(static_cast<std::string_view>(rhs)) >= 0; }

  constexpr bool operator==(const std::string_view &rhs) const { return compare(rhs) == 0; }
  constexpr bool operator!=(const std::string_view &rhs) const { return compare(rhs) != 0; }
  constexpr bool operator<(const std::string_view &rhs) const { return compare(rhs) < 0; }
  constexpr bool operator>(const std::string_view &rhs) const { return compare(rhs) > 0; }
  constexpr bool operator<=(const std::string_view &rhs) const { return compare(rhs) <= 0; }
  constexpr bool operator>=(const std::string_view &rhs) const { return compare(rhs) >= 0; }

  constexpr bool operator==(const std::string &rhs) const { return compare(rhs) == 0; }
  constexpr bool operator!=(const std::string &rhs) const { return compare(rhs) != 0; }
  constexpr bool operator<(const std::string &rhs) const { return compare(rhs) < 0; }
  constexpr bool operator>(const std::string &rhs) const { return compare(rhs) > 0; }
  constexpr bool operator<=(const std::string &rhs) const { return compare(rhs) <= 0; }
  constexpr bool operator>=(const std::string &rhs) const { return compare(rhs) >= 0; }

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
inline bool operator==(const String<N> &lhs, const String<N> &rhs) {
  return lhs.operator==()(rhs);
}

// XXX following should be moved elsewhere

struct Source final : public String<MAX_LENGTH_USER> {
  using String<MAX_LENGTH_USER>::String;
};

struct User final : public String<MAX_LENGTH_USER> {
  using String<MAX_LENGTH_USER>::String;
};

struct Account final : public String<MAX_LENGTH_ACCOUNT> {
  using String<MAX_LENGTH_ACCOUNT>::String;
};

struct Exchange final : public String<MAX_LENGTH_EXCHANGE> {
  using String<MAX_LENGTH_EXCHANGE>::String;
};

struct Symbol final : public String<MAX_LENGTH_SYMBOL> {
  using String<MAX_LENGTH_SYMBOL>::String;
};

struct Currency final : public String<MAX_LENGTH_CURRENCY> {
  using String<MAX_LENGTH_CURRENCY>::String;
};

struct OrderTemplate final : public String<MAX_LENGTH_ORDER_TEMPLATE> {
  using String<MAX_LENGTH_ORDER_TEMPLATE>::String;
};

struct ExternalAccount final : public String<MAX_LENGTH_EXTERNAL_ACCOUNT> {
  using String<MAX_LENGTH_EXTERNAL_ACCOUNT>::String;
};

struct ExternalOrderId final : public String<MAX_LENGTH_EXTERNAL_ORDER_ID> {
  using String<MAX_LENGTH_EXTERNAL_ORDER_ID>::String;
};

struct ExternalTradeId final : public String<MAX_LENGTH_EXTERNAL_TRADE_ID> {
  using String<MAX_LENGTH_EXTERNAL_TRADE_ID>::String;
};

struct RoutingId final : public String<MAX_LENGTH_ROUTING_ID> {
  using String<MAX_LENGTH_ROUTING_ID>::String;
};

struct ClOrdId final : public String<MAX_LENGTH_CL_ORD_ID> {
  using String<MAX_LENGTH_CL_ORD_ID>::String;
};

struct RequestId final : public String<MAX_LENGTH_REQUEST_ID> {
  using String<MAX_LENGTH_REQUEST_ID>::String;
};

struct Label final : public String<MAX_LENGTH_LABEL> {
  using String<MAX_LENGTH_LABEL>::String;
};

struct MeasurementKey final : public String<MAX_LENGTH_MEASUREMENT_KEY> {
  using String<MAX_LENGTH_MEASUREMENT_KEY>::String;
};

struct Description final : public String<MAX_LENGTH_DESCRIPTION> {
  using String<MAX_LENGTH_DESCRIPTION>::String;
};

struct TimeZone final : public String<MAX_LENGTH_TIME_ZONE> {
  using String<MAX_LENGTH_TIME_ZONE>::String;
};
}  // namespace roq

template <size_t N>
struct fmt::formatter<roq::String<N> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::String<N> &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
