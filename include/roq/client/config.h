/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/compat.h"
#include "roq/order_cancel_policy.h"
#include "roq/order_management.h"

namespace roq {
namespace client {

//! Settings
struct ROQ_PUBLIC Settings {
  OrderCancelPolicy order_cancel_policy = {};  //!< Cancel orders on disconnect?
  OrderManagement order_management = {};       //!< Order management style
};

//! Account regex
struct ROQ_PUBLIC Account {
  std::string_view regex;  //!< Regular expression
};

//! Symbol regex
struct ROQ_PUBLIC Symbol {
  std::string_view regex;          //!< Regular expression
  std::string_view exchange = {};  //!< Exchange name (optional)
};

//! Dispatch interface used to manage subscriptions.
class ROQ_PUBLIC Config {
 public:
  class ROQ_PUBLIC Handler {
   public:
    virtual void operator()(const Settings &) {}
    virtual void operator()(const Account &) = 0;
    virtual void operator()(const Symbol &) = 0;
  };

  virtual ~Config() {}

  //! Framework will request subscriptions by calling this method
  virtual void dispatch(Handler &) const = 0;
};

}  // namespace client
}  // namespace roq
