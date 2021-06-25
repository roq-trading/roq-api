/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cassert>
#include <string_view>
#include <type_traits>

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

  explicit constexpr TradingStatus(uint8_t type) : type_(validate(type)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::START_OF_DAY:
        return "START_OF_DAY"_sv;
      case type_t::PRE_OPEN:
        return "PRE_OPEN"_sv;
      case type_t::PRE_OPEN_NO_CANCEL:
        return "PRE_OPEN_NO_CANCEL"_sv;
      case type_t::PRE_OPEN_FREEZE:
        return "PRE_OPEN_FREEZE"_sv;
      case type_t::OPEN:
        return "OPEN"_sv;
      case type_t::FAST_MARKET:
        return "FAST_MARKET"_sv;
      case type_t::HALT:
        return "HALT"_sv;
      case type_t::CLOSE_NOT_FINAL:
        return "CLOSE_NOT_FINAL"_sv;
      case type_t::PRE_CLOSE:
        return "PRE_CLOSE"_sv;
      case type_t::PRE_CLOSE_NO_CANCEL:
        return "PRE_CLOSE_NO_CANCEL"_sv;
      case type_t::PRE_CLOSE_FREEZE:
        return "PRE_CLOSE_FREEZE"_sv;
      case type_t::CLOSE:
        return "CLOSE"_sv;
      case type_t::POST_CLOSE:
        return "POST_CLOSE"_sv;
      case type_t::END_OF_DAY:
        return "END_OF_DAY"_sv;
      default:
        assert(false);
    }
    return "UNDEFINED"_sv;
  }

  constexpr operator std::string_view() const { return name(); }

 protected:
  constexpr type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::START_OF_DAY:
      case type_t::PRE_OPEN:
      case type_t::PRE_OPEN_NO_CANCEL:
      case type_t::PRE_OPEN_FREEZE:
      case type_t::OPEN:
      case type_t::FAST_MARKET:
      case type_t::HALT:
      case type_t::CLOSE_NOT_FINAL:
      case type_t::PRE_CLOSE:
      case type_t::PRE_CLOSE_NO_CANCEL:
      case type_t::PRE_CLOSE_FREEZE:
      case type_t::CLOSE:
      case type_t::POST_CLOSE:
      case type_t::END_OF_DAY:
        return result;
      default:
        assert(false);
        return type_t::UNDEFINED;
    }
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
