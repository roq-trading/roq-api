/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <utility>

namespace roq {

class UUID final {
 public:
  using value_type = unsigned char;
  using uuid_t = std::array<value_type, 16>;

  UUID() { uuid_.fill(0); }

  /*
  // SO 6893700
  template <typename... T>
  UUID(T &&... args) : uuid_{{std::forward<T>(args)...}} {}
  */

  explicit UUID(uuid_t &&value) : uuid_(std::move(value)) {}
  explicit UUID(const uuid_t &value) : uuid_(value) {}

  UUID(const UUID &) = default;
  UUID(UUID &&) = default;

  UUID &operator=(const UUID &) = default;

  constexpr size_t size() const { return std::size(uuid_); }

  constexpr value_type const *data() const { return std::data(uuid_); }

  constexpr value_type &operator[](std::size_t index) { return uuid_[index]; }
  constexpr const value_type &operator[](std::size_t index) const { return uuid_[index]; }

  constexpr operator const uuid_t &() const { return uuid_; }

  constexpr bool is_null() const {
    // XXX not constexpr until c++20
    // return std::all_of(std::begin(uuid_), std::end(uuid_), [](char c) { c == 0; });
    // note! compilers can easily optimize this
    return uuid_[0] == 0 && uuid_[1] == 0 && uuid_[2] == 0 && uuid_[3] == 0 && uuid_[4] == 0 && uuid_[5] == 0 &&
           uuid_[6] == 0 && uuid_[7] == 0 && uuid_[8] == 0 && uuid_[9] == 0 && uuid_[10] == 0 && uuid_[11] == 0 &&
           uuid_[12] == 0 && uuid_[13] == 0 && uuid_[14] == 0 && uuid_[15] == 0;
  }

  // XXX c++20 spaceship
  constexpr bool operator==(const UUID &rhs) const { return compare(rhs) == 0; }
  constexpr bool operator!=(const UUID &rhs) const { return compare(rhs) != 0; }
  constexpr bool operator<(const UUID &rhs) const { return compare(rhs) < 0; }
  constexpr bool operator<=(const UUID &rhs) const { return compare(rhs) <= 0; }
  constexpr bool operator>(const UUID &rhs) const { return compare(rhs) > 0; }
  constexpr bool operator>=(const UUID &rhs) const { return compare(rhs) >= 0; }

 protected:
  constexpr int compare(const UUID &rhs) const {
    // XXX no constexpr algos until c++20
    // note! compilers can easily optimize this
    for (size_t i = 0; i < std::size(uuid_); ++i) {
      if (uuid_[i] != rhs.uuid_[i]) {
        return uuid_[i] < rhs.uuid_[i] ? -1 : 1;
      }
    }
    return 0;
  }

 private:
  uuid_t uuid_;
};

static_assert(sizeof(UUID) == 16);

}  // namespace roq

template <>
struct fmt::formatter<roq::UUID> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::UUID &value, Context &ctx) {
    using namespace std::literals;
    auto data = std::data(value);
    return fmt::format_to(
        ctx.out(),
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
