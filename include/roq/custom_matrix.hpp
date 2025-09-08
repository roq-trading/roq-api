/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"
#include "roq/compat/fmt.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/limits.hpp"
#include "roq/name.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Custom matrix (publish)
struct ROQ_PUBLIC CustomMatrix final {
  std::string_view label;                   //!< Label
  std::string_view account;                 //!< Account name
  std::string_view exchange;                //!< Exchange
  std::string_view symbol;                  //!< Symbol
  std::span<roq::MatrixKey const> rows;     //!< row labels
  std::span<roq::MatrixKey const> columns;  //!< column labels
  std::span<double const> data;             //!< matrix
  roq::UpdateType update_type = {};         //!< Update type
  uint32_t version = {};                    //!< Version number (does not have to be sequential)
};

template <>
constexpr std::string_view get_name<CustomMatrix>() {
  using namespace std::literals;
  return "custom_matrix"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMatrix> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CustomMatrix const &value, format_context &context) const {
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
        R"(version={})"
        R"(}})"sv,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        fmt::join(value.rows, ", "sv),
        fmt::join(value.columns, ", "sv),
        fmt::join(value.data, ", "sv),
        value.update_type,
        value.version);
  }
};