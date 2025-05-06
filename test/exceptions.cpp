/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/exceptions.hpp"

using namespace std::literals;

using namespace roq;

// NOLINTBEGIN(readability-magic-numbers)

TEST_CASE("exceptions_simple_1", "[exceptions]") {
  auto success = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (std::exception &) {
    success = true;
  }
  CHECK(success == true);
}

TEST_CASE("exceptions_simple_2", "[exceptions]") {
  auto success = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (Exception &) {
    success = true;
  }
  CHECK(success == true);
}

TEST_CASE("exceptions_simple_3", "[exceptions]") {
  auto success = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (RuntimeError &) {
    success = true;
  }
  CHECK(success == true);
}

TEST_CASE("exceptions_simple_4", "[exceptions]") {
  auto success = false;
  try {
    throw NotReady{"something's not right"sv};
  } catch (NotReady &) {
    success = true;
  }
  CHECK(success == true);
}

TEST_CASE("exceptions_simple_5", "[exceptions]") {
  auto success = false;
  try {
    throw RuntimeError{"something's not right"sv};
  } catch (RuntimeError &) {
    success = true;
  }
  CHECK(success == true);
}

TEST_CASE("exceptions_what", "[exceptions]") {
  auto success = false;
  try {
    throw NotReady{"{}"sv, 123};
  } catch (NotReady &e) {
    success = true;
    CHECK(e.what() == "123"sv);
  }
  CHECK(success == true);
  success = false;
  try {
    throw NotReady{"123"sv};
  } catch (NotReady &e) {
    success = true;
    CHECK(e.what() == "123"sv);
  }
  CHECK(success == true);
}

// NOLINTEND(readability-magic-numbers)
