{% include 'copyright' %}

#pragma once

#include <fmt/format.h>

#include <cassert>

{% include 'namespace_begin' %}

//! {{ comment }}
enum class {{ name }} : uint64_t {
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
      Context& context) {
    using namespace std::literals;
#if __cplusplus >= 202002L
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
#else
    std::string_view name{[&]() {
      switch (value) {
        case {{ namespaces | join('::') }}::{{ name }}::UNDEFINED:
          return "UNDEFINED"sv;
    {% for value in values %}
        case {{ namespaces | join('::') }}::{{ name }}::{{ value.enum_value }}:
          return "{{ value.enum_value }}"sv;
    {% endfor %}
        default:
          assert(false);
      }
      return "<UNKNOWN>"sv;
    }()};
#endif
    return fmt::format_to(
        context.out(),
        "{}"sv,
        name);
  }
};
