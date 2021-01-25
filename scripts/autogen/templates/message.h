{% include 'copyright' %}

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/span.h"
#include "roq/fixed_string.h"

#include "roq/event.h"
#include "roq/message_info.h"

{% include 'includes' %}

{% include 'namespace_begin' %}

//! {{ comment }}
struct ROQ_PUBLIC {{ name }} final {
  {{ name }}() = default;
  {{ name }}({{name }}&&) = default;
  {{ name }}(const {{name }}&) = delete;

{% include 'variables' %}
};

{% include 'namespace_end' %}

{% include 'format' %}

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::Event<{{ namespaces | join('::') }}::{{ name }}> > {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::Event<{{ namespaces | join('::') }}::{{ name }}>& event,
      Context& context) {
    return format_to(
        context.out(),
{%- raw %}
        R"({{)"
{%- endraw %}
        R"(message_info={}, )"
        R"({{ filename }}={})"
{%- raw %}
        R"(}})",
{%- endraw %}
        event.message_info,
        event.value);
  }
};
