/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string>
#include <utility>

#include "roq/api.hpp"

namespace roq {
namespace oms {

struct ROQ_PUBLIC Exception : public roq::Exception {
  template <typename... Args>
  Exception(Origin origin, RequestStatus status, Error error, format_str<Args...> const &fmt, Args &&...args)
      : roq::Exception(fmt, std::forward<Args>(args)...), origin(origin), status(status), error(error) {}

  char const *what() const noexcept override {
    using namespace std::literals;
    if (std::empty(what_))  // lazy
      what_ = fmt::format(
          R"(OMS: )"
          R"({{)"
          R"(error={})"
          R"(}})"sv,
          error);
    return what_.c_str();
  }

 private:
  mutable std::string what_;  // lazy

 public:
  const Origin origin;
  const RequestStatus status;
  const Error error;
};

//! Rejected
struct ROQ_PUBLIC Rejected : public Exception {
  template <typename... Args>
  Rejected(Origin origin, Error error, format_str<Args...> const &fmt, Args &&...args)
      : Exception(origin, RequestStatus::REJECTED, error, fmt, std::forward<Args>(args)...) {}
};

//! NotSupported
struct ROQ_PUBLIC NotSupported : public Rejected {
  template <typename... Args>
  explicit NotSupported(format_str<Args...> const &fmt, Args &&...args)
      : Rejected(Origin::GATEWAY, Error::NOT_SUPPORTED, fmt, std::forward<Args>(args)...) {}
};

//! NotReady
struct ROQ_PUBLIC NotReady : public Rejected {
  template <typename... Args>
  explicit NotReady(format_str<Args...> const &fmt, Args &&...args)
      : Rejected(Origin::GATEWAY, Error::GATEWAY_NOT_READY, fmt, std::forward<Args>(args)...) {}
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
  auto format(roq::oms::Exception const &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(error={})"
        R"(}})"sv,
        value.error);
  }
};
