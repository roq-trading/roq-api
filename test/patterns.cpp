/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <string>

#include <catch2/catch_test_macros.hpp>

#include "roq/utils/patterns.hpp"

using namespace roq;

TEST_CASE("patterns_overloaded", "[patterns]") {
  std::string method;
  auto visitor = utils::overloaded{
      [&](int) { method = "int"; }, [&](double) { method = "double"; }, [&](std::string) { method = "string"; }};
  method = "";
  visitor(1);
  CHECK(method == "int");
  method = "";
  visitor(1.2);
  CHECK(method == "double");
  method = "";
  visitor("foo");
  CHECK(method == "string");
}
