/* Copyright (c) 2017-2022, Hans Erik Thrane */
#pragma once

#include <fmt/format.h>

#include <span>

#include "roq/compat.h"
#include "roq/event.h"

namespace roq {
namespace client {

struct ROQ_PUBLIC CustomMessage final {
  std::span<std::byte const> message;
};

}  // namespace client

}  // namespace roq

template <>
struct fmt::formatter<roq::client::CustomMessage> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::client::CustomMessage &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(length={})"
        R"(}})"sv,
        std::size(value.message));
  }
};

template <>
struct fmt::formatter<roq::Event<roq::client::CustomMessage> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::client::CustomMessage> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_message={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
