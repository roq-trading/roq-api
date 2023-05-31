/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <fmt/format.h>

#include "roq/utils/message.hpp"

using namespace roq;
using namespace roq::utils;

using namespace std::literals;

TEST_CASE("simple_1", "[message]") {
  std::vector<std::byte> buffer(4096);
  Message<char> message{buffer};
  fmt::format_to(std::back_inserter(message), "{}"sv, "test"sv);
}

TEST_CASE("simple_2", "[message]") {
  std::vector<char> buffer(4096);
  Message<char> message{buffer};
  fmt::format_to(std::back_inserter(message), "{}"sv, "test"sv);
}
