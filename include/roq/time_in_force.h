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

//! Order life-time, aka time in force
struct ROQ_PACKED TimeInForce final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    GFD,                    //!< Good for day, aka DAY
    GTC,                    //!< Good till canceled
    OPG,                    //!< At the open
    IOC,                    //!< Immediate or cancel
    FOK,                    //!< Fill or kill
    GTX,                    //!< Good till crossing
    GTD,                    //!< Good till date
    AT_THE_CLOSE,           //!< At the close
    GOOD_THROUGH_CROSSING,  //!< Good through crossing
    AT_CROSSING,            //!< At crossing
    GOOD_FOR_TIME,          //!< Good for time
    GFA,                    //!< Good for auction
    GFM,                    //!< Good for this month
  };

  constexpr TimeInForce() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr TimeInForce(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr TimeInForce(uint8_t type) : type_(validate(type)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const {
    using namespace roq::literals;
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::GFD:
        return "GFD"_sv;
      case type_t::GTC:
        return "GTC"_sv;
      case type_t::OPG:
        return "OPG"_sv;
      case type_t::IOC:
        return "IOC"_sv;
      case type_t::FOK:
        return "FOK"_sv;
      case type_t::GTX:
        return "GTX"_sv;
      case type_t::GTD:
        return "GTD"_sv;
      case type_t::AT_THE_CLOSE:
        return "AT_THE_CLOSE"_sv;
      case type_t::GOOD_THROUGH_CROSSING:
        return "GOOD_THROUGH_CROSSING"_sv;
      case type_t::AT_CROSSING:
        return "AT_CROSSING"_sv;
      case type_t::GOOD_FOR_TIME:
        return "GOOD_FOR_TIME"_sv;
      case type_t::GFA:
        return "GFA"_sv;
      case type_t::GFM:
        return "GFM"_sv;
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
      case type_t::GFD:
      case type_t::GTC:
      case type_t::OPG:
      case type_t::IOC:
      case type_t::FOK:
      case type_t::GTX:
      case type_t::GTD:
      case type_t::AT_THE_CLOSE:
      case type_t::GOOD_THROUGH_CROSSING:
      case type_t::AT_CROSSING:
      case type_t::GOOD_FOR_TIME:
      case type_t::GFA:
      case type_t::GFM:
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
struct std::is_enum<roq::TimeInForce> : std::true_type {};

template <>
struct std::underlying_type<roq::TimeInForce> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::TimeInForce> : public roq::formatter {
  template <typename Context>
  auto format(const roq::TimeInForce &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, value.name());
  }
};
