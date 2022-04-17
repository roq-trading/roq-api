/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <span>

namespace roq {
namespace debug {

class HexEscaped final {
 public:
  explicit HexEscaped(const std::span<const std::byte> &buffer) : buffer_(buffer) {}
  HexEscaped(std::byte const *data, size_t length) : HexEscaped{{data, length}} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    for (auto b : buffer_)
      fmt::format_to(context.out(), R"(\x{:02x})"sv, b);
    return context.out();
  }

 private:
  const std::span<const std::byte> buffer_;
};

}  // namespace debug
}  // namespace roq

template <>
struct fmt::formatter<roq::debug::HexEscaped> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::debug::HexEscaped &value, Context &context) {
    return value.format_to(context);
  }
};
