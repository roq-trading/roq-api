/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/exposure.h"
#include <roq/logging.h>
#include <cassert>
#include "reference/utilities.h"

namespace examples {
namespace reference {

void Exposure::update(update_t type, roq::common::TradeDirection direction,
                      double quantity) {
  auto& exposure = get_exposure(direction);
  switch (type) {
    case Create: {
      exposure.created += quantity;
      break;
    }
    case Fill: {
      exposure.filled += quantity;
      LOG_IF(FATAL, is_less_than_or_equal(exposure.filled, exposure.created)) <<
          "Filled quantity (" << exposure.filled << ") should be less than created (" << exposure.created << ")";
      // TODO(thraneh): can also validate other combinations
      break;
    }
    case Cancel: {
      exposure.cancelled += quantity;
      LOG_IF(FATAL, is_less_than_or_equal(exposure.cancelled, exposure.created)) <<
          "Cancelled quantity (" << exposure.cancelled << ") should be less than created (" << exposure.created << ")";
      // TODO(thraneh): can also validate other combinations
      break;
    }
    case Reject: {
      exposure.rejected += quantity;
      LOG_IF(FATAL, is_less_than_or_equal(exposure.rejected, exposure.created)) <<
          "Rejected quantity (" << exposure.rejected << ") should be less than created (" << exposure.created << ")";
      // TODO(thraneh): can also validate other combinations
      break;
    }
  }
}

double Exposure::get(roq::common::TradeDirection direction) const {
  const auto& exposure = get_exposure(direction);
  auto result = exposure.created - exposure.filled - exposure.cancelled - exposure.rejected;
  LOG_IF(FATAL, is_less_than_or_equal(0.0, result)) <<
      "Exposure (" << result << ") is negative! ";
  return result;
}

Exposure::exposure_t& Exposure::get_exposure(roq::common::TradeDirection direction) {
  switch (direction) {
    case roq::common::TradeDirection::Undefined:
      LOG(FATAL) << "Unexpected direction";
    case roq::common::TradeDirection::Buy:
      return _bid;
    case roq::common::TradeDirection::Sell:
      return _offer;
  }
}

const Exposure::exposure_t& Exposure::get_exposure(roq::common::TradeDirection direction) const {
  switch (direction) {
    case roq::common::TradeDirection::Undefined:
      LOG(FATAL) << "Unexpected direction";
    case roq::common::TradeDirection::Buy:
      return _bid;
    case roq::common::TradeDirection::Sell:
      return _offer;
  }
}

}  // namespace reference
}  // namespace examples
