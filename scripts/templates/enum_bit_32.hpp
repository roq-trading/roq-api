{% include 'copyright' %}

#pragma once

#include "roq/compat.hpp"


{% include 'fmt_includes' %}


{% include 'std_includes' %}
#include <cassert>

{% include 'namespace_begin' %}

//! {{ comment }}
enum class {{ name }} : uint32_t {
  UNDEFINED = 0x0,
{% for value in values %}
  {{ value.enum_value }} = {{ "{:#x}".format((1).__lshift__(value.position)) }},{{ '  //!< {}'.format(value.comment) if value.comment|length > 0 else '' }}
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
      Context& context) const {
    using namespace std::literals;
    std::string_view name{[&]() {
      switch (value) {
        using enum {{ namespaces | join('::') }}::{{ name }};
        case UNDEFINED:
          return "UNDEFINED"sv;
    {% for value in values %}
        case {{ value.enum_value }}:
          return "{{ value.enum_value }}"sv;
    {% endfor %}
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
