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

#include "roq/layer.h"

namespace roq {

//! Update relating to top of book (aggregate price)
struct ROQ_PUBLIC TopOfBook final {
  TopOfBook() = default;
  TopOfBook(TopOfBook &&) = default;
  TopOfBook(const TopOfBook &) = delete;

  std::string_view exchange;  //!< Exchange name
  std::string_view symbol;    //!< Symbol
  Layer layer;                //!< Top of book
  bool snapshot =
      false;  //!< Full update (possibly old) if true and otherwise an incremental update
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TopOfBook> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::TopOfBook &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(layer={}, )"
        R"(snapshot={}, )"
        R"(exchange_time_utc={})"
        R"(}})"sv,
        value.exchange,
        value.symbol,
        value.layer,
        value.snapshot,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::TopOfBook> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::TopOfBook> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(top_of_book={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
