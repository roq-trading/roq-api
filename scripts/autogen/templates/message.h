{% include 'copyright' %}

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/span.h"

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
struct fmt::formatter<{{ namespaces | join('::') }}::Event<{{ namespaces | join('::') }}::{{ name }}> > : public roq::formatter {
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::Event<{{ namespaces | join('::') }}::{{ name }}>& event,
      Context& context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
{%- raw %}
        R"({{)"
{%- endraw %}
        R"(message_info={}, )"
        R"({{ filename }}={})"
{%- raw %}
        R"(}})"_fmt,
{%- endraw %}
        event.message_info,
        event.value);
  }
};
