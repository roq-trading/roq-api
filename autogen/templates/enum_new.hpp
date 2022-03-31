{% include 'copyright' %}

#pragma once

#include "roq/enum.hpp"

{% include 'namespace_begin' %}

namespace detail {
  struct {{ snake_case_name }}_t final {
    enum type_t : uint8_t {
      UNDEFINED = 0,
    {% for value in values %}
      {{ value.enum_value }},{{ '  //!< {}'.format(value.comment) if value.comment|length > 0 else '' }}
    {% endfor %}
    };
  };
}

//! {{ comment }}
struct ROQ_PACKED {{ name }} final : public Enum<detail::{{ snake_case_name }}_t::type_t> {                                                                   
  using enum type_t;  // https://stackoverflow.com/a/67000446                                                              

  constexpr {{ name }}() = default;                                                                                              
                                                                                                                                   
  // cppcheck-suppress noExplicitConstructor
  constexpr {{ name }}(type_t type)  // NOLINT (allow implicit)
      : Enum<detail::{{ snake_case_name }}_t::type_t>(type) {}

  constexpr explicit {{ name }}(value_t value)
      : Enum<detail::{{ snake_case_name }}_t::type_t>(value) {}

  constexpr explicit {{ name }}(const std::string_view &value)
      : Enum<detail::{{ snake_case_name }}_t::type_t>(value) {}

  constexpr Enum &operator=(type_t type) {
    type_ = type;
    return *this;
  }
};                                                                                                                         
                                                                                                                           
{% include 'namespace_end' %}

template <>
struct std::is_enum<{{ namespaces | join('::') }}::{{ name }}> : std::true_type {};

template <>
struct std::underlying_type<{{ namespaces | join('::') }}::{{ name }}> {
  using type = {{ namespaces | join('::') }}::{{ name }}::type_t;
};

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::{{ name }}> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::{{ name }}& value,
      Context& context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        "{}"sv,
        value.name());
  }
};
