/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/json/market_by_price_update.hpp"
#include "roq/json/top_of_book.hpp"

using namespace roq;

using namespace std::literals;

TEST_CASE("json_TopOfBook", "[json]") {
  TopOfBook top_of_book{
      .stream_id = 1,
      .exchange = "deribit"sv,
      .symbol = "BTC-PERPETUAL"sv,
      .layer =
          {
              .bid_price = 123.45,
              .bid_quantity = 9,
              .ask_price = 234.56,
              .ask_quantity = 98,
          },
      .update_type = UpdateType::INCREMENTAL,
      .exchange_time_utc = 1656150170ms,
      .exchange_sequence = 1234,
  };
  auto value = fmt::format("{}", json::TopOfBook{top_of_book});
  auto expected = R"({)"
                  R"("stream_id":1,)"
                  R"("exchange":"deribit",)"
                  R"("symbol":"BTC-PERPETUAL",)"
                  R"("layer":{)"
                  R"("bid_price":123.45,)"
                  R"("bid_quantity":9,)"
                  R"("ask_price":234.56,)"
                  R"("ask_quantity":98)"
                  R"(},)"
                  R"("update_type":"INCREMENTAL",)"
                  R"("exchange_time_utc":"1970-01-20T04:02:30.170000000Z",)"
                  R"("exchange_sequence":1234)"
                  R"(})"sv;
  CHECK(value == expected);
}

TEST_CASE("json_MarketByPriceUpdate", "[json]") {
  roq::MBPUpdate bids[] = {
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
  };
  roq::MBPUpdate asks[] = {
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
  };
  MarketByPriceUpdate market_by_price_update{
      .stream_id = 1,
      .exchange = "deribit"sv,
      .symbol = "BTC-PERPETUAL"sv,
      .bids = bids,
      .asks = asks,
      .update_type = UpdateType::INCREMENTAL,
      .exchange_time_utc = 1656150170ms,
      .exchange_sequence = 1234,
      .price_decimals = {},
      .quantity_decimals = {},
      .max_depth = {},
      .checksum = {},
  };
  auto value = fmt::format("{}", json::MarketByPriceUpdate{market_by_price_update});
  auto expected =
      R"({)"
      R"("stream_id":1,)"
      R"("exchange":"deribit",)"
      R"("symbol":"BTC-PERPETUAL",)"
      R"("bids":[)"
      R"({"price":1,"quantity":2,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":2,"quantity":4,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":3,"quantity":8,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":4,"quantity":10,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":5,"quantity":12,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1})"
      R"(],)"
      R"("asks":[)"
      R"({"price":1,"quantity":2,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":2,"quantity":4,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":3,"quantity":8,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":4,"quantity":10,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1},)"
      R"({"price":5,"quantity":12,"implied_quantity":3,"number_of_orders":2,"update_action":"UNDEFINED","price_level":1})"
      R"(],)"
      R"("update_type":"INCREMENTAL",)"
      R"("exchange_time_utc":"1970-01-20T04:02:30.170000000Z",)"
      R"("exchange_sequence":1234)"
      R"(})"sv;
  CHECK(value == expected);
}
