/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/api.hpp"

using namespace roq;
using namespace std::literals;

TEST_CASE("utils_trade_id", "[utils]") {
  Trade trade = {};
  auto &trade_id = trade.trade_id;
  std::string_view text_1;
  CHECK(trade_id.empty() == true);
  CHECK(trade_id.length() == 0);
  CHECK(static_cast<std::string_view>(trade_id) == text_1);
  // small
  std::string_view text_2{"123"sv};
  trade_id = text_2;
  CHECK(trade_id.empty() == false);
  CHECK(trade_id.length() == text_2.length());
  CHECK(static_cast<std::string_view>(trade_id) == text_2);
  // full
  std::string_view text_3{
      "1234567890"
      "1234567890"
      "1234567890"
      "123456789"sv};
  trade_id = text_3;
  CHECK(trade_id.empty() == false);
  CHECK(trade_id.length() == text_3.length());
  CHECK(static_cast<std::string_view>(trade_id) == text_3);
}
