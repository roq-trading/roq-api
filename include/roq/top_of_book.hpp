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
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string.hpp"

#include "roq/layer.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to top of book (aggregate price)
struct ROQ_PUBLIC TopOfBook final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  Layer layer;                                      //!< Top of book
  UpdateType update_type = {};                      //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TopOfBook> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::TopOfBook &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(layer={}, )"
        R"(update_type={}, )"
        R"(exchange_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.layer,
        value.update_type,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::TopOfBook> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::TopOfBook> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(top_of_book={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
