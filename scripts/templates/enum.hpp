{% include 'copyright' %}

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

{% include 'namespace_begin' %}

//! {{ comment }}
enum class {{ name }} : uint8_t {
  UNDEFINED = 0,
{% for value in values %}
  {{ value.enum_value }},{{ '  //!< {}'.format(value.comment) if value.comment|length > 0 else '' }}
{% endfor %}
};

{% include 'namespace_end' %}

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::{{ name }}> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(
      {{ namespaces | join('::') }}::{{ name }} const& value,
      Context& context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        "{}"sv,
        magic_enum::enum_name(value));
  }
};
