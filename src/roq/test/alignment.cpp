/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/api.hpp"

using namespace roq;

TEST_CASE("alignment_layer", "[alignment]") {
  std::array<Layer, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  CHECK(offset == std::ptrdiff_t{32});
}

TEST_CASE("alignment_mbp_update", "[alignment]") {
  std::array<MBPUpdate, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  CHECK(offset == std::ptrdiff_t{32});
}

TEST_CASE("alignment_mbo_update", "[alignment]") {
  std::array<MBOUpdate, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  CHECK(offset == std::ptrdiff_t{64});
}

TEST_CASE("alignment_trade", "[alignment]") {
  std::array<Trade, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  CHECK(offset == std::ptrdiff_t{192});
}

TEST_CASE("alignment_fill", "[alignment]") {
  std::array<Fill, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  CHECK(offset == std::ptrdiff_t{72});
}
