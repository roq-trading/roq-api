{% include 'copyright' %}

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/span.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

{% include 'includes' %}

{% include 'namespace_begin' %}

//! {{ comment }}
struct ROQ_PUBLIC {{ name }} final {
{% include 'variables' %}
};

template <>
inline constexpr std::string_view get_name<{{ name }}>() {
  using namespace std::literals;
  return "{{ filename }}"sv;
}

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

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::Trace<{{ namespaces | join('::') }}::{{ name }} const> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::Trace<{{ namespaces | join('::') }}::{{ name }} const>& event,
      Context& context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
{%- raw %}
        R"({{)"
{%- endraw %}
        R"(trace_info={}, )"
        R"({{ filename }}={})"
{%- raw %}
        R"(}})"sv,
{%- endraw %}
        event.trace_info,
        event.value);
  }
};
