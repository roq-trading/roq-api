/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <limits>
#include <stdexcept>
#include <string>

#include "roq/exceptions.hpp"

#include "roq/utils/traits.hpp"

namespace roq {
namespace utils {

template <typename T>
struct safe_cast final {
  using value_type = typename std::remove_reference<T>::type;

  explicit safe_cast(T value) : value_{value} {}

  safe_cast(safe_cast &&) = default;
  safe_cast(safe_cast const &) = delete;

  template <typename R>
  operator R() const {
    using namespace std::literals;
    using result_type = typename std::remove_reference<R>::type;
    if constexpr (std::is_same<result_type, value_type>::value) {
      // same
      return value_;
    } else if constexpr (is_integer<value_type>::value) {
      // integer to ...
      if (!(value_ >= std::numeric_limits<result_type>::lowest() && value_ <= std::numeric_limits<result_type>::max()))
        throw OverflowError{"overflow: value={}"sv, value_};
      return static_cast<result_type>(value_);
    } else if constexpr (std::is_floating_point<value_type>::value) {
      // floating point to ...
      if constexpr (is_integer<result_type>::value && std::is_signed<result_type>::value) {
        // ... signed integer
        // references:
        //   https://stackoverflow.com/a/30424410
        if (!(value_ > static_cast<double>(std::numeric_limits<result_type>::lowest()) &&
              value_ < static_cast<double>(std::numeric_limits<result_type>::max())))
          throw OverflowError{"overflow: value={}"sv, value_};
        return static_cast<result_type>(value_);
      } else {
        static_assert(always_false<result_type>, "not implemented for unsigned");
      }
    } else if constexpr (is_duration<value_type>::value) {
      return std::chrono::duration_cast<result_type>(value_);
    } else if constexpr (std::is_same<result_type, std::string>::value) {
      // ... to string
      return std::string{value_};
    } else {
      static_assert(always_false<result_type>, "not implemented for this type");
    }
  }

 private:
  value_type value_;
};

}  // namespace utils
}  // namespace roq
