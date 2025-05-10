/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_flags.hpp>

#include <initializer_list>
#include <limits>
#include <ranges>
#include <type_traits>
#include <utility>

namespace roq {

// implements a bitmap mask using an underlying enumeration
// some limitations:
// - flag (enum value) should not be 0 -- we don't currently check
// - flag (enum value) should be powers of two, or combinations -- we don't currently check

template <typename T>
  requires std::is_enum_v<T>
struct Mask final {
  using value_type = typename std::underlying_type_t<T>;

  struct sentinel final {};

  struct iterator final {
    // std::iterator_traits
    using difference_type = std::ptrdiff_t;
    using value_type = T const;
    using pointer = T const *;
    using reference = T const;
    using iterator_category = std::forward_iterator_tag;

    using underlying_type = std::underlying_type_t<T>;

    iterator(Mask<T> value) : value_{value.get()} {}

    bool operator==(sentinel const &) const {
      for (; bit_ < std::numeric_limits<underlying_type>::digits; ++bit_) {
        auto mask = static_cast<underlying_type>(size_t{1} << bit_);
        if (value_ < mask) {
          break;
        }
        if (value_ & mask) {
          return false;
        }
      }
      return true;
    }

    reference operator*() const {
      auto mask = static_cast<underlying_type>(size_t{1} << bit_++);
      return static_cast<value_type>(mask);
    }

    iterator &operator++() { return *this; }
    void operator++(int) { ++(*this); }

   private:
    underlying_type value_;
    mutable size_t bit_ = 0;  // skip undefined
  };

  constexpr Mask() = default;

  constexpr explicit Mask(value_type mask) : value_{mask} {}

  constexpr explicit Mask(T flag) : value_{static_cast<value_type>(flag)} {}

  constexpr Mask(std::initializer_list<T> flags) {
    for (auto &flag : flags) {
      value_ |= static_cast<value_type>(flag);
    }
  }

  template <typename... Args>
  constexpr Mask(Mask const &other, Args &&...args) : value_{other.value_ | Mask{std::forward<Args>(args)...}.get()} {}

  constexpr auto operator<=>(Mask const &) const = default;

  constexpr bool empty() const { return value_ == value_type{}; }

  constexpr void reset() { value_ = value_type{}; }

  constexpr value_type get() const { return value_; }

  constexpr bool is_same(T flag) const { return value_ == static_cast<value_type>(flag); }

  constexpr bool has(T flag) const { return value_ & static_cast<value_type>(flag); }

  constexpr bool has_any(T flag) const { return (value_ & static_cast<value_type>(flag)) != value_type{}; }

  constexpr bool has_any(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags) {
      value |= static_cast<value_type>(flag);
    }
    return value_ & value;
  }

  constexpr bool has_any(Mask rhs) const { return (value_ & rhs.value_) != value_type{}; }

  constexpr bool has_all(T flag) const { return (value_ & static_cast<value_type>(flag)) == static_cast<value_type>(flag); }

  constexpr bool has_all(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags) {
      value |= static_cast<value_type>(flag);
    }
    return (value_ & value) == value;
  }

  constexpr bool has_all(Mask rhs) const { return (value_ & rhs.value_) == rhs.value_; }

  constexpr Mask &set(T flag) {
    value_ |= static_cast<value_type>(flag);
    return *this;
  }

  constexpr Mask &set(Mask rhs) {
    value_ |= rhs.value_;
    return *this;
  }

  constexpr Mask &remove(T flag) {
    value_ &= ~static_cast<value_type>(flag);
    return *this;
  }

  constexpr Mask &remove(Mask rhs) {
    value_ &= ~rhs.value_;
    return *this;
  }

  constexpr bool has_none(T flag) const { return !has_any(flag); }

  constexpr bool has_none(std::initializer_list<T> flags) const { return !has_any(flags); }

  constexpr bool has_none(Mask rhs) const { return !has_any(rhs); }

  constexpr Mask negate() const { return Mask{~value_}; }

  constexpr Mask logical_and(Mask rhs) const { return Mask{value_ & rhs.value_}; }

  constexpr Mask &set(std::initializer_list<T> flags) {
    for (auto &flag : flags) {
      value_ |= static_cast<value_type>(flag);
    }
    return *this;
  }

  constexpr Mask operator~() const { return negate(); }

  constexpr Mask operator&(Mask rhs) const { return logical_and(rhs); }

  constexpr Mask &operator|=(T flag) {
    set(flag);
    return *this;
  }

  constexpr Mask &operator|=(Mask rhs) {
    set(rhs);
    return *this;
  }

 private:
  value_type value_ = {};
};

}  // namespace roq

template <typename T>
struct fmt::formatter<roq::Mask<T>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Mask<T> const &value, format_context &context) const {
    using namespace std::literals;
    using iterator = typename roq::Mask<T>::iterator;
    using sentinel = typename roq::Mask<T>::sentinel;
#if (true)
    return fmt::format_to(context.out(), "{}"sv, fmt::join(iterator{value}, sentinel{}, "|"sv));
#else
    // XXX FIXME doesn't seem to work with ranges
    auto helper = [](auto v) -> std::string_view { return magic_enum::enum_flags_name(v); };
    return fmt::format_to(context.out(), "{}"sv, fmt::join(std::ranges::views::transform(std::ranges::subrange(iterator{value}, sentinel{}), helper), "|"sv));
#endif
  }
};
