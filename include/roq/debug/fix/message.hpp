/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <cctype>

#include "roq/span.hpp"

namespace roq {
namespace debug {
namespace fix {

struct Message final {
 public:
  explicit Message(const std::span<const std::byte> &buffer) : buffer_(buffer) {}

  Message(std::byte const *data, size_t length) : Message{{data, length}} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    for (auto b : buffer_) {
      if (b == std::byte{0x1}) {
        fmt::format_to(context.out(), "|"sv);
      } else {
        char c = static_cast<char>(b);
        if (::isprint(c))
          fmt::format_to(context.out(), "{}"sv, c);
        else
          fmt::format_to(context.out(), "."sv);
      }
    }
    return context.out();
  }

 private:
  const std::span<const std::byte> buffer_;
};

}  // namespace fix
}  // namespace debug
}  // namespace roq

template <>
struct fmt::formatter<roq::debug::fix::Message> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::debug::fix::Message &value, Context &context) {
    return value.format_to(context);
  }
};
