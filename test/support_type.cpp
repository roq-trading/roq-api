/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/mask.hpp"
#include "roq/support_type.hpp"

using namespace std::literals;

using namespace roq;

TEST_CASE("support_type_format", "[support_type]") {
  CHECK(fmt::format("{}"sv, SupportType::UNDEFINED) == "UNDEFINED"sv);
  CHECK(fmt::format("{}"sv, SupportType::REFERENCE_DATA) == "REFERENCE_DATA"sv);
  // ...
  CHECK(fmt::format("{}"sv, SupportType::FUNDS) == "FUNDS"sv);
}

TEST_CASE("support_type_mask_empty", "[support_type]") {
  auto supports = Mask<SupportType>{};
  CHECK(fmt::format("{}"sv, supports) == ""sv);
}

TEST_CASE("support_type_mask_lower_bits", "[support_type]") {
  auto supports = Mask{
      SupportType::REFERENCE_DATA,
      SupportType::TOP_OF_BOOK,
  };
  REQUIRE(supports.has(SupportType::REFERENCE_DATA));
  REQUIRE(supports.has(SupportType::TOP_OF_BOOK));
  CHECK(fmt::format("{}"sv, supports) == "REFERENCE_DATA|TOP_OF_BOOK"sv);
}

TEST_CASE("support_type_mask_higher_bits", "[support_type]") {
  auto supports = Mask{
      SupportType::ORDER_ACK,
      SupportType::ORDER,
      SupportType::FUNDS,
  };
  REQUIRE(supports.has(SupportType::ORDER_ACK));
  REQUIRE(supports.has(SupportType::ORDER));
  REQUIRE(supports.has(SupportType::FUNDS));
  CHECK(fmt::format("{}"sv, supports) == "ORDER_ACK|ORDER|FUNDS"sv);
}
