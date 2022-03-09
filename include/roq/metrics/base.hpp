/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <string_view>

#include "roq/compat.hpp"

namespace roq {
namespace metrics {

//! Helper for all metrics collectors
struct ROQ_PUBLIC Base {
  inline std::string create_labels(const std::string_view &label_name_0, const std::string_view &label_value_0) {
    using namespace std::literals;
    return fmt::format(R"({}="{}")"sv, label_name_0, label_value_0);
  }
  inline std::string create_labels(
      const std::string_view &label_name_0,
      const std::string_view &label_value_0,
      const std::string_view &label_name_1,
      const std::string_view &label_value_1) {
    using namespace std::literals;
    return fmt::format(R"({}="{}", {}="{}")"sv, label_name_0, label_value_0, label_name_1, label_value_1);
  }
  inline std::string create_labels(
      const std::string_view &label_name_0,
      const std::string_view &label_value_0,
      const std::string_view &label_name_1,
      const std::string_view &label_value_1,
      const std::string_view &label_name_2,
      const std::string_view &label_value_2) {
    using namespace std::literals;
    return fmt::format(
        R"({}="{}", {}="{}", {}="{}")"sv,
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
