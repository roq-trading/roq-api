/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <chrono>
#include <map>
#include <set>
#include <string>

#include "roq/stream.h"

#include "roq/stream/utility.h"

using namespace roq;  // NOLINT

// utilities

TEST(stream, time_point_str) {
  auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch());
  stream::BasicTimePointStr convert(now);
  std::string xxx(convert.c_str());
  EXPECT_GT(xxx.length(), size_t{0});
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
    .source_name = "FEMAS",
    .source_seqno = 0,
    .receive_time_utc = {},
    .receive_time = {},
    .source_send_time = {},
    .source_receive_time = {},
    .origin_create_time = {},
    .is_last = false,
    .opaque = 0,
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

TEST(stream, join_vector_int) {
  std::ostringstream ss;
  ss << roq::join(
      std::vector<int> {1, 2, 3});
  auto text = ss.str();
  EXPECT_EQ(text, std::string("[1, 2, 3]"));
}

TEST(stream, join_vector_string) {
  std::ostringstream ss;
  ss << roq::join(
      std::vector<std::string> {
        "abc",
        "def"
      });
  auto text = ss.str();
  EXPECT_EQ(text, std::string("[\"abc\", \"def\"]"));
}

TEST(stream, join_set_int) {
  std::ostringstream ss;
  ss << roq::join(
      std::set<int> {1, 2, 3});
  auto text = ss.str();
  EXPECT_EQ(text, std::string("{1, 2, 3}"));
}

TEST(stream, join_set_string) {
  std::ostringstream ss;
  ss << roq::join(
      std::set<std::string> {
        "abc",
        "def"
      });
  auto text = ss.str();
  EXPECT_EQ(text, std::string("{\"abc\", \"def\"}"));
}

TEST(stream, join_map_int_int) {
  std::ostringstream ss;
  ss << roq::join(
      std::map<int, int> {{1, 10}, {2, 20}, {3, 30}});
  auto text = ss.str();
  EXPECT_EQ(text, std::string("{1=10, 2=20, 3=30}"));
}

TEST(stream, join_map_string_int) {
  std::ostringstream ss;
  ss << roq::join(
      std::map<std::string, int> {
        {"abc", 10},
        {"def", 20}
      });
  auto text = ss.str();
  EXPECT_EQ(text, std::string("{\"abc\"=10, \"def\"=20}"));
}

TEST(stream, join_map_int_string) {
  std::ostringstream ss;
  ss << roq::join(
      std::map<int, std::string> {
        {10, "abc"},
        {20, "def"}
      });
  auto text = ss.str();
  EXPECT_EQ(text, std::string("{10=\"abc\", 20=\"def\"}"));
}

TEST(stream, join_map_string_string) {
  std::ostringstream ss;
  ss << roq::join(
      std::map<std::string, std::string> {
        {"123", "abc"},
        {"234", "def"}
      });
  auto text = ss.str();
  EXPECT_EQ(text, std::string("{\"123\"=\"abc\", \"234\"=\"def\"}"));
}
