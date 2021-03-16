/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/mask.h"

namespace roq {
namespace {
enum class Test : int { A = 1, B = 2, C = 3 };

static_assert(Mask<Test>{Test::A} == Mask<Test>{Test::A});
static_assert(Mask<Test>{Test::A} != Mask<Test>{});
static_assert(Mask<Test>{Test::A} != Mask<Test>{Test::B});
static_assert(Mask<Test>{Test::A} == Mask<Test>{}.set(Test::A));
static_assert(Mask<Test>{Test::A, Test::B} == Mask<Test>{}.set(Test::A).set(Test::B));
}  // namespace
}  // namespace roq
