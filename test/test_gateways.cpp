/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>
#include "quinclas/client.h"

using namespace quinclas;  // NOLINT

TEST(gateways, simple_failure_1) {
  EXPECT_THROW(
    client::Gateways::create(""),
    std::runtime_error);
}

TEST(gateways, simple_failure_2) {
  EXPECT_THROW(
    client::Gateways::create("abc="),
    std::runtime_error);
}

TEST(gateways, simple_failure_3) {
  EXPECT_THROW(
    client::Gateways::create("abc=def,"),
    std::runtime_error);
}

TEST(gateways, simple_1) {
  auto gateways = client::Gateways::create("femas=/var/tmp/xyz");
  EXPECT_EQ(gateways.size(), 1);
  const auto& iter = gateways.begin();
  const auto& name = (*iter).first;
  const auto& connection = (*iter).second;
  EXPECT_EQ(name, "femas");
  EXPECT_EQ(connection.get_user(), "");
  EXPECT_EQ(connection.get_password(), "");
  const auto& address = connection.get_address();
  EXPECT_EQ(address.get_family(), AF_LOCAL);
  const auto sockaddr_un = reinterpret_cast<const struct sockaddr_un *>(address.get());
  EXPECT_STREQ(sockaddr_un->sun_path, "/var/tmp/xyz");
}

TEST(gateways, simple_2) {
  auto gateways = client::Gateways::create(
      "femas=test:1234@/var/tmp/femasapi.sock,"
      "sim=/var/tmp/simulator.sock");
  EXPECT_EQ(gateways.size(), 2);
  // femas
  const auto& femas = gateways.at("femas");
  EXPECT_EQ(femas.get_user(), "test");
  EXPECT_EQ(femas.get_password(), "1234");
  const auto& femas_address = femas.get_address();
  EXPECT_EQ(femas_address.get_family(), AF_LOCAL);
  const auto femas_sockaddr_un = reinterpret_cast<const struct sockaddr_un *>(femas_address.get());
  EXPECT_STREQ(femas_sockaddr_un->sun_path, "/var/tmp/femasapi.sock");
  // simulator
  const auto& sim = gateways.at("sim");
  EXPECT_EQ(sim.get_user(), "");
  EXPECT_EQ(sim.get_password(), "");
  const auto& sim_address = sim.get_address();
  EXPECT_EQ(sim_address.get_family(), AF_LOCAL);
  const auto sim_sockaddr_un = reinterpret_cast<const struct sockaddr_un *>(sim_address.get());
  EXPECT_STREQ(sim_sockaddr_un->sun_path, "/var/tmp/simulator.sock");
}
