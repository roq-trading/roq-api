/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <limits>
#include <stdexcept>

#include "roq/literals.h"

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
  explicit safe_cast(T value) : value_(value) {}

  safe_cast(safe_cast &&) = default;
  safe_cast(const safe_cast &) = delete;

  template <typename R>
  operator R() const {
    if constexpr (is_integer<T>::value) {
      if (!(value_ >= std::numeric_limits<R>::min() && value_ <= std::numeric_limits<R>::max()))
        throw std::overflow_error("overflow"_s);
      return static_cast<R>(value_);
    } else if constexpr (std::is_floating_point<T>::value) {
      if constexpr (is_integer<R>::value && std::is_signed<R>::value) {
        // references:
        //   https://stackoverflow.com/a/30424410
        if (!(value_ > static_cast<double>(std::numeric_limits<R>::min()) &&
              value_ < static_cast<double>(std::numeric_limits<R>::max())))
          throw std::overflow_error("overflow"_s);
        return static_cast<R>(value_);
      } else {
        static_assert(detail::always_false<R>, "not implemented for unsigned");
      }
    } else if constexpr (is_duration<T>::value) {
      return std::chrono::duration_cast<R>(value_);
    } else {
      static_assert(detail::always_false<T>, "not implemented for this type");
    }
  }

 private:
  T value_;
};

}  // namespace utils
}  // namespace roq
