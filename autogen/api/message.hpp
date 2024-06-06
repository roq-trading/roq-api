{% include 'copyright' %}

#pragma once

#include "roq/compat.hpp"


{% include 'fmt_includes' %}


{% include 'std_includes' %}


#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"
{%- include 'roq_includes' %}

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
  constexpr auto parse(format_parse_context &context) {
    return std::begin(context);
  }
  auto format(
      {{ namespaces | join('::') }}::Event<{{ namespaces | join('::') }}::{{ name }}> const& event,
      format_context& context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
{%- raw %}
        R"({{)"
{%- endraw %}
        R"({{ filename }}={}, )"
        R"(message_info={})"
{%- raw %}
        R"(}})"sv,
{%- endraw %}
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::Trace<{{ namespaces | join('::') }}::{{ name }}> > {
  constexpr auto parse(format_parse_context &context) {
    return std::begin(context);
  }
  auto format(
      {{ namespaces | join('::') }}::Trace<{{ namespaces | join('::') }}::{{ name }}> const& event,
      format_context& context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
{%- raw %}
        R"({{)"
{%- endraw %}
        R"({{ filename }}={}, )"
        R"(trace_info={})"
{%- raw %}
        R"(}})"sv,
{%- endraw %}
        event.value,
        event.trace_info);
  }
};
