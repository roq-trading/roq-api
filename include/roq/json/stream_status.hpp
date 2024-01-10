/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/stream_status.hpp"

#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct StreamStatus final {
  explicit StreamStatus(roq::StreamStatus const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("account":{},)"
        R"("supports":{},)"
        R"("transport":{},)"
        R"("protocol":{},)"
        R"("encoding":{},)"
        R"("priority":{},)"
        R"("connection_status":{},)"
        R"("interface":{},)"
        R"("authority":{},)"
        R"("path":{},)"
        R"("proxy":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.account},
        String{value_.supports},
        String{value_.transport},
        String{value_.protocol},
        String{value_.encoding},
        String{value_.priority},
        String{value_.connection_status},
        String{value_.interface},
        String{value_.authority},
        String{value_.path},
        String{value_.proxy});
  }

 private:
  roq::StreamStatus const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::StreamStatus> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::StreamStatus const &value, format_context &context) const { return value.format_to(context); }
};
