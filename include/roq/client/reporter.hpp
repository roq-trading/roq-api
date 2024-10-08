/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/client/collector.hpp"

namespace roq {
namespace client {

struct Reporter : public Collector {
  virtual void print() const = 0;
};

}  // namespace client
}  // namespace roq
