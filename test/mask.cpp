/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/mask.h"

using namespace roq;

namespace {
enum class E : int {
  A = 1,
  B = 2,
  C = 4,
  AB = A + B,
  ABC = AB + C,
};

// compile-time checks
static_assert(Mask<E>{E::A} == Mask<E>{E::A});
static_assert(Mask<E>{E::A} != Mask<E>{});
static_assert(Mask<E>{E::A} != Mask<E>{E::B});
static_assert(Mask<E>{E::A} == Mask<E>{}.set(E::A));
static_assert(Mask<E>{E::A, E::B} == Mask<E>{}.set(E::A).set(E::B));
static_assert(Mask<E>{E::AB} == Mask<E>{}.set(E::A).set(E::B));
}  // namespace

// run-time checks

TEST(mask, simple) {
  Mask m1{
      E::A,
  };
  EXPECT_TRUE(m1.has(E::A));
  EXPECT_FALSE(m1.has(E::B));
  Mask m2{m1, E::B};
  EXPECT_TRUE(m2.has(E::A));
  EXPECT_TRUE(m2.has(E::B));
  EXPECT_TRUE(m2.has_any(E::A));
  EXPECT_TRUE(m2.has_any(E::AB));
  EXPECT_TRUE(m2.has_any(E::ABC));
}
