/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/api.h"
#include "roq/platform.h"

using namespace roq;  // NOLINT

TEST(alignment, mbp_update) {
  MBPUpdate update_1[2];
  auto offset_1 = reinterpret_cast<uint8_t *>(&update_1[1]) -
    reinterpret_cast<uint8_t *>(&update_1[0]);
  EXPECT_EQ(offset_1, static_cast<int>(cache_line_size()));
}
