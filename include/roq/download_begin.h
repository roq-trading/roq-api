/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Identifies the beginning of a download sequence
struct ROQ_PUBLIC DownloadBegin final {
  std::string_view account;  //!< Account name
};

}  // namespace roq

template <>
struct fmt::formatter<roq::DownloadBegin> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::DownloadBegin &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(account="{}")"
        R"(}})"sv,
        value.account);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::DownloadBegin> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::DownloadBegin> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(download_begin={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
