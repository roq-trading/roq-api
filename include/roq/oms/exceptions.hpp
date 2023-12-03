/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <string>
#include <utility>

#include "roq/api.hpp"

namespace roq {
namespace oms {

struct ROQ_PUBLIC Exception : public roq::Exception {
  template <typename... Args>
  Exception(Origin origin, RequestStatus status, Error error, format_str<Args...> const &fmt, Args &&...args)
      : roq::Exception{fmt, std::forward<Args>(args)...}, origin{origin}, status{status}, error{error} {}

  char const *what() const noexcept override {
    using namespace fmt::literals;
    if (std::empty(what_))  // lazy
      what_ = fmt::format(
          R"(OMS: )"
          R"({{)"
          R"(error={})"
          R"(}})"_cf,
          error);
    return what_.c_str();
  }

 private:
  mutable std::string what_;  // lazy

 public:
  Origin const origin;
  RequestStatus const status;
  Error const error;
};

//! Rejected
struct ROQ_PUBLIC Rejected : public Exception {
  template <typename... Args>
  Rejected(Origin origin, Error error, format_str<Args...> const &fmt = format_str<Args...>{"rejected"}, Args &&...args)
      : Exception{origin, RequestStatus::REJECTED, error, fmt, std::forward<Args>(args)...} {}
};

//! NotSupported
struct ROQ_PUBLIC NotSupported : public Rejected {
  template <typename... Args>
  explicit NotSupported(format_str<Args...> const &fmt = format_str<Args...>{"not supported"}, Args &&...args)
      : Rejected{Origin::GATEWAY, Error::NOT_SUPPORTED, fmt, std::forward<Args>(args)...} {}
};

//! NotReady
struct ROQ_PUBLIC NotReady : public Rejected {
  template <typename... Args>
  explicit NotReady(format_str<Args...> const &fmt = format_str<Args...>{"not ready"}, Args &&...args)
      : Rejected{Origin::GATEWAY, Error::GATEWAY_NOT_READY, fmt, std::forward<Args>(args)...} {}
};

}  // namespace oms
}  // namespace roq

template <>
struct fmt::formatter<roq::oms::Exception> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::oms::Exception const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(error={})"
        R"(}})"_cf,
        value.error);
  }
};
