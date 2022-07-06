/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/numeric/int128.h>

#include <absl/base/internal/endian.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <compare>
#include <utility>

namespace roq {

class UUID final {
 public:
  using value_type = unsigned char;
  using uuid_t = std::array<value_type, 16>;

  UUID() { std::fill(std::begin(uuid_), std::end(uuid_), 0); }

  /*
  // SO 6893700
  template <typename... T>
  UUID(T &&... args) : uuid_{{std::forward<T>(args)...}} {}
  */

  explicit UUID(uuid_t &&value) : uuid_(std::move(value)) {}
  explicit UUID(uuid_t const &value) : uuid_(value) {}
  explicit UUID(absl::uint128 const value) {
    auto high = absl::big_endian::FromHost(absl::Uint128High64(value));
    auto low = absl::big_endian::FromHost(absl::Uint128Low64(value));
    static_assert(sizeof(uuid_t) == (sizeof(high) + sizeof(low)));
    std::memcpy(&uuid_[0], &high, sizeof(high));
    std::memcpy(&uuid_[sizeof(high)], &low, sizeof(low));
  }

  UUID(const UUID &) = default;
  UUID(UUID &&) = default;

  UUID &operator=(const UUID &) = default;

  constexpr size_t size() const { return std::size(uuid_); }

  constexpr value_type const *data() const { return std::data(uuid_); }

  constexpr value_type &operator[](std::size_t index) { return uuid_[index]; }
  constexpr value_type const &operator[](std::size_t index) const { return uuid_[index]; }

  constexpr operator uuid_t const &() const { return uuid_; }

  operator absl::uint128() const {
    uint64_t high, low;
    static_assert(sizeof(uuid_) == (sizeof(high) + sizeof(low)));
    std::memcpy(&high, &uuid_[0], sizeof(high));
    std::memcpy(&low, &uuid_[sizeof(high)], sizeof(low));
    return absl::MakeUint128(absl::big_endian::ToHost(high), absl::big_endian::ToHost(low));
  }

  constexpr bool empty() const {
    return std::all_of(std::begin(uuid_), std::end(uuid_), [](auto v) { return v == 0; });
  }

#if defined(__clang__)
  // note! clang13 does not yet support spaceship operator for std::array
  // https://libcxx.llvm.org/Status/Spaceship.html
  constexpr bool operator==(const UUID &rhs) const {
    for (size_t i = 0; i < sizeof(uuid_t); ++i) {
      if (uuid_[i] != rhs.uuid_[i])
        return false;
    }
    return true;
  }
  constexpr auto operator<=>(const UUID &rhs) const {
    for (size_t i = 0; i < sizeof(uuid_t); ++i) {
      int diff = uuid_[i] - rhs.uuid_[i];
      if (diff == 0)
        continue;
      if (diff < 0)
        return std::strong_ordering::less;
      return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
  }
#else
  constexpr auto operator<=>(const UUID &) const = default;
#endif

  template <typename H>
  friend H AbslHashValue(H hash, UUID const &rhs) {
    uint64_t high, low;
    static_assert(sizeof(rhs.uuid_) == (sizeof(high) + sizeof(low)));
    std::memcpy(&high, &rhs.uuid_[0], sizeof(high));
    std::memcpy(&low, &rhs.uuid_[sizeof(high)], sizeof(low));
    // note! endianness is not important here
    auto value = absl::MakeUint128(high, low);
    return H::combine(std::move(hash), value);
  }

 private:
  uuid_t uuid_;
};

static_assert(sizeof(UUID) == 16);

}  // namespace roq

template <>
struct fmt::formatter<roq::UUID> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::UUID const &value, Context &context) const {
    using namespace std::literals;
    auto data = std::data(value);
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
