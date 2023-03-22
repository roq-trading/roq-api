/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace cache {

struct ROQ_PUBLIC CancelOrder final {
  CancelOrder(roq::CancelOrder const &value)  // NOLINT
      : account{value.account}, order_id{value.order_id}, request_template{value.request_template},
        routing_id{value.routing_id}, version{value.version}, conditional_on_version{value.conditional_on_version} {}

  CancelOrder(CancelOrder const &) = delete;
  CancelOrder(CancelOrder &&) = default;

  operator roq::CancelOrder() const {
    return {
        .account = account,
        .order_id = order_id,
        .request_template = request_template,
        .routing_id = routing_id,
        .version = version,
        .conditional_on_version = conditional_on_version,
    };
  }

  Account account;
  uint32_t order_id = {};
  RequestTemplate request_template;
  RoutingId routing_id;
  uint32_t version = {};
  uint32_t conditional_on_version = {};
};

}  // namespace cache
}  // namespace roq
