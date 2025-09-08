/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"
#include "roq/compat/fmt.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/position.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"
#include "roq/uuid.hpp"

namespace roq {

//! Portfolio
struct ROQ_PUBLIC Portfolio final {
  std::string_view user;                            //!< User (optional)
  uint32_t strategy_id = {};                        //!< Strategy identifier (optional)
  std::string_view account;                         //!< Account name
  std::span<roq::Position const> positions;         //!< Position updates
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  roq::UUID session_id = {};                        //!< Reference (UUID)
  uint64_t seqno = {};                              //!< Reference (sequencing)
};

template <>
constexpr std::string_view get_name<Portfolio>() {
  using namespace std::literals;
  return "portfolio"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Portfolio> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Portfolio const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(user="{}", )"
        R"(strategy_id={}, )"
        R"(account="{}", )"
        R"(positions=[{}], )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(session_id={}, )"
        R"(seqno={})"
        R"(}})"sv,
        value.user,
        value.strategy_id,
        value.account,
        fmt::join(value.positions, ", "sv),
        value.update_type,
        value.exchange_time_utc,
        value.session_id,
        value.seqno);
  }
};