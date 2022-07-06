/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/span.hpp"

namespace roq {
namespace debug {
namespace hex {

struct Message final {
  explicit Message(std::span<const std::byte> const &buffer) : buffer_(buffer) {}

  Message(std::byte const *data, size_t length) : Message{{data, length}} {}

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

}  // namespace hex
}  // namespace debug
}  // namespace roq

template <>
struct fmt::formatter<roq::debug::hex::Message> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::debug::hex::Message const &value, Context &context) const {
    return value.format_to(context);
  }
};
