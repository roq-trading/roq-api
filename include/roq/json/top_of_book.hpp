/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/top_of_book.hpp"

#include "roq/json/layer.hpp"

#include "roq/json/context.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct TopOfBook final {
  TopOfBook(Context const &context, roq::TopOfBook const &value) : context_{context}, value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("layer":{},)"
        R"("update_type":{},)"
        R"("exchange_time_utc":{},)"
        R"("exchange_sequence":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        Layer{context_, value_.layer},
        String{value_.update_type},
        DateTime{value_.exchange_time_utc},
        value_.exchange_sequence);
  }

 private:
  Context const &context_;
  roq::TopOfBook const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::TopOfBook> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::TopOfBook const &value, Context &context) const {
    return value.format_to(context);
  }
};
