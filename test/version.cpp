/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/version.hpp"

using namespace std::literals;

using namespace roq;

TEST_CASE("simple", "[version]") {
  CHECK(Version{1, 2, 3} == Version{1, 2, 3});
  CHECK(Version{1, 2, 3} <= Version{1, 2, 3});
  CHECK(Version{1, 2, 3} >= Version{1, 2, 3});
  CHECK(Version{} < Version{1, 2, 3});
  CHECK(Version{} <= Version{1, 2, 3});
  CHECK(Version{} != Version{1, 2, 3});
  CHECK(Version{1, 2, 3} > Version{});
  CHECK(Version{1, 2, 3} >= Version{});
  CHECK(Version{1, 2, 3} != Version{});
}
