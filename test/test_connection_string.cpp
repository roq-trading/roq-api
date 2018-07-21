/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/strategy.h"

using namespace roq;  // NOLINT

TEST(connection_string, simple_failure_0) {
  EXPECT_THROW(
    strategy::Connection::create(""),
    std::runtime_error);
}

TEST(connection_string, simple_failure_1) {
  EXPECT_THROW(
    strategy::Connection::create("i:was@here"),
    std::runtime_error);
}

TEST(connection_string, simple) {
  auto connection = strategy::Connection::create("/var/tmp/xyz");
  EXPECT_EQ(connection.get_user(), "");
  EXPECT_EQ(connection.get_password(), "");
  const auto& address = connection.get_address();
  EXPECT_EQ(address.get_family(), AF_LOCAL);
  const auto sockaddr_un = reinterpret_cast<const struct sockaddr_un *>(address.get());
  EXPECT_STREQ(sockaddr_un->sun_path, "/var/tmp/xyz");
}

TEST(connection_string, simple_with_user) {
  auto connection = strategy::Connection::create("test@/var/tmp/xyz");
  EXPECT_EQ(connection.get_user(), "test");
  EXPECT_EQ(connection.get_password(), "");
  const auto& address = connection.get_address();
  EXPECT_EQ(address.get_family(), AF_LOCAL);
  const auto sockaddr_un = reinterpret_cast<const struct sockaddr_un *>(address.get());
  EXPECT_STREQ(sockaddr_un->sun_path, "/var/tmp/xyz");
}

TEST(connection_string, simple_with_user_and_password) {
  auto connection = strategy::Connection::create("test:1234@/var/tmp/xyz");
  EXPECT_EQ(connection.get_user(), "test");
  EXPECT_EQ(connection.get_password(), "1234");
  const auto& address = connection.get_address();
  EXPECT_EQ(address.get_family(), AF_LOCAL);
  const auto sockaddr_un = reinterpret_cast<const struct sockaddr_un *>(address.get());
  EXPECT_STREQ(sockaddr_un->sun_path, "/var/tmp/xyz");
}
