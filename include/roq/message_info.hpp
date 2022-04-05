/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/numbers.hpp"
#include "roq/string_types.hpp"
#include "roq/uuid.hpp"

namespace roq {

//! Trace information relating to the current message
struct ROQ_PUBLIC MessageInfo final {
  uint8_t source = {};                                   //!< Source identifier (index into the list of connections)
  std::string_view source_name;                          //!< Source name
  UUID source_session_id;                                //!< Session identifier (UUID)
  uint64_t source_seqno = {};                            //!< Sequence number (strictly increasing)
  std::chrono::nanoseconds receive_time_utc = {};        //!< Client receive time (realtime clock)
  std::chrono::nanoseconds receive_time = {};            //!< Client receive time (monotonic clock)
  std::chrono::nanoseconds source_send_time = {};        //!< Source send time (monotonic clock)
  std::chrono::nanoseconds source_receive_time = {};     //!< Source receive time (monotonic clock)
  std::chrono::nanoseconds origin_create_time = {};      //!< Origin create time (monotonic clock)
  std::chrono::nanoseconds origin_create_time_utc = {};  //!< Origin create time (realtime clock)
  bool is_last = false;                                  //!< Is last in batch?
  uint64_t opaque = {};                                  //!< Opaque value (internal)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MessageInfo> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::MessageInfo &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(source={}, )"
        R"(source_name="{}", )"
        R"(source_session_id="{}", )"
        R"(source_seqno={}, )"
        R"(receive_time_utc={}, )"
        R"(receive_time={}, )"
        R"(source_send_time={}, )"
        R"(source_receive_time={}, )"
        R"(origin_create_time={}, )"
        R"(origin_create_time_utc={}, )"
        R"(is_last={}, )"
        R"(opaque={})"
        R"(}})"sv,
        value.source,
        value.source_name,
        value.source_session_id,
        value.source_seqno,
        value.receive_time_utc,
        value.receive_time,
        value.source_send_time,
        value.source_receive_time,
        value.origin_create_time,
        value.origin_create_time_utc,
        value.is_last,
        value.opaque);
  }
};
