/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Identifies the beginning of a download sequence
struct ROQ_PUBLIC DownloadBegin final {
  std::string_view account;  //!< Account name
};

template <>
inline constexpr std::string_view get_name<DownloadBegin>() {
  using namespace std::literals;
  return "download_begin"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::DownloadBegin> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::DownloadBegin const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}")"
        R"(}})"sv,
        value.account);
  }
};
