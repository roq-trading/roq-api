/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

namespace roq {

//! Identifies the end of a download sequence
struct ROQ_PUBLIC DownloadEnd final {
  DownloadEnd() = default;
  DownloadEnd(DownloadEnd &&) = default;
  DownloadEnd(const DownloadEnd &) = delete;

  std::string_view account;   //!< Account name (as known to the gateway)
  uint32_t max_order_id = 0;  //!< Highest previous order identifier (as seen by gateway)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::DownloadEnd> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::DownloadEnd &value, Context &context) {
    return format_to(
        context.out(),

        R"({{)"
        R"(account="{}", )"
        R"(max_order_id={})"

        R"(}})",
        value.account,
        value.max_order_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::DownloadEnd> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::DownloadEnd> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(download_end={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
