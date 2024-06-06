{% include 'copyright' %}

#pragma once

{% include 'fmt_includes' %}

{% include 'std_includes' %}
#include <cassert>

{% include 'namespace_begin' %}

//! {{ comment }}
enum class {{ name }} : uint64_t {
  UNDEFINED = 0x0,
{%- for value in values %}
  {{ value.enum_value }} = {{ "{:#x}".format((1).__lshift__(value.position)) }},{{ '  //!< {}'.format(value.comment) if value.comment|length > 0 else '' }}
{%- endfor %}
};

{% include 'namespace_end' %}

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::{{ name }}> {
  constexpr auto parse(format_parse_context &context) {
    return std::begin(context);
  }
  auto format(
      {{ namespaces | join('::') }}::{{ name }} const& value,
      format_context& context) const {
    using namespace std::literals;
    auto name{[&]() {
      switch (value) {
        using enum {{ namespaces | join('::') }}::{{ name }};
        case UNDEFINED:
          return "UNDEFINED"sv;
    {%- for value in values %}
        case {{ value.enum_value }}:
          return "{{ value.enum_value }}"sv;
    {%- endfor %}
        default:
          assert(false);
      }
      return "<UNKNOWN>"sv;
    }()};
    return fmt::format_to(
        context.out(),
        "{}"sv,
        name);
  }
};
