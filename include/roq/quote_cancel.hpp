/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Cancel quotes
struct ROQ_PUBLIC QuoteCancel final {
  std::string_view account;    //!< Account name
  uint16_t quote_set_id = {};  //!< Quote Request ID
};

template <>
inline constexpr std::string_view get_name<QuoteCancel>() {
  using namespace std::literals;
  return "quote_cancel"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::QuoteCancel> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::QuoteCancel const &value, format_context &context) const {
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

template <>
struct fmt::formatter<roq::Event<roq::QuoteCancel>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::QuoteCancel> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(quote_cancel={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::QuoteCancel>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::QuoteCancel> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(quote_cancel={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
