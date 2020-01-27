/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <cstring>

#include "roq/api.h"

using namespace roq;  // NOLINT

TEST(utils, trade_id) {
  Trade trade = {};
  // empty
  std::memset(
      trade.trade_id,
      0,
      sizeof(trade.trade_id));
  auto test_1 = get_trade_id_as_string_view(trade);
  EXPECT_EQ(test_1.length(), size_t{0});
  EXPECT_EQ(test_1, std::string_view());
  // small
  std::memset(
      trade.trade_id,
      0,
      sizeof(trade.trade_id));
  constexpr std::string_view text_2("123");
  text_2.copy(
      trade.trade_id,
      sizeof(trade.trade_id));
  auto test_2 = get_trade_id_as_string_view(trade);
  EXPECT_EQ(test_2.length(), text_2.length());
  EXPECT_EQ(test_2, text_2);
  // full
  std::memset(
      trade.trade_id,
      0,
      sizeof(trade.trade_id));
  constexpr std::string_view text_3(
      "0123456789"
      "0123456789"
      "0123456789"
      "0123456789");
  static_assert(text_3.length() == sizeof(trade.trade_id));
  text_3.copy(
      trade.trade_id,
      sizeof(trade.trade_id));
  auto test_3 = get_trade_id_as_string_view(trade);
  EXPECT_EQ(test_3.length(), text_3.length());
  EXPECT_EQ(test_3, text_3);
}
