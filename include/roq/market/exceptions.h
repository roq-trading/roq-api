/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <string>
#include <utility>

#include "roq/api.h"

namespace roq {
namespace market {

class ROQ_PUBLIC BadState : public RuntimeError {
 protected:
  BadState(const source_location &loc, const std::string_view &exchange, const std::string_view &symbol)
      : RuntimeError(loc, R"("Order book in bad state (exchange="{}", symbol="{}"))"_sv, exchange, symbol),
        exchange(exchange), symbol(symbol) {}

 public:
  const std::string_view exchange;
  const std::string_view symbol;
};

struct BadStateException final : public BadState {
  BadStateException(
      const std::string_view &exchange,
      const std::string_view &symbol,
      const source_location &loc = source_location::current())
      : BadState(loc, exchange, symbol) {}
};

}  // namespace market
}  // namespace roq
