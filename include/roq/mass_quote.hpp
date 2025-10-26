/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/quote.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Quotes
struct ROQ_PUBLIC MassQuote final {
  std::string_view account;                       //!< Account name
  uint32_t quote_id = {};                         //!< Quote ID
  std::span<roq::Quote const> quotes;             //!< Quotes
  bool mmp_reset = false;                         //!< Market Maker Protection reset? (when supported by exchange)
  std::string_view mmp_group;                     //!< Market Maker Protection group name (when supported by exchange)
  std::chrono::nanoseconds valid_until_utc = {};  //!< Quote valid until UTC timestamp (when supported by exchange)
};

template <>
constexpr std::string_view get_name<MassQuote>() {
  using namespace std::literals;
  return "mass_quote"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::MassQuote> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MassQuote const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(quote_id={}, )"
        R"(quotes=[{}], )"
        R"(mmp_reset={}, )"
        R"(mmp_group="{}", )"
        R"(valid_until_utc={})"
        R"(}})"sv,
        value.account,
        value.quote_id,
        fmt::join(value.quotes, ", "sv),
        value.mmp_reset,
        value.mmp_group,
        value.valid_until_utc);
  }
};