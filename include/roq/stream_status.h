/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/connection_status.h"
#include "roq/priority.h"
#include "roq/stream_type.h"

namespace roq {

//! Update relating to current stream status
struct ROQ_PUBLIC StreamStatus final {
  uint16_t stream_id = {};       //!< Stream identifier
  std::string_view account;      //!< Account name (as known to the gateway)
  uint64_t supports = {};        //!< Update types (bit mask)
  ConnectionStatus status = {};  //!< Connection status
  StreamType type = {};          //!< Stream type
  Priority priority = {};        //!< Priority
};

}  // namespace roq

template <>
struct fmt::formatter<roq::StreamStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::StreamStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(supports='{:#x}', )"
        R"(status={}, )"
        R"(type={}, )"
        R"(priority={})"
        R"(}})"_fmt,
        value.stream_id,
        value.account,
        value.supports,
        value.status,
        value.type,
        value.priority);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::StreamStatus> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::StreamStatus> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(stream_status={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
