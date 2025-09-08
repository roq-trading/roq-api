/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"
#include "roq/compat/fmt.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Cancel quotes
struct ROQ_PUBLIC CancelQuotes final {
  std::string_view account;    //!< Account name
  uint16_t quote_set_id = {};  //!< Quote Request ID
};

template <>
constexpr std::string_view get_name<CancelQuotes>() {
  using namespace std::literals;
  return "cancel_quotes"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelQuotes> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CancelQuotes const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(quote_set_id={})"
        R"(}})"sv,
        value.account,
        value.quote_set_id);
  }
};