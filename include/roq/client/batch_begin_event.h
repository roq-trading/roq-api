/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/compat.h"
#include "roq/event.h"

#include "roq/message_info.h"

namespace roq {
namespace client {

struct ROQ_PUBLIC BatchBegin final {
};

}  // namespace client
}  // namespace roq

template <>
struct fmt::formatter<roq::client::BatchBegin> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::client::BatchBegin& value,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(}})");
  }
};

template <>
struct fmt::formatter<roq::Event<roq::client::BatchBegin> > {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::Event<roq::client::BatchBegin>& event,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(batch_begin={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
