/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {
namespace metrics {

enum class Type {
  COUNTER,
  INTER_PROCESS_LATENCY,
  LATENCY,
  PROFILE,
  CLIENTS,
  EVENTS,
  EXCEPTIONS,
  HEARTBEAT_LATENCY,
  PROCESS,
  ROUND_TRIP_LATENCY,
  UPDATED,
  EVENT_LOG,
  MARKET_DATA_LATENCY,
  QUEUE_LATENCY,
  REQUEST_LATENCY,
  JOURNAL_LATENCY,
  END_TO_END_LATENCY,
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
    case QUEUE_LATENCY:
      return "roq_queue_latency"sv;
    case REQUEST_LATENCY:
      return "roq_request_latency"sv;
    case JOURNAL_LATENCY:
      return "roq_journal_latency"sv;
    case END_TO_END_LATENCY:
      return "roq_end_to_end_latency"sv;
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
