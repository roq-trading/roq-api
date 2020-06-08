/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <string_view>

#include "roq/compat.h"

namespace roq {
namespace metrics {

struct ROQ_PUBLIC Base {
  inline std::string create_labels(
      const std::string_view& label_name_0,
      const std::string_view& label_value_0) {
    return fmt::format(
      FMT_STRING(R"({}="{}")"),
      label_name_0,
      label_value_0);
  }
  inline std::string create_labels(
      const std::string_view& label_name_0,
      const std::string_view& label_value_0,
      const std::string_view& label_name_1,
      const std::string_view& label_value_1) {
  return fmt::format(
      FMT_STRING(R"({}="{}", {}="{}")"),
      label_name_0,
      label_value_0,
      label_name_1,
      label_value_1);
  }
  inline std::string create_labels(
      const std::string_view& label_name_0,
      const std::string_view& label_value_0,
      const std::string_view& label_name_1,
      const std::string_view& label_value_1,
      const std::string_view& label_name_2,
      const std::string_view& label_value_2) {
  return fmt::format(
      FMT_STRING(R"({}="{}", {}="{}", {}="{}")"),
      label_name_0,
      label_value_0,
      label_name_1,
      label_value_1,
      label_name_2,
      label_value_2);
  }
};

}  // namespace metrics
}  // namespace roq
