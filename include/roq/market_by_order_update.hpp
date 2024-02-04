/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/mbo_update.hpp"
#include "roq/name.hpp"
#include "roq/precision.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to market by order
struct ROQ_PUBLIC MarketByOrderUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  std::span<MBOUpdate const> orders;                //!< List of order updates
  UpdateType update_type = {};                      //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
  Precision price_decimals = {};                    //!< Decimal digits required to represent prices (dynamic)
  Precision quantity_decimals = {};                 //!< Decimal digits required to represent quantities (dynamic)
  uint16_t max_depth = {};                          //!< Maximum depth (zero means unlimited)
  uint32_t checksum = {};                           //!< Checksum (internal)
};

template <>
inline constexpr std::string_view get_name<MarketByOrderUpdate>() {
  using namespace std::literals;
  return "market_by_order_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketByOrderUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MarketByOrderUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(orders=[{}], )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(sending_time_utc={}, )"
        R"(price_decimals={}, )"
        R"(quantity_decimals={}, )"
        R"(max_depth={}, )"
        R"(checksum={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        fmt::join(value.orders, ", "sv),
        value.update_type,
        value.exchange_time_utc,
        value.exchange_sequence,
        value.sending_time_utc,
        value.price_decimals,
        value.quantity_decimals,
        value.max_depth,
        value.checksum);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::MarketByOrderUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::MarketByOrderUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(market_by_order_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::MarketByOrderUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::MarketByOrderUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(market_by_order_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
