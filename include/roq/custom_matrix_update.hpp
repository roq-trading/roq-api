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
#include "roq/string_types.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Custom matrix (receive)
struct ROQ_PUBLIC CustomMatrixUpdate final {
  std::string_view label;                          //!< Label
  std::string_view account;                        //!< Account name
  std::string_view exchange;                       //!< Exchange
  std::string_view symbol;                         //!< Symbol
  std::span<MatrixKey const> rows;                 //!< row labels
  std::span<MatrixKey const> columns;              //!< column labels
  std::span<double const> data;                    //!< matrix
  UpdateType update_type = {};                     //!< Update type
  uint32_t version = {};                           //!< Version number (does not have to be sequential)
  std::chrono::nanoseconds sending_time_utc = {};  //!< Exchange sending timestamp (UTC)
  std::string_view user;                           //!< User name (origin)
};

template <>
inline constexpr std::string_view get_name<CustomMatrixUpdate>() {
  using namespace std::literals;
  return "custom_matrix_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMatrixUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CustomMatrixUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(rows=[{}], )"
        R"(columns=[{}], )"
        R"(data=[{}], )"
        R"(update_type={}, )"
        R"(version={}, )"
        R"(sending_time_utc={}, )"
        R"(user="{}")"
        R"(}})"sv,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        fmt::join(value.rows, ", "sv),
        fmt::join(value.columns, ", "sv),
        fmt::join(value.data, ", "sv),
        value.update_type,
        value.version,
        value.sending_time_utc,
        value.user);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::CustomMatrixUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::CustomMatrixUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_matrix_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CustomMatrixUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::CustomMatrixUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_matrix_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
