/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string_buffer.hpp"

namespace roq {

//! Identifies the end of a download sequence
struct ROQ_PUBLIC DownloadEnd final {
  std::string_view account;    //!< Account name
  uint32_t max_order_id = {};  //!< Highest previous order identifier (as seen by gateway)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::DownloadEnd> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::DownloadEnd &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(account="{}", )"
        R"(max_order_id={})"
        R"(}})"sv,
        value.account,
        value.max_order_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::DownloadEnd> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::DownloadEnd> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(download_end={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
