/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <chrono>

#include "roq/stream.h"

#include "roq/stream/utility.h"

using namespace roq;  // NOLINT

// utilities

TEST(stream, time_point_str) {
  auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch());
  stream::BasicTimePointStr convert(now);
  std::string xxx(convert.c_str());
  EXPECT_GT(xxx.length(), 0);
}

// enums

TEST(stream, connection_status) {
  auto connection_status = ConnectionStatus::DISCONNECTED;
  std::ostringstream stream;
  stream << connection_status;
  auto text = stream.str();
  EXPECT_EQ(
      text,
      EnumNamesConnectionStatus()[static_cast<uint8_t>(connection_status)]);
}

//

TEST(stream, message_info) {
  MessageInfo message_info {
    .source = 0,
    .source_name = "FEMAS"
  };
  std::ostringstream stream;
  stream << message_info;
  auto text = stream.str();
}

// events

/*
TEST(stream, layer) {
  Layer layer {};
  std::ostringstream stream;
  stream << layer;
  auto text = stream.str();
}

TEST(stream, market_by_price) {
  MarketByPrice market_by_price {
    .exchange = "CFFEX",
    .symbol = "IC1801",
  };
  std::ostringstream stream;
  stream << market_by_price;
  auto text = stream.str();
}
*/
