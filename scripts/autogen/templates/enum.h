{% include 'copyright' %}

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "roq/compat.h"

{% include 'namespace_begin' %}

//! {{ comment }}
struct ROQ_PACKED {{ name }} final {
  enum type_t : uint8_t {
    UNDEFINED = 0,
  {% for value in values %}
    {{ value.enum_value }},{{ '  //!< {}'.format(value.comment) if value.comment|length > 0 else '' }}
    {% if loop.last %}
    MAX = {{ value.enum_value }}
    {% endif  %}
  {% endfor %}
  };

  {{ name }}() = default;

  inline {{ name }}(type_t type)  // NOLINT
      : _type(type) {
  }

  inline explicit {{ name }}(uint8_t type)
      : _type(validate(type)) {
  }

  inline operator type_t() const {
    return _type;
  }

  inline std::string_view name() const {
    constexpr std::string_view UNDEFINED("UNDEFINED");
  {% for value in values %}
    constexpr std::string_view {{ value.enum_value }}("{{ value.enum_value }}");
  {% endfor %}
    switch (_type) {
      case type_t::UNDEFINED:
        break;
  {% for value in values %}
      case type_t::{{ value.enum_value }}:
        return {{ value.enum_value }};
  {% endfor %}
      default:
        assert(false);
    }
    return UNDEFINED;
  }

 protected:
  inline type_t validate(uint8_t type) {
    auto result = static_cast<type_t>(type);
    switch (result) {
      case type_t::UNDEFINED:
  {% for value in values %}
      case type_t::{{ value.enum_value }}:
  {% endfor %}
        return result;
      default:
        assert(false);
        return type_t::UNDEFINED;
    }
  }

 private:
  type_t _type = type_t::UNDEFINED;
};

{% include 'namespace_end' %}

template <>
struct std::is_enum<{{ namespaces | join('::') }}::{{ name }}> : std::true_type {};

template <>
struct std::underlying_type<{{ namespaces | join('::') }}::{{ name }}> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::{{ name }}> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::{{ name }}& value,
      Context& context) {
    return format_to(
        context.out(),
        "{}",
        value.name());
  }
};
