/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

#include <initializer_list>
#include <type_traits>
#include <utility>

namespace roq {

// implements a bitmap mask using an underlying enumeration
// some limitations:
// - flag (enum value) should not be 0 -- we don't currently check
// - flag (enum value) should be powers of two, or combinations -- we don't currently check

template <typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
class Mask final {
 public:
  using value_type = typename std::underlying_type<T>::type;

  struct sentinel final {};

  struct iterator final {
    // std::iterator_traits
    using difference_type = std::ptrdiff_t;
    using value_type = T const;
    using pointer = T const *;
    using reference = T const;
    using iterator_category = std::forward_iterator_tag;

    iterator(Mask<T> value) : value_(value) {}
    bool operator==(sentinel const &) const {
      for (; index_ < magic_enum::enum_count<T>(); ++index_) {
        // special case for "undefined"
        if constexpr (static_cast<value_type>(magic_enum::enum_value<T>(0)) == value_type{}) {
          if (index_ == 0)
            continue;
        }
        if (value_.has(magic_enum::enum_value<T>(index_)))
          return false;
      }
      return true;
    }
    reference operator*() const { return magic_enum::enum_value<T>(index_++); }
    iterator &operator++() { return *this; }
    void operator++(int) { ++(*this); }

   private:
    Mask<T> value_;
    mutable size_t index_ = 0;
  };

  constexpr Mask() = default;

  constexpr explicit Mask(value_type mask) : value_{mask} {}

  constexpr explicit Mask(T flag) : value_{static_cast<value_type>(flag)} {}

  // cppcheck-suppress noExplicitConstructor
  constexpr Mask(std::initializer_list<T> flags) {
    for (auto &flag : flags) {
      // cppcheck-suppress useStlAlgorithm
      value_ |= static_cast<value_type>(flag);
    }
  }

  template <typename... Args>
  constexpr Mask(Mask const &other, Args &&...args) : value_{other.value_ | Mask{std::forward<Args>(args)...}.get()} {}

  constexpr auto operator==(Mask const &rhs) const { return value_ == rhs.value_; }

  constexpr auto operator<=>(Mask const &) const = default;

  constexpr bool empty() const { return value_ == value_type{}; }

  constexpr void reset() { value_ = value_type{}; }

  constexpr value_type get() const { return value_; }

  constexpr bool has(T flag) const { return value_ & static_cast<value_type>(flag); }

  constexpr bool has_any(T flag) const { return (value_ & static_cast<value_type>(flag)) != value_type{}; }

  constexpr bool has_any(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags) {
      // cppcheck-suppress useStlAlgorithm
      value |= static_cast<value_type>(flag);
    }
    return value_ & value;
  }

  constexpr bool has_any(Mask rhs) const { return (value_ & rhs.value_) != value_type{}; }

  constexpr bool has_all(T flag) const {
    return (value_ & static_cast<value_type>(flag)) == static_cast<value_type>(flag);
  }

  constexpr bool has_all(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags) {
      // cppcheck-suppress useStlAlgorithm
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
      // cppcheck-suppress useStlAlgorithm
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
struct std::underlying_type<roq::Mask<T>> {
  using type = typename roq::Mask<T>::value_type;
};

template <typename T>
struct fmt::formatter<roq::Mask<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Mask<T> const &value, Context &context) const {
    using namespace std::literals;
    // special case for "undefined"
    using value_type = typename roq::Mask<T>::value_type;
    if constexpr (static_cast<value_type>(magic_enum::enum_value<T>(0)) == value_type{}) {
      if (value.get() == value_type{})
        return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(magic_enum::enum_value<T>(0)));
    }
    // normal case
    using iterator = typename roq::Mask<T>::iterator;
    using sentinel = typename roq::Mask<T>::sentinel;
    return fmt::format_to(context.out(), "{}"sv, fmt::join(iterator{value}, sentinel{}, "|"sv));
  }
};
