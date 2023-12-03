/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/json/market_by_price_update.hpp"
#include "roq/json/reference_data.hpp"
#include "roq/json/top_of_book.hpp"

using namespace roq;

using namespace std::literals;
using namespace std::chrono_literals;

TEST_CASE("json_ReferenceData", "[json]") {
  json::Context context{
      .price_decimals = Decimals::_1,
      .quantity_decimals = Decimals::_0,
  };
  ReferenceData reference_data{
      .stream_id = 1,
      .exchange = "deribit"sv,
      .symbol = "BTC-PERPETUAL"sv,
      .description = "Perpetual"sv,
      .security_type = SecurityType::FUTURES,
      .base_currency = "BTC"sv,
      .quote_currency = "BTC"sv,
      .margin_currency = "BTC"sv,
      .commission_currency = "BTC"sv,
      .tick_size = 0.1,
      .multiplier = 10.0,
      .min_notional = NaN,
      .min_trade_vol = 1.0,
      .max_trade_vol = 999.0,
      .trade_vol_step_size = 1.0,
      .option_type = {},
      .strike_currency = {},
      .strike_price = NaN,
      .underlying = {},
      .time_zone = {},
      .issue_date = std::chrono::days{19168},
      .settlement_date = std::chrono::days{19168},
      .expiry_datetime = {},
      .expiry_datetime_utc = {},
      .discard = false,
  };
  auto value = fmt::format("{}"sv, json::ReferenceData{context, reference_data});
  auto expected = R"({)"
                  R"("stream_id":1,)"
                  R"("exchange":"deribit",)"
                  R"("symbol":"BTC-PERPETUAL",)"
                  R"("description":"Perpetual",)"
                  R"("security_type":"FUTURES",)"
                  R"("base_currency":"BTC",)"
                  R"("quote_currency":"BTC",)"
                  R"("margin_currency":"BTC",)"
                  R"("commission_currency":"BTC",)"
                  R"("tick_size":"0.1",)"
                  R"("multiplier":"10",)"
                  R"("min_notional":null,)"
                  R"("min_trade_vol":"1",)"
                  R"("max_trade_vol":"999",)"
                  R"("trade_vol_step_size":"1",)"
                  R"("option_type":null,)"
                  R"("strike_currency":null,)"
                  R"("strike_price":null,)"
                  R"("underlying":null,)"
                  R"("time_zone":null,)"
                  R"("issue_date":"2022-06-25",)"
                  R"("settlement_date":"2022-06-25",)"
                  R"("expiry_datetime":null,)"
                  R"("expiry_datetime_utc":null,)"
                  R"("discard":false)"
                  R"(})"sv;
  CHECK(value == expected);
}

TEST_CASE("json_TopOfBook", "[json]") {
  json::Context context{
      .price_decimals = Decimals::_2,
      .quantity_decimals = Decimals::_0,
  };
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
      .exchange_time_utc = 1656150170s,
      .exchange_sequence = 1234,
  };
  auto value = fmt::format("{}"sv, json::TopOfBook{context, top_of_book});
  auto expected = R"({)"
                  R"("stream_id":1,)"
                  R"("exchange":"deribit",)"
                  R"("symbol":"BTC-PERPETUAL",)"
                  R"("layer":{)"
                  R"("bid_price":"123.45",)"
                  R"("bid_quantity":"9",)"
                  R"("ask_price":"234.56",)"
                  R"("ask_quantity":"98")"
                  R"(},)"
                  R"("update_type":"INCREMENTAL",)"
                  R"("exchange_time_utc":"2022-06-25T09:42:50.000000000Z",)"
                  R"("exchange_sequence":1234)"
                  R"(})"sv;
  CHECK(value == expected);
}

TEST_CASE("json_MarketByPriceUpdate", "[json]") {
  json::Context context{
      .price_decimals = Decimals::_0,
      .quantity_decimals = Decimals::_0,
  };
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
      .exchange_time_utc = 1656150170s,
      .exchange_sequence = 1234,
      .price_decimals = {},
      .quantity_decimals = {},
      .max_depth = {},
      .checksum = {},
  };
  auto value = fmt::format("{}"sv, json::MarketByPriceUpdate{context, market_by_price_update});
  auto expected =
      R"({)"
      R"("stream_id":1,)"
      R"("exchange":"deribit",)"
      R"("symbol":"BTC-PERPETUAL",)"
      R"("bids":[)"
      R"({"price":"1","quantity":"2","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"2","quantity":"4","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"3","quantity":"8","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"4","quantity":"10","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"5","quantity":"12","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1})"
      R"(],)"
      R"("asks":[)"
      R"({"price":"1","quantity":"2","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"2","quantity":"4","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"3","quantity":"8","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"4","quantity":"10","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1},)"
      R"({"price":"5","quantity":"12","implied_quantity":"3","number_of_orders":2,"update_action":null,"price_level":1})"
      R"(],)"
      R"("update_type":"INCREMENTAL",)"
      R"("exchange_time_utc":"2022-06-25T09:42:50.000000000Z",)"
      R"("exchange_sequence":1234)"
      R"(})"sv;
  CHECK(value == expected);
}
