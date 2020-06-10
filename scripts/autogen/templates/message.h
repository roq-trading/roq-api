{% include 'copyright' %}

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/event.h"
#include "roq/span.h"
#include "roq/string.h"

#include "roq/message_info.h"

{% include 'includes' %}

{% include 'namespace_begin' %}

struct ROQ_PUBLIC {{ name }} final {
{% include 'variables' %}
};

struct ROQ_PUBLIC {{ name }}Event final {
  const MessageInfo& message_info;
  const {{ name }}& {{ filename }};
};

template <>
inline {{ name }}Event create_event(
    const MessageInfo& message_info,
    const {{ name }}Event& event) {
  return decltype(event) {
    .message_info = message_info,
    .{{ filename }} = event.{{ filename }},
  };
}

namespace detail {
template <>
struct event_value_helper<{{ name }}Event> final {
  using type = {{ name }};
  explicit event_value_helper(const {{ name }}Event& event)
      : _event(event) {
  }
  operator const type&() const {
    return _event.{{ filename }};
  }
 private:
  const {{ name }}Event& _event;
};
}  // namespace detail

{% include 'namespace_end' %}

{% include 'format' %}

template <>
struct fmt::formatter<{{ namespaces | join('::') }}::{{ name }}Event> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const {{ namespaces | join('::') }}::{{ name }}Event& value,
      Context& context) {
    return format_to(
        context.out(),
{%- raw %}
        R"({{)"
{% endraw %}
        R"(message_info={}, )"
        R"({{ filename }}={})"
{%- raw %}
        R"(}})",
{% endraw %}
        value.message_info,
        value.{{ filename }});
  }
};
