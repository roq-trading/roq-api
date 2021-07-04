/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cassert>
#include <string_view>
#include <type_traits>

#include <magic_enum.hpp>

#include "roq/compat.h"
#include "roq/format.h"
#include "roq/literals.h"

namespace roq {

//! Enumeration of the tradig status of a symbol
struct ROQ_PACKED TradingStatus final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    START_OF_DAY,         //!< No matching, no order actions
    PRE_OPEN,             //!< No matching, all order actions
    PRE_OPEN_NO_CANCEL,   //!< No matching, only new orders
    PRE_OPEN_FREEZE,      //!< Matching, no order actions
    OPEN,                 //!< Matching, all order actions
    FAST_MARKET,          //!< Same as Open, some settings could be relaxed by the exchange
    HALT,                 //!< No matching, only order cancellation
    CLOSE_NOT_FINAL,      //!< Same as Close, state required to support mid-session PreOpen
    PRE_CLOSE,            //!< No matching, all order actions
    PRE_CLOSE_NO_CANCEL,  //!< No matching, only new orders
    PRE_CLOSE_FREEZE,     //!< Matching, no order actions
    CLOSE,                //!< No matching, no order actions, good-for-day orders automatically canceled
    POST_CLOSE,           //!< No matching, all order actions (only with next-trading-day validity)
    END_OF_DAY,           //!< No matching, no order actions
  };

  constexpr TradingStatus() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr TradingStatus(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr TradingStatus(uint8_t value)
      : type_(magic_enum::enum_cast<type_t>(value).value_or(type_t::UNDEFINED)) {}

  explicit constexpr TradingStatus(const std::string_view &value)
      : type_(magic_enum::enum_cast<type_t>(value).value_or(type_t::UNDEFINED)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const { return magic_enum::enum_name(type_); }

  constexpr operator std::string_view() const { return name(); }

  static constexpr auto values() { return magic_enum::enum_values<type_t>(); }

  static constexpr size_t count() { return magic_enum::enum_count<type_t>(); }

  static constexpr TradingStatus from_index(size_t index) { return magic_enum::enum_value<type_t>(index); }

  constexpr size_t to_index() const {
    auto result = magic_enum::enum_index(type_);  // std::optional
    return result.value();                        // note! could throw
  }

 private:
  type_t type_ = type_t::UNDEFINED;
};

}  // namespace roq

template <>
struct std::is_enum<roq::TradingStatus> : std::true_type {};

template <>
struct std::underlying_type<roq::TradingStatus> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::TradingStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::TradingStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, value.name());
  }
};
