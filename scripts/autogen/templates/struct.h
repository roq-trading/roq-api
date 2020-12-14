{% include 'copyright' %}

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/span.h"
#include "roq/fixed_string.h"

{% include 'includes' %}

{% include 'namespace_begin' %}

//! {{ comment }}
struct ROQ_PUBLIC {{ name }} final {
{% include 'variables' %}
};

{% include 'namespace_end' %}

{% include 'format' %}
