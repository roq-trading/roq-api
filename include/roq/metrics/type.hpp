/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <magic_enum/magic_enum.hpp>

#include <fmt/format.h>

namespace roq {
namespace metrics {

//! Enumeration of metrics keys
enum class Type {
  COUNTER,                //!< Gateway specific
  INTER_PROCESS_LATENCY,  //!< IPC latency between components (one-way)
  LATENCY,                //!< Exchange latency (transport or protocol level)
  PROFILE,                //!< Function profiling
  CLIENTS,                //!< Connection events
  EVENTS,                 //!< Event profiling
  EXCEPTIONS,             //!< Rejected order requests (counter)
  HEARTBEAT_LATENCY,      //!< Heartbeat latency between components
  PROCESS,                //!< Process information
  ROUND_TRIP_LATENCY,     //!< Tick-to-trade latency (round-trip, internal)
  UPDATED,                //!< State changes for connections (counter)
  EVENT_LOG,              //!< Event-log activity (counter)
  MARKET_DATA_LATENCY,    //!< Market data latency (one-way)
  REQUEST_LATENCY,        //!< Request latency (round-trip)
  JOURNAL_LATENCY,        //!< Journal latency (round-trip)
  END_TO_END_LATENCY,     //!< Latency between entry (origin) and exit (used by fix-bridge)
  RATE_LIMITER,           //!< Rate limiter (gauge)
};

inline static std::string_view get_metrics_name(Type type) {
  using namespace std::literals;
  switch (type) {
    using enum Type;
    case COUNTER:
      return "roq_counter"sv;
    case INTER_PROCESS_LATENCY:
      return "roq_inter_process_latency"sv;
    case LATENCY:
      return "roq_latency"sv;
    case PROFILE:
      return "roq_profile"sv;
    case CLIENTS:
      return "roq_clients"sv;
    case EVENTS:
      return "roq_events"sv;
    case EXCEPTIONS:
      return "roq_exceptions"sv;
    case HEARTBEAT_LATENCY:
      return "roq_heartbeat_latency"sv;
    case PROCESS:
      return "roq_process"sv;
    case ROUND_TRIP_LATENCY:
      return "roq_round_trip_latency"sv;
    case UPDATED:
      return "roq_updated"sv;
    case EVENT_LOG:
      return "roq_event_log"sv;
    case MARKET_DATA_LATENCY:
      return "roq_market_data_latency"sv;
    case REQUEST_LATENCY:
      return "roq_request_latency"sv;
    case JOURNAL_LATENCY:
      return "roq_journal_latency"sv;
    case END_TO_END_LATENCY:
      return "roq_end_to_end_latency"sv;
    case RATE_LIMITER:
      return "roq_rate_limiter"sv;
  }
  std::abort();
}

}  // namespace metrics
}  // namespace roq

template <>
struct fmt::formatter<roq::metrics::Type> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::metrics::Type const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
