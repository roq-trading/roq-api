/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace oms {

//! Response (all fields are optional)
struct ROQ_PUBLIC Response final {
  RequestType type = {};
  Origin origin = {};
  RequestStatus status = {};
  Error error = {};
  std::string_view text;
  uint32_t version = {};
  std::string_view request_id;
  double quantity = NaN;
  double price = NaN;
};

}  // namespace oms
}  // namespace roq

template <>
struct fmt::formatter<roq::oms::Response> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::oms::Response &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(type={}, )"
        R"(origin={}, )"
        R"(status={}, )"
        R"(error={}, )"
        R"(text="{}", )"
        R"(version={}, )"
        R"(request_id="{}", )"
        R"(quantity={}, )"
        R"(price={})"
        R"(}})"sv,
        value.type,
        value.origin,
        value.status,
        value.error,
        value.text,
        value.version,
        value.request_id,
        value.quantity,
        value.price);
  }
};