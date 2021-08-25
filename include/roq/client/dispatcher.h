/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

#include "roq/client/custom_message.h"

namespace roq {
namespace client {

//! Dispatch interface used to manage client requests
/*!
 * The implementation can throw if the request
 * is invalid or if the environment is known to
 * not be in a good state. For example, if a
 * connection is not yet established.
 *
 * It is implementation specific if the request
 * is handled synchronously or asynchronously.
 */
class ROQ_PUBLIC Dispatcher {
 public:
  virtual ~Dispatcher() {}

  virtual void stop() = 0;

  //! Send a request to create a new order
  virtual void send(const CreateOrder &, uint8_t source) = 0;

  //! Send a request to modify an existing order
  virtual void send(const ModifyOrder &, uint8_t source) = 0;

  //! Send a request to cancel an existing order
  virtual void send(const CancelOrder &, uint8_t source) = 0;

  //! Send a request to cancel all orders
  virtual void send(const CancelAllOrders &, uint8_t source) = 0;

  //! Broadcast custom metrics
  virtual void send(const CustomMetrics &, uint8_t source) = 0;

  //! Useful to communicate between threads
  virtual void enqueue(const CustomMessage &) = 0;
};

}  // namespace client
}  // namespace roq
