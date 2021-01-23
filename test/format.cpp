/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/connection_status.h"
#include "roq/market_by_price_update.h"
#include "roq/subscribe.h"

using namespace roq;  // NOLINT
using namespace fmt;  // NOLINT

TEST(format, ConnectionStatus) {
  for (uint8_t i = 0; i <= ConnectionStatus::MAX; ++i) {
    ConnectionStatus value(i);
    EXPECT_EQ(format("{}", value), value.name());
  }
}

TEST(format, subscribe) {
  Subscribe subscribe{
      .accounts = {"test", "abc"},
      .symbols_by_exchange = {
          {"abc", {"123", "234"}},
      }};
  auto result = fmt::format("{}", subscribe);
  EXPECT_GT(result.length(), size_t{0});
  EXPECT_EQ(
      result,
      R"({)"
      R"(accounts={"abc", "test"}, )"
      R"(symbols_by_exchange={("abc", {"123", "234"})})"
      R"(})");
}

TEST(format, market_by_price) {
  roq::MBPUpdate bids[] = {
      {.price = 1.0, .quantity = 2.0},
      {.price = 2.0, .quantity = 4.0},
      {.price = 3.0, .quantity = 8.0},
      {.price = 4.0, .quantity = 10.0},
      {.price = 5.0, .quantity = 12.0},
  };
  roq::MBPUpdate asks[] = {
      {.price = 1.0, .quantity = 2.0},
      {.price = 2.0, .quantity = 4.0},
      {.price = 3.0, .quantity = 8.0},
      {.price = 4.0, .quantity = 10.0},
      {.price = 5.0, .quantity = 12.0},
  };
  roq::MarketByPriceUpdate market_by_price{
      .exchange = "deribit",
      .symbol = "BTC-27DEC19",
      .bids =
          {
              .items = bids,
              .length = std::size(bids),
          },
      .asks =
          {
              .items = asks,
              .length = std::size(asks),
          },
      .snapshot = true,
      .exchange_time_utc = {}};
  auto result = fmt::format("{}", market_by_price);
  EXPECT_GT(result.length(), size_t{0});
  EXPECT_EQ(
      result,
      R"({)"
      R"(exchange="deribit", )"
      R"(symbol="BTC-27DEC19", )"
      R"(bids=[{price=1, quantity=2}, {price=2, quantity=4}, {price=3, quantity=8}, {price=4, quantity=10}, {price=5, quantity=12}], )"
      R"(asks=[{price=1, quantity=2}, {price=2, quantity=4}, {price=3, quantity=8}, {price=4, quantity=10}, {price=5, quantity=12}], )"
      R"(snapshot=true, )"
      R"(exchange_time_utc=0ns)"
      R"(})");
}
