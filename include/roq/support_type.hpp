/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

namespace roq {

//! Enumeration of support types
enum class SupportType : uint64_t {
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

}  // namespace roq

template <>
struct fmt::formatter<roq::SupportType> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::SupportType &value, Context &context) {
    using namespace std::literals;
    std::string_view name{[&]() {
      switch (value) {
        using enum roq::SupportType;
        case UNDEFINED:
          return "UNDEFINED"sv;
        case REFERENCE_DATA:
          return "REFERENCE_DATA"sv;
        case MARKET_STATUS:
          return "MARKET_STATUS"sv;
        case TOP_OF_BOOK:
          return "TOP_OF_BOOK"sv;
        case MARKET_BY_PRICE:
          return "MARKET_BY_PRICE"sv;
        case MARKET_BY_ORDER:
          return "MARKET_BY_ORDER"sv;
        case TRADE_SUMMARY:
          return "TRADE_SUMMARY"sv;
        case STATISTICS:
          return "STATISTICS"sv;
        case CREATE_ORDER:
          return "CREATE_ORDER"sv;
        case MODIFY_ORDER:
          return "MODIFY_ORDER"sv;
        case CANCEL_ORDER:
          return "CANCEL_ORDER"sv;
        case ORDER_ACK:
          return "ORDER_ACK"sv;
        case ORDER:
          return "ORDER"sv;
        case ORDER_STATE:
          return "ORDER_STATE"sv;
        case TRADE:
          return "TRADE"sv;
        case POSITION:
          return "POSITION"sv;
        case FUNDS:
          return "FUNDS"sv;
        default:
          assert(false);
      }
      return "<UNKNOWN>"sv;
    }()};
    return fmt::format_to(context.out(), "{}"sv, name);
  }
};
