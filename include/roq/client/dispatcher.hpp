/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/server/trace_info.hpp"

#include "roq/client/custom_message.hpp"

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

  //! Make it possible to populate origin_create_time and receive_time for external events
  template <typename Callback>
  void create_trace_info(Callback callback) {
    TimeSetter time_setter(*this);
    callback(static_cast<const roq::server::TraceInfo &>(time_setter));
  }

 protected:
  virtual roq::server::TraceInfo create_trace_info() = 0;
  virtual void release_trace_info() = 0;

  struct TimeSetter final {
    explicit TimeSetter(Dispatcher &dispatcher)
        : dispatcher_(dispatcher), trace_info_(dispatcher_.create_trace_info()) {}

    ~TimeSetter() {
      try {
        dispatcher_.release_trace_info();
      } catch (...) {
      }
    }

    operator const roq::server::TraceInfo &() const { return trace_info_; }

   private:
    Dispatcher &dispatcher_;
    roq::server::TraceInfo trace_info_;
  };
};

}  // namespace client
}  // namespace roq
