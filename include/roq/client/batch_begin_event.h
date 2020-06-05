/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/compat.h"

#include "roq/message_info.h"

namespace roq {
namespace client {

struct ROQ_PUBLIC BatchBeginEvent final {
  const MessageInfo& message_info;
};

}  // namespace client
}  // namespace roq

template <>
struct fmt::formatter<roq::client::BatchBeginEvent> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::client::BatchBeginEvent& value,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={})"
        R"(}})",
        value.message_info);
  }
};
