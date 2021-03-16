/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <initializer_list>
#include <type_traits>

namespace roq {

// implements a bitmap mask using an underlying enumeration
// some limitations:
// - flag (enum value) should not be 0 -- we don't currently check
// - flag (enum value) should be powers of two, or combinations -- we don't currently check

template <typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
class Mask final {
 public:
  using value_type = typename std::underlying_type<T>::type;

  constexpr Mask() = default;

  constexpr explicit Mask(value_type mask) : value_{mask} {}

  constexpr explicit Mask(T flag) : value_{static_cast<value_type>(flag)} {}

  constexpr Mask(std::initializer_list<T> flags) {
    for (auto &flag : flags)
      value_ |= static_cast<value_type>(flag);
  }

  constexpr bool operator==(const Mask &rhs) const { return value_ == rhs.value_; }

  constexpr bool operator!=(const Mask &rhs) const { return value_ != rhs.value_; }

  constexpr value_type get() const { return value_; }

  constexpr bool has(T flag) const { return value_ & static_cast<value_type>(flag); }

  constexpr bool has_any(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags)
      value |= static_cast<value_type>(flag);
    return value_ & value;
  }

  constexpr bool has_all(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags)
      value |= static_cast<value_type>(flag);
    return (value_ & value) == value;
  }

  constexpr Mask &set(T flag) {
    value_ |= static_cast<value_type>(flag);
    return *this;
  }
  constexpr Mask &remove(T flag) {
    value_ &= ~static_cast<value_type>(flag);
    return *this;
  }

 private:
  value_type value_ = {};
};

}  // namespace roq
