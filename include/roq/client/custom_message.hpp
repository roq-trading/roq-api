/* Copyright (c) 2017-2025, Hans Erik Thrane */
#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <span>

#include "roq/event.hpp"

namespace roq {
namespace client {

struct ROQ_PUBLIC CustomMessage final {
  std::span<std::byte const> message;
};

}  // namespace client

}  // namespace roq

template <>
struct fmt::formatter<roq::client::CustomMessage> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::client::CustomMessage const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(length={})"
        R"(}})"sv,
        std::size(value.message));
  }
};

template <>
struct fmt::formatter<roq::Event<roq::client::CustomMessage>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::client::CustomMessage> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_message={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
