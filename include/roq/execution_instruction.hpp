/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <cstdlib>

namespace roq {

//! Enumeration of execution types
enum class ExecutionInstruction : uint32_t {
  UNDEFINED = 0,
  PARTICIPATE_DO_NOT_INITIATE = 0x1,  //!< Cancel if order would have executed on placement (i.e. not as maker)
  CANCEL_IF_NOT_BEST = 0x2,           //!< Cancel if order can not be placed at best price
  DO_NOT_INCREASE = 0x4,              //!< Order may only reduce net position, order quantity can automatically be reduced by exchange
  DO_NOT_REDUCE = 0x8,                //!< Order can not be partially filled, aka. all-or-none (AON) orders
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ExecutionInstruction> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ExecutionInstruction const &value, format_context &context) const {
    using namespace std::literals;
    auto name = [&]() -> std::string_view {
      switch (value) {
        using enum roq::ExecutionInstruction;
        case UNDEFINED:
          return "UNDEFINED"sv;
        case PARTICIPATE_DO_NOT_INITIATE:
          return "PARTICIPATE_DO_NOT_INITIATE"sv;
        case CANCEL_IF_NOT_BEST:
          return "CANCEL_IF_NOT_BEST"sv;
        case DO_NOT_INCREASE:
          return "DO_NOT_INCREASE"sv;
        case DO_NOT_REDUCE:
          return "DO_NOT_REDUCE"sv;
      }
      std::abort();
    }();
    return fmt::format_to(context.out(), "{}"sv, name);
  }
};
