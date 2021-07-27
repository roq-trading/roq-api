/* Copyright (c) 2017-2021, Hans Erik Thrane */
#pragma once

#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/span.h"

namespace roq {
namespace client {

struct ROQ_PUBLIC CustomMessage final {
  roq::span<std::byte const> message;
};

}  // namespace client

}  // namespace roq

template <>
struct fmt::formatter<roq::client::CustomMessage> : public roq::formatter {
  template <typename Context>
  auto format(const roq::client::CustomMessage &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(length={})"
        R"(}})"_sv,
        std::size(value.message));
  }
};

template <>
struct fmt::formatter<roq::Event<roq::client::CustomMessage> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::client::CustomMessage> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_message={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
