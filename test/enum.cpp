/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include "roq/enum.hpp"

using namespace std::literals;

using namespace Catch::literals;

namespace detail {
enum class MyEnum : uint8_t {
  UNDEFINED,
  A,
  B,
};
}

namespace {
struct MyEnum final : public roq::Enum<detail::MyEnum> {
  using enum type_t;  // https://stackoverflow.com/a/67000446
  using base_t::base_t;
  using base_t::operator=;

  constexpr MyEnum &operator=(type_t type) {
    type_ = type;
    return *this;
  }
};
}  // namespace

template <>
struct std::is_enum<MyEnum> : std::true_type {};

template <>
struct std::underlying_type<MyEnum> {
  using type = typename MyEnum::value_t;
};

static_assert(std::is_enum<MyEnum>::value);
static_assert(std::is_same<std::underlying_type<MyEnum>::type, std::underlying_type<detail::MyEnum>::type>::value);

namespace {
struct MyStruct {
  MyEnum e;
};
}  // namespace

TEST_CASE("enum_simple", "[enum]") {
  MyEnum e0;
  CHECK(fmt::format("{}"sv, e0) == "UNDEFINED"sv);
  MyEnum e1{MyEnum::A};
  CHECK(fmt::format("{}"sv, e1) == "A"sv);
  MyEnum e2 = MyEnum::B;
  CHECK(fmt::format("{}"sv, e2) == "B"sv);
  MyEnum e3;
  e3 = MyEnum::A;
  CHECK(fmt::format("{}"sv, e3) == "A"sv);
  MyEnum e4{static_cast<std::underlying_type<detail::MyEnum>::type>(detail::MyEnum::B)};
  CHECK(fmt::format("{}"sv, e4) == "B"sv);
}

TEST_CASE("struct_simple", "[enum]") {
  MyStruct s0;
  CHECK(fmt::format("{}"sv, s0.e) == "UNDEFINED"sv);
  MyStruct s1{
      .e = MyEnum::A,
  };
  CHECK(fmt::format("{}"sv, s1.e) == "A"sv);
  MyStruct s2;
  s2.e = MyEnum::B;
  CHECK(fmt::format("{}"sv, s2.e) == "B"sv);
}

TEST_CASE("gcc_failure", "[enum]") {
  MyEnum e;
  e = true ? MyEnum::A : MyEnum::B;
}
