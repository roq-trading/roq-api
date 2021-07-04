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

//! Last known order status received from exchange
struct ROQ_PACKED OrderStatus final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    SENT,       //!< Order has been sent to exchange (no response has been received yet)
    ACCEPTED,   //!< Order has been accepted by exchange and is not yet been activated
    SUSPENDED,  //!< Order has been suspended and requires external action to re-activate
    WORKING,    //!< Order is working and has not yet been completely filled
    STOPPED,    //!< Order has guaranteed fill at as of yet unknown price
    COMPLETED,  //!< Order has been completely filled
    EXPIRED,    //!< Order has expired
    CANCELED,   //!< Order has been canceled
    REJECTED,   //!< Order has been rejected
  };

  constexpr OrderStatus() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr OrderStatus(type_t type)  // NOLINT (allow implicit)
      : type_(type) {}

  explicit constexpr OrderStatus(uint8_t type) : type_(validate(type)) {}

  constexpr operator type_t() const { return type_; }

  constexpr std::string_view name() const { return magic_enum::enum_name(type_); }

  constexpr operator std::string_view() const { return name(); }

  static constexpr size_t count() { return magic_enum::enum_count<type_t>(); }

  static constexpr OrderStatus from_index(size_t index) { return magic_enum::enum_value<type_t>(index); }

  constexpr size_t to_index() const {
    auto result = magic_enum::enum_index(type_);  // std::optional
    return result.value();                        // note! could throw
  }

 protected:
  constexpr type_t validate(uint8_t type) {
    auto result = magic_enum::enum_cast<type_t>(type);
    return result.has_value() ? result.value() : type_t::UNDEFINED;
  }

 private:
  type_t type_ = type_t::UNDEFINED;
};

}  // namespace roq

template <>
struct std::is_enum<roq::OrderStatus> : std::true_type {};

template <>
struct std::underlying_type<roq::OrderStatus> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::OrderStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, value.name());
  }
};
