/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <string_view>

namespace roq {
namespace metrics {

//! Helper for all metrics collectors
struct ROQ_PUBLIC Base {
  inline std::string create_labels(std::string_view const &label_name_0, std::string_view const &label_value_0) {
    using namespace fmt::literals;
    return fmt::format(R"({}="{}")"_cf, label_name_0, label_value_0);
  }
  inline std::string create_labels(
      std::string_view const &label_name_0,
      std::string_view const &label_value_0,
      std::string_view const &label_name_1,
      std::string_view const &label_value_1) {
    using namespace fmt::literals;
    return fmt::format(R"({}="{}", {}="{}")"_cf, label_name_0, label_value_0, label_name_1, label_value_1);
  }
  inline std::string create_labels(
      std::string_view const &label_name_0,
      std::string_view const &label_value_0,
      std::string_view const &label_name_1,
      std::string_view const &label_value_1,
      std::string_view const &label_name_2,
      std::string_view const &label_value_2) {
    using namespace fmt::literals;
    return fmt::format(
        R"({}="{}", {}="{}", {}="{}")"_cf,
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
