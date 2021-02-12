/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

namespace roq {

//! Identifies the beginning of a download sequence
struct ROQ_PUBLIC DownloadBegin final {
  DownloadBegin() = default;
  DownloadBegin(DownloadBegin &&) = default;
  DownloadBegin(const DownloadBegin &) = delete;

  std::string_view account;  //!< Account name (as known to the gateway)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::DownloadBegin> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::DownloadBegin &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(account="{}")"
        R"(}})"sv,
        value.account);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::DownloadBegin> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::DownloadBegin> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(download_begin={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
