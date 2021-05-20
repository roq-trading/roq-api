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

//! Enumeration of support types
struct ROQ_PACKED SupportType final {
  //! helper
  enum type_t : uint64_t {
    UNDEFINED = 0x0,
    REFERENCE_DATA = 0x1,    //!< Reference data
    MARKET_STATUS = 0x2,     //!< Market status
    TOP_OF_BOOK = 0x4,       //!< Top of book
    MARKET_BY_PRICE = 0x8,   //!< Market by price
    MARKET_BY_ORDER = 0x10,  //!< Market by order
    TRADE_SUMMARY = 0x20,    //!< Trade summary
    STATISTICS = 0x40,       //!< Statistics
    CREATE_ORDER = 0x10000,  //!< Create order
    MODIFY_ORDER = 0x20000,  //!< Modify order
    CANCEL_ORDER = 0x40000,  //!< Cancel order
    ORDER_ACK = 0x80000,     //!< Order ack
    ORDER = 0x100000,        //!< Order
    ORDER_STATE = 0x800000,  //!< Order
    TRADE = 0x200000,        //!< Trade
    POSITION = 0x400000,     //!< Position
    FUNDS = 0x10000000,      //!< Funds
  };

  constexpr SupportType() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr SupportType(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr SupportType(uint64_t type) : type_(validate(type)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::REFERENCE_DATA:
        return "REFERENCE_DATA"_sv;
      case type_t::MARKET_STATUS:
        return "MARKET_STATUS"_sv;
      case type_t::TOP_OF_BOOK:
        return "TOP_OF_BOOK"_sv;
      case type_t::MARKET_BY_PRICE:
        return "MARKET_BY_PRICE"_sv;
      case type_t::MARKET_BY_ORDER:
        return "MARKET_BY_ORDER"_sv;
      case type_t::TRADE_SUMMARY:
        return "TRADE_SUMMARY"_sv;
      case type_t::STATISTICS:
        return "STATISTICS"_sv;
      case type_t::CREATE_ORDER:
        return "CREATE_ORDER"_sv;
      case type_t::MODIFY_ORDER:
        return "MODIFY_ORDER"_sv;
      case type_t::CANCEL_ORDER:
        return "CANCEL_ORDER"_sv;
      case type_t::ORDER_ACK:
        return "ORDER_ACK"_sv;
      case type_t::ORDER:
        return "ORDER"_sv;
      case type_t::ORDER_STATE:
        return "ORDER_STATE"_sv;
      case type_t::TRADE:
        return "TRADE"_sv;
      case type_t::POSITION:
        return "POSITION"_sv;
      case type_t::FUNDS:
        return "FUNDS"_sv;
      default:
        assert(false);
    }
    return "UNDEFINED"_sv;
  }

  constexpr operator std::string_view() const { return name(); }

 protected:
  constexpr type_t validate(uint64_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::REFERENCE_DATA:
      case type_t::MARKET_STATUS:
      case type_t::TOP_OF_BOOK:
      case type_t::MARKET_BY_PRICE:
      case type_t::MARKET_BY_ORDER:
      case type_t::TRADE_SUMMARY:
      case type_t::STATISTICS:
      case type_t::CREATE_ORDER:
      case type_t::MODIFY_ORDER:
      case type_t::CANCEL_ORDER:
      case type_t::ORDER_ACK:
      case type_t::ORDER:
      case type_t::ORDER_STATE:
      case type_t::TRADE:
      case type_t::POSITION:
      case type_t::FUNDS:
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
struct std::is_enum<roq::SupportType> : std::true_type {};

template <>
struct std::underlying_type<roq::SupportType> {
  using type = uint64_t;
};

template <>
struct fmt::formatter<roq::SupportType> : public roq::formatter {
  template <typename Context>
  auto format(const roq::SupportType &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_fmt, value.name());
  }
};
