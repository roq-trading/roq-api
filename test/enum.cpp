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
struct MyEnum : public roq::Enum<detail::MyEnum> {
  using enum detail::MyEnum;
};

struct MyStruct {
  MyEnum e;
};
}  // namespace

TEST_CASE("enum_simple", "[enum]") {
  MyEnum e0;
  CHECK(fmt::format("{}"sv, e0) == "UNDEFINED"sv);
  MyEnum e1{MyEnum::A};
  CHECK(fmt::format("{}"sv, e1) == "A"sv);
  // XXX doesn't work...
  /*
  MyEnum e2 = MyEnum::B;
  CHECK(fmt::format("{}"sv, e2) == "B"sv);
  */
  MyStruct s0;
  CHECK(fmt::format("{}"sv, s0.e) == "UNDEFINED"sv);
  // XXX doesn't work...
  /*
  MyStruct s1{
      .e = MyEnum::A,
  };
  CHECK(fmt::format("{}"sv, s1.e) == "A"sv);
  */
}
