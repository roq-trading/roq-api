/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/cancel_policy.h"
#include "roq/compat.h"

namespace roq {
namespace client {

//! Settings
struct ROQ_PUBLIC Settings {
  CancelPolicy cancel_policy = {};  //!< Cancel orders on disconnect?
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
