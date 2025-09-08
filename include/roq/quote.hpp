/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"
#include "roq/compat/fmt.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/execution_instruction.hpp"
#include "roq/limits.hpp"
#include "roq/mask.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Quote
struct ROQ_PUBLIC Quote final {
  uint32_t quote_entry_id = {};                                 //!< Quote Entry ID
  roq::Exchange exchange;                                       //!< Exchange
  roq::Symbol symbol;                                           //!< Symbol
  double bid_price = roq::NaN;                                  //!< Bid price level
  double bid_quantity = {};                                     //!< Total quantity available at bid
  double ask_price = roq::NaN;                                  //!< Ask price level
  double ask_quantity = {};                                     //!< Total quantity available at ask
  uint16_t quote_set_id = {};                                   //!< Quote Set ID (can be used to group and cancel quotes)
  roq::Mask<roq::ExecutionInstruction> execution_instructions;  //!< Execution instructions
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Quote> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Quote const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(quote_entry_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(bid_price={}, )"
        R"(bid_quantity={}, )"
        R"(ask_price={}, )"
        R"(ask_quantity={}, )"
        R"(quote_set_id={}, )"
        R"(execution_instructions={})"
        R"(}})"sv,
        value.quote_entry_id,
        value.exchange,
        value.symbol,
        value.bid_price,
        value.bid_quantity,
        value.ask_price,
        value.ask_quantity,
        value.quote_set_id,
        value.execution_instructions);
  }
};