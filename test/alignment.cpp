/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/api.h"

using namespace roq;

TEST(alignment, layer) {
  std::array<Layer, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  EXPECT_EQ(offset, std::ptrdiff_t{32});
}

TEST(alignment, mbp_update) {
  std::array<MBPUpdate, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  EXPECT_EQ(offset, std::ptrdiff_t{32});
}

TEST(alignment, mbo_update) {
  std::array<MBOUpdate, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  EXPECT_EQ(offset, std::ptrdiff_t{64});
}

TEST(alignment, trade) {
  std::array<Trade, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  EXPECT_EQ(offset, std::ptrdiff_t{64});
}

TEST(alignment, fill) {
  std::array<Fill, 2> value;
  auto offset = reinterpret_cast<uint8_t *>(&value[1]) - reinterpret_cast<uint8_t *>(&value[0]);
  EXPECT_EQ(offset, std::ptrdiff_t{64});
}
