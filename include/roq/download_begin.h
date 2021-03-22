/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Identifies the beginning of a download sequence
struct ROQ_PUBLIC DownloadBegin final {
  std::string_view account;  //!< Account name (as known to the gateway)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::DownloadBegin> : public roq::formatter {
  template <typename Context>
  auto format(const roq::DownloadBegin &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}")"
        R"(}})"_fmt,
        value.account);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::DownloadBegin> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::DownloadBegin> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(download_begin={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
