/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace oms {

struct ROQ_PUBLIC TradeUpdate final {
  std::string_view account;
  uint32_t order_id = {};
  std::string_view exchange;
  std::string_view symbol;
  Side side = {};
  PositionEffect position_effect = {};
  std::chrono::nanoseconds create_time_utc = {};
  std::chrono::nanoseconds update_time_utc = {};
  std::string_view external_account;
  std::string_view external_order_id;
  std::span<Fill> fills;
  UpdateType update_type = {};
  std::chrono::nanoseconds sending_time_utc = {};
};

}  // namespace oms
}  // namespace roq

template <>
struct fmt::formatter<roq::oms::TradeUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::oms::TradeUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(position_effect={}, )"
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(fills=[{}], )"
        R"(update_type={}, )"
        R"(sending_time_utc={})"
        R"(}})"_cf,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.create_time_utc,
        value.update_time_utc,
        value.external_account,
        value.external_order_id,
        fmt::join(value.fills, ", "sv),
        value.update_type,
        value.sending_time_utc);
  }
};
