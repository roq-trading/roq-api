/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/exceptions.h"

using namespace roq;
using namespace std::literals;

TEST(exceptions, simple_1) {
  auto ok = false;
  try {
    throw NotReady("something's not right"sv);
  } catch (std::exception &) {
    ok = true;
  }
  EXPECT_TRUE(ok);
}

TEST(exceptions, simple_2) {
  auto ok = false;
  try {
    throw NotReady("something's not right"sv);
  } catch (Exception &) {
    ok = true;
  }
  EXPECT_TRUE(ok);
}

TEST(exceptions, simple_3) {
  auto ok = false;
  try {
    throw NotReady("something's not right"sv);
  } catch (RuntimeError &) {
    ok = true;
  }
  EXPECT_TRUE(ok);
}

TEST(exceptions, simple_4) {
  auto ok = false;
  try {
    throw NotReady("something's not right"sv);
  } catch (NotReady &) {
    ok = true;
  }
  EXPECT_TRUE(ok);
}

TEST(exceptions, simple_5) {
  auto ok = false;
  try {
    throw RuntimeError("something's not right"sv);
  } catch (RuntimeError &) {
    ok = true;
  }
  EXPECT_TRUE(ok);
}

TEST(exceptions, what) {
  auto ok = false;
  try {
    throw NotReady("{}"sv, 123);
  } catch (NotReady &e) {
    ok = true;
    EXPECT_EQ(e.what(), "123"sv);
  }
  EXPECT_TRUE(ok);
  ok = false;
  try {
    throw NotReady("123"sv);
  } catch (NotReady &e) {
    ok = true;
    EXPECT_EQ(e.what(), "123"sv);
  }
  EXPECT_TRUE(ok);
}
