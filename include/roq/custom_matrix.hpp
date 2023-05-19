/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Custom matrix (publish)
struct ROQ_PUBLIC CustomMatrix final {
  std::string_view label;              //!< Label
  std::string_view account;            //!< Account name
  std::string_view exchange;           //!< Exchange
  std::string_view symbol;             //!< Symbol
  std::span<MatrixKey const> rows;     //!< row labels
  std::span<MatrixKey const> columns;  //!< column labels
  std::span<double const> data;        //!< matrix
  UpdateType update_type = {};         //!< Update type
  uint32_t version = {};               //!< Version number (does not have to be sequential)
};

template <>
inline constexpr std::string_view get_name<CustomMatrix>() {
  using namespace std::literals;
  return "custom_matrix"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMatrix> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::CustomMatrix const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
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
        R"(}})"_cf,
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

template <>
struct fmt::formatter<roq::Event<roq::CustomMatrix>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::CustomMatrix> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_matrix={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CustomMatrix>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::CustomMatrix> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_matrix={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
