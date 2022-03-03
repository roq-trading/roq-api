/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include "roq/utils/mask.h"

using namespace roq;
using namespace roq::utils;

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

TEST_CASE("mask_simple", "mask") {
  Mask m1{
      E::A,
  };
  CHECK(m1.has(E::A) == true);
  CHECK(m1.has(E::B) == false);
  Mask m2{m1, E::B};
  CHECK(m2.has(E::A) == true);
  CHECK(m2.has(E::B) == true);
  CHECK(m2.has_any(E::A) == true);
  CHECK(m2.has_any(E::AB) == true);
  CHECK(m2.has_any(E::ABC) == true);
}
