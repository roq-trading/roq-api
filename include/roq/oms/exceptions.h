/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string>

#include "roq/api.h"

namespace roq {
namespace oms {

class ROQ_PUBLIC Exception : public roq::Exception {
 public:
  char const *what() const noexcept override {
    using namespace std::literals;
    if (std::empty(what_))  // lazy
      what_ = fmt::format("OMS: {}"sv, *this);
    return what_.c_str();
  }

 protected:
  Exception(const source_location &loc, Origin origin, RequestStatus status, Error error)
      : roq::Exception(loc), origin(origin), status(status), error(error) {}

 private:
  mutable std::string what_;  // lazy

 public:
  const Origin origin;
  const RequestStatus status;
  const Error error;
};

//! Rejected
class ROQ_PUBLIC Rejected : public Exception {
 protected:
  Rejected(const source_location &loc, Origin origin, Error error)
      : Exception(loc, origin, RequestStatus::REJECTED, error) {}
};

struct RejectedException final : public Rejected {
  RejectedException(Origin origin, Error error, const source_location &loc = source_location::current())
      : Rejected(loc, origin, error) {}
};

//! NotSupported
class ROQ_PUBLIC NotSupported : public Rejected {
 protected:
  explicit NotSupported(const source_location &loc) : Rejected(loc, Origin::GATEWAY, Error::NOT_SUPPORTED) {}
};

struct NotSupportedException final : public NotSupported {
  explicit NotSupportedException(const source_location &loc = source_location::current()) : NotSupported(loc) {}
};

//! NotReady
class ROQ_PUBLIC NotReady : public Rejected {
 protected:
  explicit NotReady(const source_location &loc) : Rejected(loc, Origin::GATEWAY, Error::GATEWAY_NOT_READY) {}
};

struct NotReadyException final : public NotReady {
  explicit NotReadyException(const source_location &loc = source_location::current()) : NotReady(loc) {}
};

}  // namespace oms
}  // namespace roq

template <>
struct fmt::formatter<roq::oms::Exception> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::oms::Exception &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(error={})"
        R"(}})"sv,
        value.error);
  }
};
