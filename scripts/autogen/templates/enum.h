{% include 'copyright' %}

#pragma once

#include <cassert>
#include <string_view>
#include <type_traits>

#include <magic_enum.hpp>

#include "roq/compat.h"
#include "roq/format.h"
#include "roq/literals.h"

{% include 'namespace_begin' %}

//! {{ comment }}
struct ROQ_PACKED {{ name }} final {
  //! helper
  enum type_t : uint8_t {
    UNDEFINED = 0,
  {% for value in values %}
    {{ value.enum_value }},{{ '  //!< {}'.format(value.comment) if value.comment|length > 0 else '' }}
  {% endfor %}
  };

  constexpr {{ name }}() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr {{ name }}(type_t type)  // NOLINT (allow implicit)
      : type_(type) {
  }

  explicit constexpr {{ name }}(uint8_t type)
      : type_(validate(type)) {
  }

  constexpr operator type_t() const {
    return type_;
  }

  constexpr std::string_view name() const {
    return magic_enum::enum_name(type_);
  }

  constexpr operator std::string_view() const {
    return name();
  }

  static constexpr size_t count() {
    return magic_enum::enum_count<type_t>();
  }

  static constexpr {{ name }} from_index(size_t index) {
    return magic_enum::enum_value<type_t>(index);
  }

  constexpr size_t to_index() const {
    auto result = magic_enum::enum_index(type_); // std::optional
    return result.value();  // note! could throw
  }

 protected:
  constexpr type_t validate(uint8_t type) {
    auto result = magic_enum::enum_cast<type_t>(type);
    return result.has_value() ? result.value() : type_t::UNDEFINED;
  }

 private:
  type_t type_ = type_t::UNDEFINED;
};

{% include 'namespace_end' %}

template <>
struct std::is_enum<{{ namespaces | join('::') }}::{{ name }}> : std::true_type {};

template <>
struct std::underlying_type<{{ namespaces | join('::') }}::{{ name }}> {
  using type = uint8_t;
};

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::{{ name }}> : public roq::formatter {
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::{{ name }}& value,
      Context& context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        "{}"_sv,
        value.name());
  }
};
