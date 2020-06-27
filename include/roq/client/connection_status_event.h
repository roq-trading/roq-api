/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/event.h"
#include "roq/connection_status.h"

template <>
struct fmt::formatter<roq::Event<roq::ConnectionStatus> > {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::Event<roq::ConnectionStatus>& event,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(connection_status={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
