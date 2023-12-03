/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/debug/hex/message.hpp"

using namespace std::literals;
using namespace fmt::literals;

using namespace roq;

namespace {
// note!
//   https://stackoverflow.com/a/45172360
template <typename... Ts>
std::array<std::byte, sizeof...(Ts)> make_bytes(Ts &&...args) noexcept {
  return {{std::byte(std::forward<Ts>(args))...}};
}
}  // namespace

TEST_CASE("hex_escaped", "[debug]") {
  {
    std::array<std::byte, 0> buffer;
    auto s = fmt::format("{}"_cf, debug::hex::Message{buffer});
    CHECK(s == ""sv);
  }
  {
    auto buffer = make_bytes(0x0, 0x1, 0xfe, 0xff);
    auto s = fmt::format("{}"_cf, debug::hex::Message{buffer});
    CHECK(s == R"(\x00\x01\xfe\xff)"sv);
  }
}
