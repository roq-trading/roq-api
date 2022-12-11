/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/trace_info.hpp"

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
  virtual void send(CreateOrder const &, uint8_t source, bool is_last = true) = 0;

  //! Send a request to modify an existing order
  virtual void send(ModifyOrder const &, uint8_t source, bool is_last = true) = 0;

  //! Send a request to cancel an existing order
  virtual void send(CancelOrder const &, uint8_t source, bool is_last = true) = 0;

  //! Send a request to cancel all orders
  virtual void send(CancelAllOrders const &, uint8_t source) = 0;

  //! Broadcast custom metrics
  virtual void send(CustomMetrics const &, uint8_t source) = 0;

  //! Useful to communicate between threads
  virtual void enqueue(CustomMessage const &) = 0;

  //! Make it possible to populate origin_create_time and receive_time for external events
  template <typename Callback>
  void create_trace_info(Callback callback) {
    TimeSetter time_setter{*this};
    callback(static_cast<TraceInfo const &>(time_setter));
  }

 protected:
  virtual TraceInfo create_trace_info() = 0;
  virtual void release_trace_info() = 0;

  struct TimeSetter final {
    explicit TimeSetter(Dispatcher &dispatcher)
        : dispatcher_{dispatcher}, trace_info_{dispatcher_.create_trace_info()} {}

    ~TimeSetter() {
      try {
        dispatcher_.release_trace_info();
      } catch (...) {
      }
    }

    operator TraceInfo const &() const { return trace_info_; }

   private:
    Dispatcher &dispatcher_;
    TraceInfo trace_info_;
  };
};

}  // namespace client
}  // namespace roq
