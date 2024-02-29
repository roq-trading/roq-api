/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <bit>

namespace roq {

// note!
//   interface using native byte order
//   network byte order for internal representation

struct UUID final {
  using value_type = __uint128_t;

  UUID() {}

  UUID(UUID const &rhs) : value_{rhs.value_} {}

  UUID &operator=(UUID const &rhs) {
    value_ = rhs.value_;
    return *this;
  }

  explicit UUID(value_type value) : value_{create(value)} {}

  UUID(uint64_t high, uint64_t low) : value_{create(high, low)} {}

  constexpr auto operator<=>(UUID const &) const = default;

  constexpr operator value_type() const {
    if constexpr (std::endian::native == std::endian::little) {
      return std::byteswap(value_);
    }
    return value_;
  }

  constexpr operator std::pair<uint64_t, uint64_t>() const {
    if constexpr (std::endian::native == std::endian::little) {
      // note! legacy (testing purposes) -- inconsistent with uint128_t constructor
      auto high = static_cast<uint64_t>(value_);
      auto low = static_cast<uint64_t>(value_ >> 64);
      return {high, low};
    } else {
      auto high = static_cast<uint64_t>(value_ >> 64);
      auto low = static_cast<uint64_t>(value_);
      return {high, low};
    }
  }

  constexpr value_type *data() { return &value_; }
  constexpr value_type const *data() const { return &value_; }

  constexpr size_t size() const { return sizeof(value_); }

  constexpr bool empty() const { return value_ == value_type{}; }

 protected:
  static value_type create(value_type value) {
    if constexpr (std::endian::native == std::endian::little) {
      return std::byteswap(value);
    }
    return value;
  }

  static value_type create(uint64_t high, uint64_t low) {
    if constexpr (std::endian::native == std::endian::little) {
      // note! legacy (testing purposes) -- inconsistent with uint128_t constructor
      return static_cast<value_type>(low) << 64 | static_cast<value_type>(high);
    }
    return static_cast<value_type>(high) << 64 | static_cast<value_type>(low);
  }

 private:
  value_type value_ = {};
};

static_assert(sizeof(UUID) == 16);

}  // namespace roq

template <>
struct fmt::formatter<roq::UUID> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::UUID const &value, format_context &context) const {
    using namespace std::literals;
    auto data = reinterpret_cast<std::byte const *>(std::data(value));
    return fmt::format_to(
        context.out(),
        "{:02x}{:02x}{:02x}{:02x}-"
        "{:02x}{:02x}-"
        "{:02x}{:02x}-"
        "{:02x}{:02x}-"
        "{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}"sv,
        data[0],
        data[1],
        data[2],
        data[3],
        data[4],
        data[5],
        data[6],
        data[7],
        data[8],
        data[9],
        data[10],
        data[11],
        data[12],
        data[13],
        data[14],
        data[15]);
  }
};
