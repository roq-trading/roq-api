/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <string_view>

#include "roq/numbers.hpp"

#include "roq/error.hpp"
#include "roq/origin.hpp"
#include "roq/request_status.hpp"
#include "roq/request_type.hpp"

namespace roq {
namespace oms {

//! Response (all fields are optional)
struct ROQ_PUBLIC Response final {
  RequestType request_type = {};
  Origin origin = {};
  RequestStatus request_status = {};
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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::oms::Response const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(request_type={}, )"
        R"(origin={}, )"
        R"(request_status={}, )"
        R"(error={}, )"
        R"(text="{}", )"
        R"(version={}, )"
        R"(request_id="{}", )"
        R"(quantity={}, )"
        R"(price={})"
        R"(}})"sv,
        value.request_type,
        value.origin,
        value.request_status,
        value.error,
        value.text,
        value.version,
        value.request_id,
        value.quantity,
        value.price);
  }
};
