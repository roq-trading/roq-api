/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <span>

namespace roq {
namespace debug {
namespace hex {

struct Message final {
  explicit Message(std::span<std::byte const> const &buffer) : buffer_{buffer} {}
  explicit Message(std::string_view const &buffer)
      : buffer_{reinterpret_cast<std::byte const *>(std::data(buffer)), std::size(buffer)} {}

  Message(std::byte const *data, size_t length) : Message{{data, length}} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    for (auto b : buffer_)
      fmt::format_to(context.out(), R"(\x{:02x})"sv, static_cast<uint8_t>(b));
    return context.out();
  }

 private:
  std::span<std::byte const> const buffer_;
};

}  // namespace hex
}  // namespace debug
}  // namespace roq

template <>
struct fmt::formatter<roq::debug::hex::Message> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::debug::hex::Message const &value, format_context &context) const { return value.format_to(context); }
};
