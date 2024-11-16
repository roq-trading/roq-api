/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <magic_enum/magic_enum.hpp>

#include <nameof.hpp>

#include "roq/exceptions.hpp"

namespace roq {
namespace utils {

template <typename T>
T parse_enum(auto &value) {
  auto result = magic_enum::enum_cast<T>(value, magic_enum::case_insensitive);
  if (result.has_value())
    return result.value();
  using namespace std::literals;
  throw RuntimeError{R"(Unexpected: value="{}" ({}))"sv, value, nameof::nameof_full_type<T>()};
}

}  // namespace utils
}  // namespace roq
