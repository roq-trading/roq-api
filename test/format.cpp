/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/format.h"

using namespace roq;  // NOLINT
using namespace fmt;  // NOLINT

TEST(format, ConnectionStatus) {
  typedef std::underlying_type<ConnectionStatus>::type type;
  for (auto i = static_cast<type>(ConnectionStatus::MIN);
      i <= static_cast<type>(ConnectionStatus::MAX); ++i) {
    EXPECT_EQ(
      format("{}", static_cast<ConnectionStatus>(i)),
      EnumNamesConnectionStatus()[i]);
  }
}

TEST(format, subscribe) {
  Subscribe subscribe {
    .accounts = {
      "test",
      "abc"
    }
  };
  auto xxx = fmt::format("{}", subscribe);
  // EXPECT_EQ(xxx, "abc");
}

TEST(format, market_by_price) {
  roq::MBPUpdate bid[] = {
    { .price = 1.0, .quantity = 2.0 },
    { .price = 2.0, .quantity = 4.0 },
    { .price = 3.0, .quantity = 8.0 },
    { .price = 4.0, .quantity = 10.0 },
    { .price = 5.0, .quantity = 12.0 },
  };
  roq::MBPUpdate ask[] = {
    { .price = 1.0, .quantity = 2.0 },
    { .price = 2.0, .quantity = 4.0 },
    { .price = 3.0, .quantity = 8.0 },
    { .price = 4.0, .quantity = 10.0 },
    { .price = 5.0, .quantity = 12.0 },
  };
  roq::MarketByPrice market_by_price {
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .bid_length = std::size(bid),
    .bid = bid,
    .ask_length = std::size(ask),
    .ask = ask,
    .snapshot = true,
    .exchange_time = {}
  };
  auto xxx = fmt::format("{}", market_by_price);
  EXPECT_GT(xxx.length(), size_t(0));
}
