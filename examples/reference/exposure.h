/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>

namespace examples {
namespace reference {

// meant to catch programming mistakes
class Exposure {
 public:
  double get() const;
  void create(double quantity);
  void fill(double quantity);
  void cancel(double quantity);
  void reject(double quantity);
 private:
  double _created = 0.0;
  double _filled = 0.0;
  double _cancelled = 0.0;
  double _rejected = 0.0;
};

}  // namespace reference
}  // namespace examples
