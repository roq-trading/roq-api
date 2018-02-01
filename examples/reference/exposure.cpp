/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/exposure.h"
#include <glog/logging.h>
#include <cassert>
#include "reference/utilities.h"

namespace examples {
namespace reference {

double Exposure::get() const {
  auto result = _created - _filled - _cancelled - _rejected;
  LOG_IF(FATAL, is_less_than_or_equal(0.0, result)) <<
      "Exposure (" << result << ") is negative! ";
  return result;
}

void Exposure::create(double quantity) {
  _created += quantity;
}

void Exposure::fill(double quantity) {
  _filled += quantity;
  LOG_IF(FATAL, is_less_than_or_equal(_filled, _created)) <<
      "Filled quantity (" << _filled << ") should be less than create (" << _created << ")";
  // TODO(thraneh): can also validate other combinations
}

void Exposure::cancel(double quantity) {
  _cancelled += quantity;
  LOG_IF(FATAL, is_less_than_or_equal(_cancelled, _created)) <<
      "Cancelled quantity (" << _cancelled << ") should be less than create (" << _created << ")";
  // TODO(thraneh): can also validate other combinations
}

void Exposure::reject(double quantity) {
  _rejected += quantity;
  LOG_IF(FATAL, is_less_than_or_equal(_rejected, _created)) <<
      "Rejected quantity (" << _rejected << ") should be less than create (" << _created << ")";
  // TODO(thraneh): can also validate other combinations
}

}  // namespace reference
}  // namespace examples
