/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include "roq/utils/math.hpp"

using namespace roq::utils;

TEST_CASE("math_power_of_two", "[math]") {
  for (auto i = 0; i < 63; ++i) {
    CHECK(power_of_two(1 << i) == true);
  }
  CHECK(power_of_two(3) == false);
  CHECK(power_of_two(5) == false);
  CHECK(power_of_two(6) == false);
  CHECK(power_of_two(7) == false);
}

TEST_CASE("math_round_up_pow_2", "[math]") {
  CHECK(round_up<4096>(1) == 4096);
  CHECK(round_up<4096>(4095) == 4096);
  CHECK(round_up<4096>(4096) == 4096);
  CHECK(round_up<4096>(4097) == 8192);
  CHECK(round_up<4096>(8191) == 8192);
  CHECK(round_up<4096>(8192) == 8192);
}

TEST_CASE("math_round_up", "[math]") {
  CHECK(round_up<4095>(1) == 4095);
  CHECK(round_up<4095>(4094) == 4095);
  CHECK(round_up<4095>(4095) == 4095);
  CHECK(round_up<4095>(4096) == 8190);
  CHECK(round_up<4095>(8189) == 8190);
  CHECK(round_up<4095>(8190) == 8190);
}

TEST_CASE("math_round_down_pow_2", "[math]") {
  CHECK(round_down<4096>(1) == 0);
  CHECK(round_down<4096>(4095) == 0);
  CHECK(round_down<4096>(4096) == 4096);
  CHECK(round_down<4096>(4097) == 4096);
  CHECK(round_down<4096>(8191) == 4096);
  CHECK(round_down<4096>(8192) == 8192);
}

TEST_CASE("math_round_down", "[math]") {
  CHECK(round_down<4095>(1) == 0);
  CHECK(round_down<4095>(4094) == 0);
  CHECK(round_down<4095>(4095) == 4095);
  CHECK(round_down<4095>(4096) == 4095);
  CHECK(round_down<4095>(8189) == 4095);
  CHECK(round_down<4095>(8190) == 8190);
}

TEST_CASE("math_round_up_next_pow_2", "[math]") {
  CHECK(round_up_next_pow_2(uint32_t{0}) == uint32_t{0});
  CHECK(round_up_next_pow_2(uint32_t{1}) == uint32_t{1});
  CHECK(round_up_next_pow_2(uint32_t{2}) == uint32_t{2});
  CHECK(round_up_next_pow_2(uint32_t{3}) == uint32_t{4});
  CHECK(round_up_next_pow_2(uint32_t{4}) == uint32_t{4});
  CHECK(round_up_next_pow_2(uint32_t{5}) == uint32_t{8});
  CHECK(round_up_next_pow_2(uint32_t{7}) == uint32_t{8});
  CHECK(round_up_next_pow_2(uint32_t{8}) == uint32_t{8});
  CHECK(round_up_next_pow_2(uint32_t{9}) == uint32_t{16});
}
