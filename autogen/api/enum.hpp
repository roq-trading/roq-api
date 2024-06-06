{% include 'copyright' %}

#pragma once

#include <magic_enum.hpp>

{% include 'fmt_includes' %}

{% include 'std_includes' %}

{% include 'namespace_begin' %}

//! {{ comment }}
enum class {{ name }} : uint8_t {
  UNDEFINED = 0,
{%- for value in values %}
  {{ value.enum_value }},{{ '  //!< {}'.format(value.comment) if value.comment|length > 0 else '' }}
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
    return fmt::format_to(
        context.out(),
        "{}"sv,
        magic_enum::enum_name(value));
  }
};
