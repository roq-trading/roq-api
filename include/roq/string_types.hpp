/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/string.hpp"

namespace roq {

namespace detail {
static constexpr auto const MAX_LENGTH_SOURCE = size_t{16};
static constexpr auto const MAX_LENGTH_USER = size_t{16};

static constexpr auto const MAX_LENGTH_ACCOUNT = size_t{32};
static constexpr auto const MAX_LENGTH_EXCHANGE = size_t{32};
static constexpr auto const MAX_LENGTH_SYMBOL = size_t{48};
static constexpr auto const MAX_LENGTH_CURRENCY = size_t{32};

static constexpr auto const MAX_LENGTH_MBO_ORDER_ID = size_t{36};  // note! UUID

static constexpr auto const MAX_LENGTH_REQUEST_TEMPLATE = size_t{16};
static constexpr auto const MAX_LENGTH_EXTERNAL_ACCOUNT = size_t{64};
static constexpr auto const MAX_LENGTH_EXTERNAL_ORDER_ID = size_t{64};
static constexpr auto const MAX_LENGTH_EXTERNAL_TRADE_ID = size_t{40};
static constexpr auto const MAX_LENGTH_ROUTING_ID = size_t{64};

static constexpr auto const MAX_LENGTH_CL_ORD_ID = size_t{36};
static constexpr auto const MAX_LENGTH_REQUEST_ID = size_t{36};

static constexpr auto const MAX_LENGTH_LABEL = size_t{32};
static constexpr auto const MAX_LENGTH_MEASUREMENT_KEY = size_t{8};

static constexpr auto const MAX_LENGTH_MATRIX_KEY = size_t{8};

static constexpr auto const MAX_LENGTH_DESCRIPTION = size_t{128};
static constexpr auto const MAX_LENGTH_TIME_ZONE = size_t{32};

static constexpr auto const MAX_LENGTH_PARAMETER_KEY = size_t{32};
static constexpr auto const MAX_LENGTH_PARAMETER_VALUE = size_t{32};
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Source const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::User> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::User const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Account> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Account const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Exchange> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Exchange const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Symbol> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Symbol const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Currency> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Currency const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::MBOOrderId> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::MBOOrderId const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::RequestTemplate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RequestTemplate const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ExternalAccount> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ExternalAccount const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ExternalOrderId> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ExternalOrderId const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ExternalTradeId> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ExternalTradeId const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::RoutingId> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RoutingId const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ClOrdId> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ClOrdId const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::RequestId> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RequestId const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Label> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Label const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::MeasurementKey> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::MeasurementKey const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::MatrixKey> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::MatrixKey const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::Description> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Description const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::TimeZone> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::TimeZone const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ParameterKey> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ParameterKey const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};

template <>
struct fmt::formatter<roq::ParameterValue> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ParameterValue const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<std::string_view>(value));
  }
};
