/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/exceptions.hpp"

using namespace std::literals;

using namespace roq;

TEST_CASE("exceptions_simple_1", "[exceptions]") {
  auto ok = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (std::exception &) {
    ok = true;
  }
  CHECK(ok == true);
}

TEST_CASE("exceptions_simple_2", "[exceptions]") {
  auto ok = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (Exception &) {
    ok = true;
  }
  CHECK(ok == true);
}

TEST_CASE("exceptions_simple_3", "[exceptions]") {
  auto ok = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (RuntimeError &) {
    ok = true;
  }
  CHECK(ok == true);
}

TEST_CASE("exceptions_simple_4", "[exceptions]") {
  auto ok = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (NotReady &) {
    ok = true;
  }
  CHECK(ok == true);
}

TEST_CASE("exceptions_simple_5", "[exceptions]") {
  auto ok = false;
  try {
    throw RuntimeError{"something's not right"sv};
  } catch (RuntimeError &) {
    ok = true;
  }
  CHECK(ok == true);
}

TEST_CASE("exceptions_what", "[exceptions]") {
  auto ok = false;
  try {
    throw NotReady{"{}"sv, 123};
  } catch (NotReady &e) {
    ok = true;
    CHECK(e.what() == "123"sv);
  }
  CHECK(ok == true);
  ok = false;
  try {
    throw NotReady{"123"sv};
  } catch (NotReady &e) {
    ok = true;
    CHECK(e.what() == "123"sv);
  }
  CHECK(ok == true);
}
