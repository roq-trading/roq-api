/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"

namespace roq {

//! Enumeration of execution types
struct ROQ_PACKED ExecutionInstruction final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
    PARTICIPATE_DO_NOT_INITIATE,
    CANCEL_IF_NOT_BEST,
    DO_NOT_INCREASE,
    DO_NOT_REDUCE,
    MAX = DO_NOT_REDUCE
  };

  ExecutionInstruction() = default;

  // cppcheck-suppress noExplicitConstructor
  inline ExecutionInstruction(type_t type)  // NOLINT
      : type_(type) {}

  inline explicit ExecutionInstruction(uint8_t type) : type_(validate(type)) {}

  inline operator type_t() const { return type_; }

  inline std::string_view name() const {
    using namespace std::literals;  // NOLINT
    switch (type_) {
      case type_t::UNDEFINED:
        break;
      case type_t::PARTICIPATE_DO_NOT_INITIATE:
        return "PARTICIPATE_DO_NOT_INITIATE"sv;
      case type_t::CANCEL_IF_NOT_BEST:
        return "CANCEL_IF_NOT_BEST"sv;
      case type_t::DO_NOT_INCREASE:
        return "DO_NOT_INCREASE"sv;
      case type_t::DO_NOT_REDUCE:
        return "DO_NOT_REDUCE"sv;
      default:
        assert(false);
    }
    return "UNDEFINED"sv;
  }

  inline operator std::string_view() const { return name(); }

 protected:
  inline type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
      case type_t::PARTICIPATE_DO_NOT_INITIATE:
      case type_t::CANCEL_IF_NOT_BEST:
      case type_t::DO_NOT_INCREASE:
      case type_t::DO_NOT_REDUCE:
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
struct std::is_enum<roq::ExecutionInstruction> : std::true_type {};

template <>
struct std::underlying_type<roq::ExecutionInstruction> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<roq::ExecutionInstruction> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::ExecutionInstruction &value, Context &context) {
    return format_to(context.out(), "{}", value.name());
  }
};
