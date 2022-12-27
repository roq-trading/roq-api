/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <cctype>
#include <span>

namespace roq {
namespace debug {
namespace fix {

struct Message final {
 public:
  explicit Message(std::span<const std::byte> const &buffer) : buffer_{buffer} {}

  Message(std::byte const *data, size_t length) : Message{{data, length}} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    for (auto b : buffer_) {
      if (b == std::byte{0x1}) {
        fmt::format_to(context.out(), "|"_cf);
      } else {
        char c = static_cast<char>(b);
        if (::isprint(c))
          fmt::format_to(context.out(), "{}"_cf, c);
        else
          fmt::format_to(context.out(), "."_cf);
      }
    }
    return context.out();
  }

 private:
  std::span<const std::byte> const buffer_;
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
  auto format(roq::debug::fix::Message const &value, Context &context) const {
    return value.format_to(context);
  }
};
