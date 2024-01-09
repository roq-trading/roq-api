/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/position.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Portfolio update
struct ROQ_PUBLIC PortfolioUpdate final {
  std::string_view user;                            //!< User (optional)
  uint32_t strategy_id = {};                        //!< Strategy identifier (optional)
  std::string_view account;                         //!< Account name
  std::span<Position const> positions;              //!< Position updates
  UpdateType update_type = {};                      //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
};

template <>
inline constexpr std::string_view get_name<PortfolioUpdate>() {
  using namespace std::literals;
  return "portfolio_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::PortfolioUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::PortfolioUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(user="{}", )"
        R"(strategy_id={}, )"
        R"(account="{}", )"
        R"(positions=[{}], )"
        R"(update_type={}, )"
        R"(exchange_time_utc={})"
        R"(}})"_cf,
        value.user,
        value.strategy_id,
        value.account,
        fmt::join(value.positions, ", "sv),
        value.update_type,
        value.exchange_time_utc);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::PortfolioUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::PortfolioUpdate> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(portfolio_update={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::PortfolioUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::PortfolioUpdate> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(portfolio_update={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
