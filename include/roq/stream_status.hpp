/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string.hpp"

#include "roq/connection_status.hpp"
#include "roq/priority.hpp"
#include "roq/stream_type.hpp"
#include "roq/support_type.hpp"

namespace roq {

//! Update relating to current stream status
struct ROQ_PUBLIC StreamStatus final {
  uint16_t stream_id = {};       //!< Stream identifier
  std::string_view account;      //!< Account name
  Mask<SupportType> supports;    //!< Support types (bit mask)
  ConnectionStatus status = {};  //!< Connection status
  StreamType type = {};          //!< Stream type
  Priority priority = {};        //!< Priority
};

}  // namespace roq

template <>
struct fmt::formatter<roq::StreamStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::StreamStatus &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(supports={}, )"
        R"(status={}, )"
        R"(type={}, )"
        R"(priority={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.supports,
        value.status,
        value.type,
        value.priority);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::StreamStatus> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::StreamStatus> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(stream_status={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
