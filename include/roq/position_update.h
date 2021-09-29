/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Update relating to current position for a symbol/side/account
struct ROQ_PUBLIC PositionUpdate final {
  uint16_t stream_id = {};            //!< Stream identifier
  std::string_view account;           //!< Account name
  std::string_view exchange;          //!< Exchange
  std::string_view symbol;            //!< Symbol
  std::string_view external_account;  //!< External account name
  double long_quantity = NaN;         //!< Current long position
  double short_quantity = NaN;        //!< Current short position
  double long_quantity_begin = NaN;   //!< Long position at period begin
  double short_quantity_begin = NaN;  //!< Short position at period begin
};

}  // namespace roq

template <>
struct fmt::formatter<roq::PositionUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::PositionUpdate &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(external_account="{}", )"
        R"(long_quantity={}, )"
        R"(short_quantity={}, )"
        R"(long_quantity_begin={}, )"
        R"(short_quantity_begin={})"
        R"(}})"_sv,
        value.stream_id,
        value.account,
        value.exchange,
        value.symbol,
        value.external_account,
        value.long_quantity,
        value.short_quantity,
        value.long_quantity_begin,
        value.short_quantity_begin);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::PositionUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::PositionUpdate> &event, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(position_update={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
