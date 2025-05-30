/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/side.hpp"

using namespace std::literals;

using namespace roq;

TEST_CASE("side_simple", "[side]") {
  CHECK(fmt::format("{}"sv, Side::BUY) == "BUY"sv);
  CHECK(fmt::format("{}"sv, Side::SELL) == "SELL"sv);
}
