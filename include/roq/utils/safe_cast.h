/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <limits>
#include <stdexcept>
#include <string>

#include "roq/exceptions.h"

#include "roq/utils/traits.h"

namespace roq {
namespace utils {

namespace detail {
// references:
//   https://stackoverflow.com/a/53945549
template <class...>
constexpr std::false_type always_false{};
}  // namespace detail

template <typename T>
struct safe_cast final {
  using value_type = typename std::remove_reference<T>::type;

  explicit safe_cast(T value) : value_(value) {}

  safe_cast(safe_cast &&) = default;
  safe_cast(const safe_cast &) = delete;

  template <typename R>
  operator R() const {
    using namespace std::literals;
    using result_type = typename std::remove_reference<R>::type;
    if constexpr (std::is_same<result_type, value_type>::value) {
      // same
      return value_;
    } else if constexpr (is_integer<value_type>::value) {
      // integer to ...
      if (!(value_ >= std::numeric_limits<result_type>::min() && value_ <= std::numeric_limits<result_type>::max()))
        throw OverflowErrorException("overflow: value={}"sv, value_);
      return static_cast<result_type>(value_);
    } else if constexpr (std::is_floating_point<value_type>::value) {
      // floating point to ...
      if constexpr (is_integer<result_type>::value && std::is_signed<result_type>::value) {
        // ... signed integer
        // references:
        //   https://stackoverflow.com/a/30424410
        if (!(value_ > static_cast<double>(std::numeric_limits<result_type>::min()) &&
              value_ < static_cast<double>(std::numeric_limits<result_type>::max())))
          throw OverflowErrorException("overflow: value={}"sv, value_);
        return static_cast<result_type>(value_);
      } else {
        static_assert(detail::always_false<result_type>, "not implemented for unsigned");
      }
    } else if constexpr (is_duration<value_type>::value) {
      return std::chrono::duration_cast<result_type>(value_);
    } else if constexpr (std::is_same<result_type, std::string>::value) {
      // ... to string
      return std::string{value_};
    } else {
      static_assert(detail::always_false<result_type>, "not implemented for this type");
    }
  }

 private:
  value_type value_;
};

}  // namespace utils
}  // namespace roq
