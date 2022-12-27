/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <span>

namespace roq {
namespace debug {
namespace hex {

struct Message final {
  explicit Message(std::span<const std::byte> const &buffer) : buffer_{buffer} {}

  Message(std::byte const *data, size_t length) : Message{{data, length}} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    for (auto b : buffer_)
      fmt::format_to(context.out(), R"(\x{:02x})"_cf, static_cast<uint8_t>(b));
    return context.out();
  }

 private:
  std::span<const std::byte> const buffer_;
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
