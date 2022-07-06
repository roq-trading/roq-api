/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/mask.hpp"

using namespace std::literals;

using namespace roq;

namespace {
enum class E : int {
  A = 1,
  B = 2,
  C = 4,
};

// compile-time checks
static_assert(Mask<E>{E::A} == Mask<E>{E::A});
static_assert(Mask<E>{E::A} != Mask<E>{});
static_assert(Mask<E>{E::A} != Mask<E>{E::B});
static_assert(Mask<E>{E::A} == Mask<E>{}.set(E::A));
static_assert(Mask<E>{E::A, E::B} == Mask<E>{}.set(E::A).set(E::B));
static_assert(Mask<E>{E::A, E::B, E::C} == Mask<E>{}.set(E::A).set(E::B).set(E::C));
}  // namespace

template <>
struct fmt::formatter<E> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const E &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};

// run-time checks

TEST_CASE("mask_simple", "[mask]") {
  Mask m1{
      E::A,
  };
  CHECK(m1.has(E::A) == true);
  CHECK(m1.has(E::B) == false);
  Mask m2{m1, E::B};
  CHECK(m2.has(E::A) == true);
  CHECK(m2.has(E::B) == true);
  CHECK(m2.has_any(E::A) == true);
  CHECK(m2.has_any({E::A, E::B}) == true);
  CHECK(m2.has_any({E::A, E::B, E::C}) == true);
  CHECK(fmt::format("{}"sv, Mask{E::A}) == "A"sv);
  CHECK(fmt::format("{}"sv, Mask{E::B, E::C}) == "B|C"sv);
  CHECK(fmt::format("{}"sv, Mask{E::A, E::B, E::C}) == "A|B|C"sv);
}

// with "undefined"
namespace {
enum class E2 : int {
  U = 0,
  A = 1,
  B = 2,
  C = 4,
};
}

template <>
struct fmt::formatter<E2> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const E2 &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};

TEST_CASE("mask_with_undefined", "[mask]") {
  // CHECK(fmt::format("{}"sv, Mask<E2>{}) == "U"sv);
  CHECK(fmt::format("{}"sv, Mask{E2::U}) == "U"sv);
  CHECK(fmt::format("{}"sv, Mask{E2::A}) == "A"sv);
  CHECK(fmt::format("{}"sv, Mask{E2::A, E2::C}) == "A|C"sv);
  CHECK(fmt::format("{}"sv, Mask{E2::A, E2::B, E2::C}) == "A|B|C"sv);
}
