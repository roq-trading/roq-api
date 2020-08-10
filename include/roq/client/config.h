/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/compat.h"

namespace roq {
namespace client {

//! Account regex
struct ROQ_PUBLIC Account {
  std::string_view regex;  //!< Regular expression
};

//! Symbol regex
struct ROQ_PUBLIC Symbol {
  std::string_view regex;  //!< Regular expression
  std::string_view exchange = std::string_view();  //!< Exchange name (optional)
};

//! Dispatch interface used to manage subscriptions.
class ROQ_PUBLIC Config {
 public:
  class ROQ_PUBLIC Handler {
   public:
    virtual void operator()(const Account& account) = 0;
    virtual void operator()(const Symbol& symbol) = 0;
  };

  virtual ~Config() {}

  //! Framework will request subscriptions by calling this method
  virtual void dispatch(Handler& handler) const = 0;
};

}  // namespace client
}  // namespace roq
