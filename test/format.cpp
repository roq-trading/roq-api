/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <array>

#include "roq/market_by_order_update.hpp"
#include "roq/market_by_price_update.hpp"
#include "roq/side.hpp"

using namespace std::literals;

using namespace roq;

TEST_CASE("format_side", "[format]") {
  CHECK(fmt::format("{}"sv, Side{Side::UNDEFINED}) == "UNDEFINED"sv);
  CHECK(fmt::format("{}"sv, Side{Side::BUY}) == "BUY"sv);
  CHECK(fmt::format("{}"sv, Side{Side::SELL}) == "SELL"sv);
}

TEST_CASE("format_string", "[format]") {
  MBOOrderId order_id = "1234"sv;
  CHECK(fmt::format("{}"sv, order_id) == "1234"sv);
}

TEST_CASE("format_market_by_price", "[format]") {
  std::array<MBPUpdate, 5> bids{{
      {
          .price = 1.0,
          .quantity = 2.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 2.0,
          .quantity = 4.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 3.0,
          .quantity = 8.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 4.0,
          .quantity = 10.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 5.0,
          .quantity = 12.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
  }};
  std::array<MBPUpdate, 5> asks{{
      {
          .price = 1.0,
          .quantity = 2.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 2.0,
          .quantity = 4.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 3.0,
          .quantity = 8.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 4.0,
          .quantity = 10.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
      {
          .price = 5.0,
          .quantity = 12.0,
          .implied_quantity = 3.0,
          .number_of_orders = 2,
          .update_action = {},
          .price_level = 1,
      },
  }};
  auto market_by_price = MarketByPriceUpdate{
      .stream_id = {},
      .exchange = "deribit"sv,
      .symbol = "BTC-27DEC19"sv,
      .bids = bids,
      .asks = asks,
      .update_type = UpdateType::SNAPSHOT,
      .exchange_time_utc = {},
      .exchange_sequence = 123,
      .price_precision = Precision::_0,
      .quantity_precision = Precision::_1,
      .max_depth = 10,
      .checksum = 123,
  };
  auto result = fmt::format("{}"sv, market_by_price);
  CHECK(std::size(result) > 0uz);
  auto expected = R"({)"
                  R"(stream_id=0, )"
                  R"(exchange="deribit", )"
                  R"(symbol="BTC-27DEC19", )"
                  R"(bids=[)"
                  R"({price=1, quantity=2, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=2, quantity=4, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=3, quantity=8, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=4, quantity=10, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=5, quantity=12, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1})"
                  R"(], )"
                  R"(asks=[)"
                  R"({price=1, quantity=2, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=2, quantity=4, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=3, quantity=8, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=4, quantity=10, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1}, )"
                  R"({price=5, quantity=12, implied_quantity=3, number_of_orders=2, update_action=UNDEFINED, price_level=1})"
                  R"(], )"
                  R"(update_type=SNAPSHOT, )"
                  R"(exchange_time_utc=0ns, )"
                  R"(exchange_sequence=123, )"
                  R"(sending_time_utc=0ns, )"
                  R"(price_precision=_0, )"
                  R"(quantity_precision=_1, )"
                  R"(max_depth=10, )"
                  R"(checksum=123)"
                  R"(})"sv;
  CHECK(result == expected);
}
