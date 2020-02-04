/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/metrics.h"

#include <fmt/format.h>

namespace roq {

std::string MetricsBase::create_labels(
    const std::string_view& label_name_0,
    const std::string_view& label_value_0) {
  return fmt::format(
      FMT_STRING("{}=\"{}\""),
      label_name_0,
      label_value_0);
}

std::string MetricsBase::create_labels(
    const std::string_view& label_name_0,
    const std::string_view& label_value_0,
    const std::string_view& label_name_1,
    const std::string_view& label_value_1) {
  return fmt::format(
      FMT_STRING("{}=\"{}\", {}=\"{}\""),
      label_name_0,
      label_value_0,
      label_name_1,
      label_value_1);
}

std::string MetricsBase::create_labels(
    const std::string_view& label_name_0,
    const std::string_view& label_value_0,
    const std::string_view& label_name_1,
    const std::string_view& label_value_1,
    const std::string_view& label_name_2,
    const std::string_view& label_value_2) {
  return fmt::format(
      FMT_STRING("{}=\"{}\", {}=\"{}\", {}=\"{}\""),
      label_name_0,
      label_value_0,
      label_name_1,
      label_value_1,
      label_name_2,
      label_value_2);
}

}  // namespace roq
