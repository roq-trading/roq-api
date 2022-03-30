{% include 'copyright' %}

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string.hpp"

{% include 'includes' %}

{% include 'namespace_begin' %}

//! {{ comment }}
struct ROQ_PUBLIC {{ name }} final {
{% include 'variables' %}
};

{% include 'namespace_end' %}

{% include 'format' %}

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::Event<{{ namespaces | join('::') }}::{{ name }}> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::Event<{{ namespaces | join('::') }}::{{ name }}>& event,
      Context& context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
{%- raw %}
        R"({{)"
{%- endraw %}
        R"(message_info={}, )"
        R"({{ filename }}={})"
{%- raw %}
        R"(}})"sv,
{%- endraw %}
        event.message_info,
        event.value);
  }
};
