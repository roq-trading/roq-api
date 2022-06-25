/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

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
  auto expected = R"({"stream_id":1,"exchange":"deribit","symbol":"BTC-PERPETUAL",)"
                  R"("layer":{bid_price=123.45, bid_quantity=9, ask_price=234.56, ask_quantity=98},)"
                  R"("update_type":"INCREMENTAL","exchange_time_utc":"1970-01-20T04:02:30.170000000Z",)"
                  R"("exchange_sequence":1234})"sv;
  CHECK(value == expected);
}
