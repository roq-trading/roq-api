/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/quote.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Quotes
struct ROQ_PUBLIC MassQuote final {
  std::string_view account;            //!< Account name
  uint64_t quote_req_id = {};          //!< Quote Request ID
  std::span<roq::Quote const> quotes;  //!< Quotes
};

template <>
inline constexpr std::string_view get_name<MassQuote>() {
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
        R"(quote_req_id={}, )"
        R"(quotes=[{}])"
        R"(}})"sv,
        value.account,
        value.quote_req_id,
        fmt::join(value.quotes, ", "sv));
  }
};
