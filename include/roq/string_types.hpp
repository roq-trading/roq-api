/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/compat/fmt.hpp"

#include <fmt/format.h>

#include "roq/string.hpp"

namespace roq {

namespace detail {
static constexpr size_t const MAX_LENGTH_SOURCE = 16;
static constexpr size_t const MAX_LENGTH_USER = 16;

static constexpr size_t const MAX_LENGTH_ACCOUNT = 32;
static constexpr size_t const MAX_LENGTH_EXCHANGE = 32;
static constexpr size_t const MAX_LENGTH_SYMBOL = 64;
static constexpr size_t const MAX_LENGTH_CURRENCY = 32;

static constexpr size_t const MAX_LENGTH_CFI_CODE = 6;

static constexpr size_t const MAX_LENGTH_MBO_ORDER_ID = 36;  // note! UUID

static constexpr size_t const MAX_LENGTH_REQUEST_TEMPLATE = 16;
static constexpr size_t const MAX_LENGTH_EXTERNAL_ACCOUNT = 64;
static constexpr size_t const MAX_LENGTH_EXTERNAL_ORDER_ID = 64;
static constexpr size_t const MAX_LENGTH_EXTERNAL_TRADE_ID = 40;
static constexpr size_t const MAX_LENGTH_ROUTING_ID = 64;

static constexpr size_t const MAX_LENGTH_CL_ORD_ID = 36;
static constexpr size_t const MAX_LENGTH_REQUEST_ID = 36;

static constexpr size_t const MAX_LENGTH_LABEL = 32;
static constexpr size_t const MAX_LENGTH_MEASUREMENT_KEY = 8;

static constexpr size_t const MAX_LENGTH_MATRIX_KEY = 8;

static constexpr size_t const MAX_LENGTH_DESCRIPTION = 128;
static constexpr size_t const MAX_LENGTH_TIME_ZONE = 32;

static constexpr size_t const MAX_LENGTH_PARAMETER_KEY = 32;
static constexpr size_t const MAX_LENGTH_PARAMETER_VALUE = 32;
}  // namespace detail

struct ROQ_PUBLIC Source final : public String<detail::MAX_LENGTH_USER> {
  using String<detail::MAX_LENGTH_USER>::String;
};

struct ROQ_PUBLIC User final : public String<detail::MAX_LENGTH_USER> {
  using String<detail::MAX_LENGTH_USER>::String;
};

struct ROQ_PUBLIC Account final : public String<detail::MAX_LENGTH_ACCOUNT> {
  using String<detail::MAX_LENGTH_ACCOUNT>::String;
};

struct ROQ_PUBLIC Exchange final : public String<detail::MAX_LENGTH_EXCHANGE> {
  using String<detail::MAX_LENGTH_EXCHANGE>::String;
};

struct ROQ_PUBLIC Symbol final : public String<detail::MAX_LENGTH_SYMBOL> {
  using String<detail::MAX_LENGTH_SYMBOL>::String;
};

struct ROQ_PUBLIC Currency final : public String<detail::MAX_LENGTH_CURRENCY> {
  using String<detail::MAX_LENGTH_CURRENCY>::String;
};

struct ROQ_PUBLIC CFICode final : public String<detail::MAX_LENGTH_CFI_CODE> {
  using String<detail::MAX_LENGTH_CFI_CODE>::String;
};

struct ROQ_PUBLIC MBOOrderId final : public String<detail::MAX_LENGTH_MBO_ORDER_ID> {
  using String<detail::MAX_LENGTH_MBO_ORDER_ID>::String;
};

struct ROQ_PUBLIC RequestTemplate final : public String<detail::MAX_LENGTH_REQUEST_TEMPLATE> {
  using String<detail::MAX_LENGTH_REQUEST_TEMPLATE>::String;
};

struct ROQ_PUBLIC ExternalAccount final : public String<detail::MAX_LENGTH_EXTERNAL_ACCOUNT> {
  using String<detail::MAX_LENGTH_EXTERNAL_ACCOUNT>::String;
};

struct ROQ_PUBLIC ExternalOrderId final : public String<detail::MAX_LENGTH_EXTERNAL_ORDER_ID> {
  using String<detail::MAX_LENGTH_EXTERNAL_ORDER_ID>::String;
};

struct ROQ_PUBLIC ExternalTradeId final : public String<detail::MAX_LENGTH_EXTERNAL_TRADE_ID> {
  using String<detail::MAX_LENGTH_EXTERNAL_TRADE_ID>::String;
};

struct ROQ_PUBLIC RoutingId final : public String<detail::MAX_LENGTH_ROUTING_ID> {
  using String<detail::MAX_LENGTH_ROUTING_ID>::String;
};

struct ROQ_PUBLIC ClOrdId final : public String<detail::MAX_LENGTH_CL_ORD_ID> {
  using String<detail::MAX_LENGTH_CL_ORD_ID>::String;
};

struct ROQ_PUBLIC RequestId final : public String<detail::MAX_LENGTH_REQUEST_ID> {
  using String<detail::MAX_LENGTH_REQUEST_ID>::String;
};

struct ROQ_PUBLIC Label final : public String<detail::MAX_LENGTH_LABEL> {
  using String<detail::MAX_LENGTH_LABEL>::String;
};

struct ROQ_PUBLIC MeasurementKey final : public String<detail::MAX_LENGTH_MEASUREMENT_KEY> {
  using String<detail::MAX_LENGTH_MEASUREMENT_KEY>::String;
};

struct ROQ_PUBLIC MatrixKey final : public String<detail::MAX_LENGTH_MATRIX_KEY> {
  using String<detail::MAX_LENGTH_MATRIX_KEY>::String;
};

struct ROQ_PUBLIC Description final : public String<detail::MAX_LENGTH_DESCRIPTION> {
  using String<detail::MAX_LENGTH_DESCRIPTION>::String;
};

struct ROQ_PUBLIC TimeZone final : public String<detail::MAX_LENGTH_TIME_ZONE> {
  using String<detail::MAX_LENGTH_TIME_ZONE>::String;
};

struct ROQ_PUBLIC ParameterKey final : public String<detail::MAX_LENGTH_PARAMETER_KEY> {
  using String<detail::MAX_LENGTH_PARAMETER_KEY>::String;
};

struct ROQ_PUBLIC ParameterValue final : public String<detail::MAX_LENGTH_PARAMETER_VALUE> {
  using String<detail::MAX_LENGTH_PARAMETER_VALUE>::String;
};

// validate

static_assert(sizeof(Source) == detail::MAX_LENGTH_USER);
static_assert(sizeof(User) == detail::MAX_LENGTH_USER);
static_assert(sizeof(Account) == detail::MAX_LENGTH_ACCOUNT);
static_assert(sizeof(Exchange) == detail::MAX_LENGTH_EXCHANGE);
static_assert(sizeof(Symbol) == detail::MAX_LENGTH_SYMBOL);
static_assert(sizeof(Currency) == detail::MAX_LENGTH_CURRENCY);
static_assert(sizeof(CFICode) == detail::MAX_LENGTH_CFI_CODE);
static_assert(sizeof(MBOOrderId) == detail::MAX_LENGTH_MBO_ORDER_ID);
static_assert(sizeof(RequestTemplate) == detail::MAX_LENGTH_REQUEST_TEMPLATE);
static_assert(sizeof(ExternalAccount) == detail::MAX_LENGTH_EXTERNAL_ACCOUNT);
static_assert(sizeof(ExternalOrderId) == detail::MAX_LENGTH_EXTERNAL_ORDER_ID);
static_assert(sizeof(ExternalTradeId) == detail::MAX_LENGTH_EXTERNAL_TRADE_ID);
static_assert(sizeof(RoutingId) == detail::MAX_LENGTH_ROUTING_ID);
static_assert(sizeof(ClOrdId) == detail::MAX_LENGTH_CL_ORD_ID);
static_assert(sizeof(RequestId) == detail::MAX_LENGTH_REQUEST_ID);
static_assert(sizeof(Label) == detail::MAX_LENGTH_LABEL);
static_assert(sizeof(MeasurementKey) == detail::MAX_LENGTH_MEASUREMENT_KEY);
static_assert(sizeof(MatrixKey) == detail::MAX_LENGTH_MATRIX_KEY);
static_assert(sizeof(Description) == detail::MAX_LENGTH_DESCRIPTION);
static_assert(sizeof(TimeZone) == detail::MAX_LENGTH_TIME_ZONE);
static_assert(sizeof(ParameterKey) == detail::MAX_LENGTH_PARAMETER_KEY);
static_assert(sizeof(ParameterValue) == detail::MAX_LENGTH_PARAMETER_VALUE);

}  // namespace roq

template <>
struct fmt::formatter<roq::Source> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Source const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::User> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::User const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Account> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Account const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Exchange> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Exchange const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Symbol> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Symbol const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Currency> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Currency const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::CFICode> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CFICode const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::MBOOrderId> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MBOOrderId const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::RequestTemplate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RequestTemplate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ExternalAccount> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ExternalAccount const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ExternalOrderId> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ExternalOrderId const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ExternalTradeId> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ExternalTradeId const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::RoutingId> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RoutingId const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ClOrdId> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ClOrdId const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::RequestId> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RequestId const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Label> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Label const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::MeasurementKey> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MeasurementKey const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::MatrixKey> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MatrixKey const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Description> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Description const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::TimeZone> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TimeZone const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ParameterKey> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ParameterKey const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ParameterValue> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ParameterValue const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
