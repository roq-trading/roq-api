/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <cctype>
#include <span>

namespace roq {
namespace debug {
namespace fix {

struct Message final {
 public:
  explicit Message(std::span<std::byte const> const &buffer) : buffer_{buffer} {}
  explicit Message(std::string_view const &buffer)
      : buffer_{reinterpret_cast<std::byte const *>(std::data(buffer)), std::size(buffer)} {}

  Message(std::byte const *data, size_t length) : Message{{data, length}} {}

  auto format_to(auto &context) const {
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
  std::span<std::byte const> const buffer_;
};

}  // namespace fix
}  // namespace debug
}  // namespace roq

template <>
struct fmt::formatter<roq::debug::fix::Message> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::debug::fix::Message const &value, format_context &context) const { return value.format_to(context); }
};
