/* Copyright (c) 2017-2024, Hans Erik Thrane */
#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::client::CustomMessage const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(length={})"
        R"(}})"_cf,
        std::size(value.message));
  }
};

template <>
struct fmt::formatter<roq::Event<roq::client::CustomMessage>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::client::CustomMessage> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_message={})"
        R"(}})"_cf,
        event.message_info,
        event.value);
  }
};
