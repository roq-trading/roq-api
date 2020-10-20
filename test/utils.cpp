/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <cstring>

#include "roq/api.h"

using namespace roq;  // NOLINT

TEST(utils, trade_id) {
  Trade trade = {};
  auto &trade_id = trade.trade_id;
  std::string_view text_1;
  EXPECT_TRUE(trade_id.empty());
  EXPECT_EQ(trade_id.length(), size_t {});
  EXPECT_EQ(static_cast<std::string_view>(trade_id), text_1);
  // small
  std::string_view text_2("123");
  trade_id = text_2;
  EXPECT_FALSE(trade_id.empty());
  EXPECT_EQ(trade_id.length(), text_2.length());
  EXPECT_EQ(static_cast<std::string_view>(trade_id), text_2);
  // full
  std::string_view text_3(
      "1234567890"
      "1234567890"
      "1234567890"
      "123456789");
  trade_id = text_3;
  EXPECT_FALSE(trade_id.empty());
  EXPECT_EQ(trade_id.length(), text_3.length());
  EXPECT_EQ(static_cast<std::string_view>(trade_id), text_3);
}
