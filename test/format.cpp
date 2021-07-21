/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/market_by_price_update.h"
#include "roq/side.h"
#include "roq/subscribe.h"

using namespace roq;
using namespace roq::literals;

TEST(format, Side) {
  EXPECT_EQ(format("{}"_sv, Side{Side::UNDEFINED}), "UNDEFINED"_sv);
  EXPECT_EQ(format("{}"_sv, Side{Side::BUY}), "BUY"_sv);
  EXPECT_EQ(format("{}"_sv, Side{Side::SELL}), "SELL"_sv);
}

TEST(format, subscribe) {
  Subscribe subscribe{
      .accounts = {"test"_s, "abc"_s},
      .symbols_by_exchange =
          {
              {"abc"_s, {"123"_s, "234"_s}},
          },
      .order_cancel_policy = OrderCancelPolicy::MANAGED_ORDERS,
      .order_management = OrderManagement::FIX,
  };
  auto result = format("{}"_sv, subscribe);
  EXPECT_GT(result.length(), size_t{0});
  EXPECT_EQ(
      result,
      R"({)"
      R"(accounts=["abc", "test"], )"
      R"(symbols_by_exchange=[("abc", ["123", "234"])], )"
      R"(order_cancel_policy=MANAGED_ORDERS, )"
      R"(order_management=FIX)"
      R"(})"_sv);
}

TEST(format, market_by_price) {
  roq::MBPUpdate bids[] = {
      {.price = 1.0, .quantity = 2.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 2.0, .quantity = 4.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 3.0, .quantity = 8.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 4.0, .quantity = 10.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 5.0, .quantity = 12.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
  };
  roq::MBPUpdate asks[] = {
      {.price = 1.0, .quantity = 2.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 2.0, .quantity = 4.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 3.0, .quantity = 8.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 4.0, .quantity = 10.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
      {.price = 5.0, .quantity = 12.0, .implied_quantity = 3.0, .price_level = 1, .number_of_orders = 2},
  };
  roq::MarketByPriceUpdate market_by_price{
      .stream_id = {},
      .exchange = "deribit"_sv,
      .symbol = "BTC-27DEC19"_sv,
      .bids = {bids, std::size(bids)},
      .asks = {asks, std::size(asks)},
      .snapshot = true,
      .exchange_time_utc = {},
  };
  auto result = format("{}"_sv, market_by_price);
  EXPECT_GT(result.length(), size_t{0});
#if FMT_VERSION < 70000
  auto expected =
      R"({)"
      R"(stream_id=0, )"
      R"(exchange="deribit", )"
      R"(symbol="BTC-27DEC19", )"
      R"(bids=[{price=1.0, quantity=2.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=2.0, quantity=4.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=3.0, quantity=8.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=4.0, quantity=10.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=5.0, quantity=12.0, implied_quantity=3.0, price_level=1, number_of_orders=2}], )"
      R"(asks=[{price=1.0, quantity=2.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=2.0, quantity=4.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=3.0, quantity=8.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=4.0, quantity=10.0, implied_quantity=3.0, price_level=1, number_of_orders=2}, {price=5.0, quantity=12.0, implied_quantity=3.0, price_level=1, number_of_orders=2}], )"
      R"(snapshot=true, )"
      R"(exchange_time_utc=0ns)"
      R"(})"_sv;
#else
  auto expected =
      R"({)"
      R"(stream_id=0, )"
      R"(exchange="deribit", )"
      R"(symbol="BTC-27DEC19", )"
      R"(bids=[{price=1, quantity=2, implied_quantity=3, price_level=1, number_of_orders=2}, {price=2, quantity=4, implied_quantity=3, price_level=1, number_of_orders=2}, {price=3, quantity=8, implied_quantity=3, price_level=1, number_of_orders=2}, {price=4, quantity=10, implied_quantity=3, price_level=1, number_of_orders=2}, {price=5, quantity=12, implied_quantity=3, price_level=1, number_of_orders=2}], )"
      R"(asks=[{price=1, quantity=2, implied_quantity=3, price_level=1, number_of_orders=2}, {price=2, quantity=4, implied_quantity=3, price_level=1, number_of_orders=2}, {price=3, quantity=8, implied_quantity=3, price_level=1, number_of_orders=2}, {price=4, quantity=10, implied_quantity=3, price_level=1, number_of_orders=2}, {price=5, quantity=12, implied_quantity=3, price_level=1, number_of_orders=2}], )"
      R"(snapshot=true, )"
      R"(exchange_time_utc=0ns)"
      R"(})"_sv;
#endif
  EXPECT_EQ(result, expected);
}
