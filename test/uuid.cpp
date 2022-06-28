/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/uuid.hpp"

using namespace roq;

using namespace std::literals;

using namespace Catch::literals;

TEST_CASE("uuid_endian", "[uuid]") {
  auto raw = absl::MakeUint128(uint64_t{1} << 63, 0);
  UUID uuid{raw};
  REQUIRE(std::size(uuid) == 16);
  CHECK(uuid[0] == 0x80);
  CHECK(uuid[15] == 0x00);
  auto value = static_cast<absl::uint128>(uuid);
  CHECK(value == raw);
}
