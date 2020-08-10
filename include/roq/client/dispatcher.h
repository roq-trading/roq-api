/* Copyright (c) 2017-2020, Hans Erik Thrane */

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
  virtual void send(
      const CreateOrder& create_order,
      uint8_t source) = 0;

  //! Send a request to modify an existing order
  virtual void send(
      const ModifyOrder& modify_order,
      uint8_t source) = 0;

  //! Send a request to cancel an existing order
  virtual void send(
      const CancelOrder& cancel_order,
      uint8_t source) = 0;

  virtual void enqueue(const CustomMessage& message) = 0;
};

}  // namespace client
}  // namespace roq
