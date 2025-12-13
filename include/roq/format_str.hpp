/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <array>
#include <source_location>
#include <string_view>

// capture source location

namespace roq {

namespace detail {
// compile-time evaluated string
template <std::size_t N>
struct static_string final {
  consteval static_string(std::string_view const &str) : length_{std::min(N, std::size(str))}, buffer_{create(str, length_)} {}

  static_string(static_string &&) = delete;
  static_string(static_string const &) = default;

  constexpr operator std::string_view() const { return {std::data(buffer_), length_}; }

 protected:
  static consteval auto create(std::string_view const &str, std::size_t length) {
    static_assert(N > 0);
    std::array<char, N> buffer;
    for (std::size_t i = 0; i < length; ++i) {
      buffer[i] = str[i];
    }
    for (std::size_t i = length; i < N; ++i) {
      buffer[i] = '\0';
    }
    return buffer;
  }

 private:
  std::size_t const length_;
  std::array<char, N> const buffer_;
};
}  // namespace detail

struct format_str final {
  template <typename T>
  consteval format_str(T const &str, std::source_location const loc = std::source_location::current())
      : str{str}, file_name{extract_basename(loc.file_name())}, line{loc.line()} {}

  // note! string could be truncated
  using file_name_type = detail::static_string<32>;  // NOLINT(readability-magic-numbers)

  std::string_view const str;
  file_name_type const file_name;
  std::uint32_t const line;

 private:
  static consteval std::string_view extract_basename(char const *path) {
    if (path == nullptr) {
      return {};
    }
    std::string_view tmp{path};
    if (std::empty(tmp)) {
      return tmp;
    }
    auto pos = tmp.find_last_of('/');
    if (pos == std::string_view::npos || pos == (std::size(tmp) - 1)) {
      return tmp;
    }
    return tmp.substr(++pos);
  }
};
}  // namespace roq

template <size_t N>
struct fmt::formatter<roq::detail::static_string<N>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::detail::static_string<N> const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
